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
    recordModel = db->GetRecordModel();
    yearModel = db->GetYearModel();
    monthModel = db->GetMonthModel();
    dayModel = db->GetDayModel();

    QDateTime timestamp = QDateTime::currentDateTime();
    localTimeNow = timestamp.toString("yyyy-MM-dd hh:mm:ss");

    filterString =
        QString("strftime('%Y-%m-%d %H:%M:%S', OutDate) > strftime('%Y-%m-%d %H:%M:%S', '%1') and IsTaken = 1").arg(
            localTimeNow);
    //            QString("rowid > %1 and IsTaken = 1").arg((recordModel->rowCount() - 10) > 0 ?
    //                   (recordModel->rowCount() - 10) : 0);
    //        QString("strftime('%Y-%m-%d', OutDate) >= date('now','-1 month') and strftime('%Y-%m-%d', OutDate) <= date('now') and IsTaken = 1");
    db->SetFilter(filterString);
    filterBarcode.clear();
    filterOutDate.clear();
    filterStartOutDate.clear();
    filterEndOutDate.clear();
    GetEarliestExpDate();
    CreateLines();
    GetMost();
}

InOutOperator::~InOutOperator() {
    delete recordModel;
    delete yearModel;
    delete monthModel;
    delete dayModel;
    delete m_lineYear;
    delete m_lineMonth;
}

bool InOutOperator::in(QString barcode, QString name, QString phone) {
    if (db->IsItemExist(barcode)) {
        qDebug() << "Already has item " << barcode;
        // speech->say(ITEM_EXIST);
        return true;
    }
    if (!db->InsertRecord(barcode, name, phone)) {
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
        speech->say("出库失败");
        fileIo->DeleteFile(photoUrl);
        return false;
    }
    //    speech->say("出库成功");
    UpdateCount(true);
    GetMost();
    emit updateDatabaseDone();
    return true;
}

bool InOutOperator::isItemAlreadyOut(QString barcode) {
    if (db->IsItemOut(barcode)) {
        //        speech->say("该快递已出库");
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
    filterString =
        QString("strftime('%Y-%m-%d %H:%M:%S', OutDate) > strftime('%Y-%m-%d %H:%M:%S', '%1') and IsTaken = 1").arg(
            localTimeNow);
    //        QString("strftime('%Y-%m-%d', OutDate) >= date('now','-1 month') and strftime('%Y-%m-%d', OutDate) <= date('now') and IsTaken = 1");
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

void InOutOperator::GetEarliestExpDate() {
    QString tempFilter = "";
    dayModel->setFilter(tempFilter);
    while (dayModel->canFetchMore()) {
        dayModel->fetchMore();
    }
    int counts = dayModel->rowCount();
    if (counts != 0) {
        QSqlRecord record = dayModel->record(counts - 1);
        m_earliestDate = record.value("Day").toString();
    } else {
        m_earliestDate = nullptr;
    }
}

int InOutOperator::getExpCountFromDateRange(QString start, QString end) {
    QString tempFilter = QString("Day >= '%1' and Day <= '%2' ").arg(
                             start, end);
    dayModel->setFilter(tempFilter);
    while (dayModel->canFetchMore()) {
        dayModel->fetchMore();
    }
    int count =  dayModel->rowCount();
    int sum = 0;
    for (int index = 0; index < count; ++index) {
        sum += dayModel->record(index).value("Count").toInt();
    }
    return sum;
}

int InOutOperator::getExpCountOfMonth(QString year) {
    QVector<QPointF> points;
    int max = 0;
    for (int mon = 1; mon <= 12; ++mon) {
        QString month;
        if (mon < 10) {
            month = QString("%1-0%2").arg(year, QString::number(mon));
        } else {
            month = QString("%1-%2").arg(year, QString::number(mon));
        }
        QString tempFilter = QString("Month = '%1'").arg(month);
        monthModel->setFilter(tempFilter);
        int count = monthModel->record(0).value("Count").toInt();

        if (count > max) {
            max = count;
        }
        points.append(QPointF(mon, count));
    }
    m_lineYear->replace(points);
    return (max % 10 == 0) && (max != 0) ? max : ((max / 10) + 1) * 10;
}

int InOutOperator::getExpCountOfDay(QString year, QString month) {
    QVector<QPointF> points;
    int max = 0; //max of axisY
    int monthDiff = month.toInt() - 1;
    for (int day = 0; day < 31; ++day) {
        QString tempFilter = QString("Day = date('%1-01-01', '+%2 month', '+%3 day')").arg(year,
                             QString::number(monthDiff), QString::number(day));
        dayModel->setFilter(tempFilter);
        int count = dayModel->record(0).value("Count").toInt();
        if (count > max) {
            max = count;
        }
        points.append(QPointF(day + 1, count));
    }
    m_lineMonth->replace(points);
    return (max % 10 == 0) && (max != 0) ? max : ((max / 10) + 1) * 10;
}

bool InOutOperator::deleteAndUpdate(int row) {
    if (deleteRow(row)) {
        db->SetFilter(filterString);
        emit updateDatabaseDone();
        return true;
    }
    return false;
}

bool InOutOperator::markUnOut(int row) {
    QSqlRecord record = recordModel->record(row);
    QString expOrderPhotoUrl = record.value(db->EXPORDERPHOTOURL).toString();
    QString clientPhotoUrl = record.value(db->CLIENTPHOTURL).toString();
    record.setValue(db->ISTAKEN, 0);
    record.setValue(db->OUTDATE, "");
    record.setValue(db->EXPORDERPHOTOURL, "");
    record.setValue(db->CLIENTPHOTURL, "");
    recordModel->setRecord(row, record);
    if (recordModel->submitAll()) {
        fileIo->DeleteFile(expOrderPhotoUrl);
        fileIo->DeleteFile(clientPhotoUrl);
        UpdateCount(false);
        GetMost();
        db->SetFilter(filterString);
        emit updateDatabaseDone();
        return true;
    }
    return false;
}

bool InOutOperator::deleteAll() {
    for (int i = 0; i < recordModel->rowCount(); ++i) {
        if (!deleteRow(i)) {
            return false;
        }
    }
    db->SetFilter(filterString);
    emit updateDatabaseDone();
    return true;
}

QStringList InOutOperator::yearList() {
    m_yearList.clear();
    int earliestYear;
    if (m_earliestDate == nullptr) {
        earliestYear = 2019;
    } else {
        earliestYear = QDate::fromString(m_earliestDate, "yyyy-MM-dd").year();
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
    if (filterString != "") {
        filterString.append(QString(" and IsTaken = 1"));
    }
    qDebug() << "Filter string is " << filterString;
}

void InOutOperator::CreateLines() {
    m_lineYear = new QLineSeries();
    m_lineMonth = new QLineSeries();
}

void InOutOperator::UpdateCount(bool isAdd) {
    QDate today = QDate::currentDate();
    QString year = today.toString("yyyy");
    QString month = today.toString("yyyy-MM");
    QString day = today.toString("yyyy-MM-dd");
    // update year count
    yearModel->setFilter(QString("Year = '%1'").arg(year));
    if (yearModel->rowCount() == 0) {
        QSqlRecord newRecord = yearModel->record();
        newRecord.setValue("Year", year);
        newRecord.setValue("Count", 1);
        yearModel->insertRecord(-1, newRecord);
        if (!yearModel->submitAll()) {
            qDebug() << "Insert year table error: " << yearModel->lastError().text();
            yearModel->revertAll();
        }
    } else {
        QSqlRecord record = yearModel->record(0);
        if (isAdd) {
            record.setValue("Count", record.value("Count").toInt() + 1);
        } else {
            record.setValue("Count", record.value("Count").toInt() - 1);
        }
        yearModel->setRecord(0, record);
        if (!yearModel->submitAll()) {
            qDebug() << "Update year table error: " << yearModel->lastError().text();
            yearModel->revertAll();
        }
    }
    // update month count
    monthModel->setFilter(QString("Month = '%1'").arg(month));
    if (monthModel->rowCount() == 0) {
        QSqlRecord newRecord = monthModel->record();
        newRecord.setValue("Month", month);
        newRecord.setValue("Count", 1);
        monthModel->insertRecord(-1, newRecord);
        if (!monthModel->submitAll()) {
            qDebug() << "Insert month table error: " << monthModel->lastError().text();
            monthModel->revertAll();
        }
    } else {
        QSqlRecord record = monthModel->record(0);
        if (isAdd) {
            record.setValue("Count", record.value("Count").toInt() + 1);
        } else {
            record.setValue("Count", record.value("Count").toInt() - 1);
        }
        monthModel->setRecord(0, record);
        if (!monthModel->submitAll()) {
            qDebug() << "Update month table error: " << monthModel->lastError().text();
            monthModel->revertAll();
        }
    }
    // update day count
    dayModel->setFilter(QString("Day = '%1'").arg(day));
    if (dayModel->rowCount() == 0) {
        QSqlRecord newRecord = dayModel->record();
        newRecord.setValue("Day", day);
        newRecord.setValue("Count", 1);
        dayModel->insertRecord(-1, newRecord);
        if (!dayModel->submitAll()) {
            qDebug() << "Insert day table error: " << dayModel->lastError().text();
            dayModel->revertAll();
        }
    } else {
        QSqlRecord record = dayModel->record(0);
        if (isAdd) {
            record.setValue("Count", record.value("Count").toInt() + 1);
        } else {
            record.setValue("Count", record.value("Count").toInt() - 1);
        }
        dayModel->setRecord(0, record);
        if (!dayModel->submitAll()) {
            qDebug() << "Update day table error: " << dayModel->lastError().text();
            dayModel->revertAll();
        }
    }
}

void InOutOperator::GetMost() {
    QString tempFilter = "";
    yearModel->setFilter(tempFilter);
    monthModel->setFilter(tempFilter);
    dayModel->setFilter(tempFilter);

    yearModel->setSort(1, Qt::SortOrder::DescendingOrder);
    yearModel->select();
    m_mostYear = yearModel->record(0).value("Year").toString();
    m_mostYearNumber = yearModel->record(0).value("Count").toString();
    yearModel->setSort(0, Qt::SortOrder::DescendingOrder);
    yearModel->select();
    emit mostYearChanged();
    emit mostYearNumberChanged();

    monthModel->setSort(1, Qt::SortOrder::DescendingOrder);
    monthModel->select();
    m_mostMonth = monthModel->record(0).value("Month").toString();
    m_mostMonthNumber = monthModel->record(0).value("Count").toString();
    monthModel->setSort(0, Qt::SortOrder::DescendingOrder);
    monthModel->select();
    emit mostMonthChanged();
    emit mostMonthNumberChanged();

    dayModel->setSort(1, Qt::SortOrder::DescendingOrder);
    dayModel->select();
    m_mostDay = dayModel->record(0).value("Day").toString();
    m_mostDayNumber = dayModel->record(0).value("Count").toString();
    dayModel->setSort(0, Qt::SortOrder::DescendingOrder);
    dayModel->select();
    emit mostDayChanged();
    emit mostDayNumberChanged();
}

bool InOutOperator::deleteRow(int row) {
    QString expOrderPhotoUrl = recordModel->record(row).value(db->EXPORDERPHOTOURL).toString();
    QString clientPhotoUrl = recordModel->record(row).value(db->CLIENTPHOTURL).toString();
    if (recordModel->removeRow(row)) {
        if (recordModel->submit()) {
            fileIo->DeleteFile(expOrderPhotoUrl);
            fileIo->DeleteFile(clientPhotoUrl);
            return true;
        }
    }
    qDebug() << "Delete row error : " << recordModel->lastError().text();
    return false;
}
