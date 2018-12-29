#include "commonhelper.h"
#include <QTime>
#include <QCoreApplication>

CommonHelper::CommonHelper(QObject *parent) : QObject(parent)
{

}

CommonHelper::CommonHelper(const CommonHelper &)
{

}

CommonHelper &CommonHelper::operator=(const CommonHelper &)
{

}

CommonHelper *CommonHelper::instance = new CommonHelper();
CommonHelper *CommonHelper::getInstance() {
    return instance;
}

void CommonHelper::delay(int n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
