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

DbOperate *InOutOperator::expDb() const {
    return db;
}
