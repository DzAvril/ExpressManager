#include "inoutoperator.h"
#include "dboperate.h"
#include <QDebug>
#include "config.h"
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlError>
#include <QPoint>

InOutOperator::InOutOperator(QObject *parent) : QObject(parent) {
    db = new DbOperate();
    speech = Speech::getInstance();
    fileIo = FileIo::getInstance();
    commonHelper = CommonHelper::getInstance();
    model = db->expTableModel();
    model->setSort(db->OUTDATE, Qt::SortOrder::DescendingOrder);
    filterString.clear();
    db->SetFilter(filterString);
    filterBarcode.clear();
    filterOutDate.clear();
    filterStartOutDate.clear();
    filterEndOutDate.clear();

    CreateLines();
}

bool InOutOperator::in(QString barcode, QString name, QString phone) {
    if (db->IsItemExist(barcode)) {
        qDebug() << "Already has item " << barcode;
        // speech->say(ITEM_EXIST);
        return true;
    }
    if (!db->InsertItem(barcode, name, phone)) {
        // speech->say(IN_ERROR);
        return false;
    }
    return true;
}

bool InOutOperator::out(QString barcode, QString photoUrl) {
    if (!db->IsItemExist(barcode)) {
        qDebug("There is no %s in database.", barcode.toStdString().c_str());
        speech->say(NO_SUCH_ITEM);
        fileIo->DeleteFile(photoUrl);
        return false;
    }
    QDateTime timestamp = QDateTime::currentDateTime();
    QString timeStr = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    commonHelper->PutTextToImage(photoUrl, barcode, timeStr);
    if (!(db->UpdateClientPhotoUrl(barcode, photoUrl))
        || (!db->UpdateIsTaken(barcode, 1))
        || (!db->UpdateOutDate(barcode, timeStr))) {
        speech->say(OUT_ERROR);
        fileIo->DeleteFile(photoUrl);
        return false;
    }
    speech->say(OUT_SUCCESS);
    emit updateDatabaseDone();
    return true;
}

bool InOutOperator::isItemAlreadyOut(QString barcode) {
    if (db->IsItemOut(barcode)) {
        speech->say(ALREADY_EXIST);
        return true;
    }
    return false;
}

void InOutOperator::setBarcodeFilter(QString barcode) {
    if (barcode == "") {
        filterBarcode.clear();
    } else {
        filterBarcode = barcode;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setOutDateFilter(QString outdate) {
    if (outdate == "") {
        filterOutDate.clear();
    } else {
        filterOutDate = outdate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setStartOutDateFilter(QString startOutDate) {
    if (startOutDate == "") {
        filterStartOutDate.clear();
    } else {
        filterStartOutDate = startOutDate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setEndOutDateFilter(QString endOutDate) {
    if (endOutDate == "") {
        filterEndOutDate.clear();
    } else {
        filterEndOutDate = endOutDate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::resetFilter() {
    filterBarcode.clear();
    filterOutDate.clear();
    filterStartOutDate.clear();
    filterEndOutDate.clear();
    filterString.clear();
    db->SetFilter(filterString);
}

void InOutOperator::updateOrderPhoto(QString barcode, QString photoUrl) {
    if (!db->IsItemExist(barcode)) {
        qDebug("There is no %s in database.", barcode.toStdString().c_str());
        speech->say(NO_SUCH_ITEM);
        fileIo->DeleteFile(photoUrl);
        return;
    }
    QDateTime timestamp = QDateTime::currentDateTime();
    QString timeStr = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    commonHelper->PutTextToImage(photoUrl, barcode, timeStr);
    if (!(db->UpdateOrderPhotoUrl(barcode, photoUrl))) {
        fileIo->DeleteFile(photoUrl);
        return;
    }
    return;
}

QString InOutOperator::getEarliestExpDate() const {

    int counts = model->rowCount();
    if (counts != 0) {
        QString s = get(counts - 1, db->OUTDATE);
        return s.mid(0, 10); // 2019-01-01
    } else {
        return nullptr;
    }
}

int InOutOperator::getExpCountFromDateRange(QString start, QString end) {
    QString tempFilter = QString("strftime('%Y-%m-%d', OutDate) >= '%1' and strftime('%Y-%m-%d', OutDate) <= '%2' ").arg(
                             start, end);
    db->SetFilter(tempFilter);
    int count =  model->rowCount();
    db->SetFilter(filterString);
    return count;
}

int InOutOperator::getExpCountOfMonth(QString year) {
    QVector<QPointF> points;
    int max = 0;
    for (int mon = 0; mon <= 11; ++mon) {
        QString startDate = QString("date('%1-01-01', '+%2 month')").arg(year, QString::number(mon));
        QString endDate = QString("date('%1-01-01', '+%2 month', '+1 month', '-1 day')").arg(year, QString::number(mon));
        QString tempFilter = QString("strftime('%Y-%m-%d', OutDate) >= %1 and strftime('%Y-%m-%d', OutDate) <= %2").arg(
                                 startDate, endDate);
        db->SetFilter(tempFilter);
        int count =  model->rowCount();
        if (count > max) {
            max = count;
        }
        points.append(QPointF(mon + 1, count));
    }
    m_lineYear->replace(points);
    db->SetFilter(filterString);
    return (max % 10 == 0) && (max != 0) ? max : ((max / 10) + 1) * 10;
}

int InOutOperator::getExpCountOfDay(QString year, QString month) {
    QVector<QPointF> points;
    int max = 0; //max of axisY
    int monthDiff = month.toInt() - 1;
    for (int day = 0; day < 31; ++day) {
        QString tempFilter = QString("strftime('%Y-%m-%d', OutDate) = date('%1-01-01', '+%2 month', '+%3 day')").arg(year,
                             QString::number(monthDiff), QString::number(day));
        db->SetFilter(tempFilter);
        int count =  model->rowCount();
        if (count > max) {
            max = count;
        }
        points.append(QPointF(day + 1, count));
    }
    m_lineMonth->replace(points);
    db->SetFilter(filterString);
    return (max % 10 == 0) && (max != 0) ? max : ((max / 10) + 1) * 10;
}

QString InOutOperator::get(int row, int role) const {
    return model->record(row).value(role).toString();
}

bool InOutOperator::deleteRow(int row) {
    if (model->removeRow(row)) {
        if (model->submit()) {
            emit updateDatabaseDone();
            return true;
        }
    }
    qDebug() << "Delete row error : " << model->lastError().text();
    return false;
}

QStringList InOutOperator::yearList() {
    m_yearList.clear();
    int earliestYear;
    if(getEarliestExpDate() == nullptr) {
        earliestYear = 2019;
    } else {
        earliestYear = QDate::fromString(getEarliestExpDate(), "yyyy-MM-dd").year();
    }
    int thisYear = QDate::currentDate().year();
    for (int year = earliestYear; year <= thisYear; ++year) {
        m_yearList.append(QString::number(year));
    }
    return m_yearList;
}

void InOutOperator::set_lineYear(QLineSeries *line) {
    if (m_lineYear == line) {
        return;
    }
    if (m_lineYear)
        if (m_lineYear->parent() == this) {
            delete m_lineYear;
        }
    m_lineYear = line;
    emit lineYearChanged();
}

void InOutOperator::set_lineMonth(QLineSeries *line) {
    if (m_lineMonth == line) {
        return;
    }
    if (m_lineMonth)
        if (m_lineMonth->parent() == this) {
            delete m_lineMonth;
        }
    m_lineMonth = line;
    emit lineMonthChanged();
}

void InOutOperator::PackFilterFrame() {
    filterString.clear();
    filters.clear();
    if (!filterBarcode.isEmpty()) {
        filters.append(QString("Barcode like '%1%'").arg(filterBarcode));
    }
    if (!filterOutDate.isEmpty()) {
        filters.append(QString("strftime('%Y-%m-%d', OutDate) = '%1'").arg(filterOutDate));
    }
    if ((!filterStartOutDate.isEmpty()) && (!filterEndOutDate.isEmpty())) {
        filters.append(QString("strftime('%Y-%m-%d', OutDate) >= '%1' and strftime('%Y-%m-%d', OutDate) <= '%2' ").arg(
                           filterStartOutDate, filterEndOutDate));
    }
    for (int ix = 0; ix < filters.size(); ++ix) {
        if (ix != (filters.size() - 1)) {
            filterString += filters.at(ix);
            filterString += " and ";
        } else {
            filterString += filters.at(ix);
        }
    }
    qDebug() << "Filter string is " << filterString;
}

void InOutOperator::CreateLines() {
    m_lineYear = new QLineSeries();
    m_lineMonth = new QLineSeries();
}
