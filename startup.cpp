#include "startup.h"
#include "config.h"
#include <QDebug>
#include <QGuiApplication>

Startup::Startup(QObject *parent) : QObject(parent)
{
}

void Startup::SetObject(QObject *obj)
{
    object = obj;
}

void Startup::SystemUp()
{
    FileIo *fileIo = FileIo::getInstance();
    QString path = QGuiApplication::applicationDirPath() + DATA_PATH;
    if (!fileIo->IsPathExist(path)) {
        fileIo->CreateDir(path);
    }
    path = QGuiApplication::applicationDirPath() + PHOTO_PATH;
    if (!fileIo->IsPathExist(path)) {
        fileIo->CreateDir(path);
    }
    path = QGuiApplication::applicationDirPath() + TEMP_PATH;
    if (!fileIo->IsPathExist(path)) {
        fileIo->CreateDir(path);
    }

}
