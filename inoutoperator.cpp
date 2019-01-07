#include "inoutoperator.h"
#include "dboperate.h"
#include <QDebug>
#include "config.h"

InOutOperator::InOutOperator(QObject *parent) : QObject(parent)
{
    db = DbOperate::getInstance();
    speech = Speech::getInstance();
}

bool InOutOperator::in(QString barcode, QString name, QString phone)
{
    if(barcode.isEmpty()) {
        speech->say(BARCODE_EMPTY);
        qDebug() << "Barcode cannot be empty.";
        return false;
    }
    if(db->IsItemExist(barcode)) {
        qDebug() << "Already has item " << barcode;
        speech->say(ITEM_EXIST);
        return false;
    }
    if(!db->InsertItem(barcode, name, phone)) {
        speech->say(IN_ERROR);
        return false;
    }
    return true;
}

bool InOutOperator::out(QString barcode, QString photoUrl)
{

}
