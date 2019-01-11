#include "inoutoperator.h"
#include "dboperate.h"
#include <QDebug>
#include "config.h"
#include <QDateTime>

InOutOperator::InOutOperator(QObject *parent) : QObject(parent) {
    db = new DbOperate();
    speech = Speech::getInstance();
    fileIo = FileIo::getInstance();
    commonHelper = CommonHelper::getInstance();
    model = db->expTableModel();
    filterString.clear();
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
    return true;
}

bool InOutOperator::isItemAlreadyOut(QString barcode) {
    if (db->IsItemOut(barcode)) {
        speech->say(ALREADY_EXIST);
        return true;
    }
    return false;
}

void InOutOperator::setBarcodeFilter(QString barcode)
{
    if(barcode == "") {
        filterBarcode.clear();
    } else {
        filterBarcode = barcode;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setOutDateFilter(QString outdate)
{
    if(outdate == "") {
        filterOutDate.clear();
    } else {
        filterOutDate = outdate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setStartOutDateFilter(QString startOutDate)
{
    if(startOutDate == "") {
        filterStartOutDate.clear();
    } else {
        filterStartOutDate = startOutDate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::setEndOutDateFilter(QString endOutDate)
{
    if(endOutDate == "") {
        filterEndOutDate.clear();
    } else {
        filterEndOutDate = endOutDate;
    }
    PackFilterFrame();
    db->SetFilter(filterString);
}

void InOutOperator::resetFilter()
{
    filterString.clear();
    filterBarcode.clear();
    filterOutDate.clear();
    filterStartOutDate.clear();
    filterEndOutDate.clear();
    db->SetFilter(filterString);
}

DbOperate *InOutOperator::expDb() const {
    return db;
}

void InOutOperator::PackFilterFrame()
{
    filterString.clear();
    if(!filterBarcode.isEmpty()) {
        filterString += QString("Barcode like '%1%'").arg(filterBarcode);
        if(!filterOutDate.isEmpty()) {
            filterString += " and ";
            filterString += QString("todo");
            if((!filterStartOutDate.isEmpty()) && (!filterEndOutDate.isEmpty())) {
                filterString += " and ";
                filterString += QString("todo");
            }
        } else {
            if((!filterStartOutDate.isEmpty()) && (!filterEndOutDate.isEmpty())) {
                filterString += QString("todo");
            }
        }
    } else {
        if(!filterOutDate.isEmpty()) {
            filterString += QString("todo");
            if((!filterStartOutDate.isEmpty()) && (!filterEndOutDate.isEmpty())) {
                filterString += " and ";
                filterString += QString("todo");
            }
        } else {
            if((!filterStartOutDate.isEmpty()) && (!filterEndOutDate.isEmpty())) {
                filterString += QString("todo");
            }
        }
    }
    qDebug() << "Filter string is " << filterString;
}
