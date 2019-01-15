#include "inoutoperator.h"
#include "dboperate.h"
#include <QDebug>
#include "config.h"
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlError>

InOutOperator::InOutOperator(QObject *parent) : QObject(parent) {
    db = new DbOperate();
    speech = Speech::getInstance();
    fileIo = FileIo::getInstance();
    commonHelper = CommonHelper::getInstance();
    model = db->expTableModel();
    filterString =
        QString("strftime('%Y-%m-%d', OutDate) >= date('now','-1 month') and strftime('%Y-%m-%d', OutDate) <= date('now')");
    db->SetFilter(filterString);
    filterBarcode.clear();
    filterOutDate.clear();
    filterStartOutDate.clear();
    filterEndOutDate.clear();
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
    filterString =
        QString("strftime('%Y-%m-%d', OutDate) >= date('now','-1 month') and strftime('%Y-%m-%d', OutDate) <= date('now')");
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

QList<QMap<int, int>> InOutOperator::getExpCountOfMonth(QString month)
{
    QList<QMap<int, int>> data;
    QMap<int, int> item, item1, item2;
    item[0] = 2;
    item[1] = 5;
    item1[0] = 4;
    item1[1] = 10;
    item2[0] = 6;
    item2[1] = 15;
    data.append(item);
    data.append(item1);
    data.append(item2);
    return data;
}

int InOutOperator::getExpCountOfDay(QString day)
{

}

QString InOutOperator::get(int row, int role) const
{
    return model->record(row).value(role).toString();
}

bool InOutOperator::deleteRow(int row)
{
    if(model->removeRow(row)) {
        if(model->submit()) {
            emit updateDatabaseDone();
            return true;
        }
    }
    qDebug() << "Delete row error : " << model->lastError().text();
    return false;
}

QStringList InOutOperator::yearList()
{
    int earliestYear = QDate::fromString(getEarliestExpDate(), "yyyy-MM-dd").year();
    int thisYear = QDate::currentDate().year();
    for(int year = earliestYear; year <= thisYear; ++year) {
        m_yearList.append(QString::number(year));
    }
    return m_yearList;
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
