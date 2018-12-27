#include "queryexpinfo.h"
#include <iostream>

QueryExpInfo::QueryExpInfo(QObject *parent) : QObject(parent)
{
    senderName = "NULL";
    senderPhone = "NULL";
}

bool QueryExpInfo::queryByBarcode(QString barcode)
{
//    printf("Start query express info of %s.\n", barcode.toStdString().c_str());
    std::cout << "Start query express info of " << barcode.toStdString() << std::endl;
    setQsenderName("xuzhi");
    setQsenderPhone("162172");
    return true;
}

QString QueryExpInfo::QsenderName()
{
    return senderName;
}

QString QueryExpInfo::QsenderPhone()
{
    return senderPhone;
}

void QueryExpInfo::setQsenderName(QString value)
{
    senderName = value;
    emit senderNameChanged();
}

void QueryExpInfo::setQsenderPhone(QString value)
{
    senderPhone = value;
    emit senderPhoneChanged();
}
