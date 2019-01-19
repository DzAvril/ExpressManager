#include "fileio.h"
#include <QDir>
#include <QDebug>
#include <QApplication>
#include "config.h"
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>
FileIo::FileIo(QObject *parent) : QObject(parent) {

}

FileIo::FileIo(const FileIo &) {

}

//FileIo &FileIo::operator=(const FileIo &) {

//}

FileIo *FileIo::instance = new FileIo();
bool FileIo::IsPathExist(const QString &path) {
    //    qDebug() << "Check is path " << path << "exist";
    QDir dir(path);
    if (dir.exists()) {
        return true;
    }
    return false;
}

bool FileIo::CreateDir(const QString &path) {
    qDebug() << "Create path " << path;
    bool result = true;
    QDir dir(path);
    if (dir.exists()) {
        return result;
    } else {
        result = dir.mkdir(path);
    }
    return result;
}


FileIo *FileIo::getInstance() {
    return instance;
}

const QString FileIo::getPhotoPath() {
    return QGuiApplication::applicationDirPath() + PHOTO_PATH;
}

bool FileIo::DeleteFile(const QString &path) {
    QFile file(path);
    return file.remove();
}

const QString FileIo::GetCurrentPath() {
    return QGuiApplication::applicationDirPath();
}

void FileIo::revealInExplore(const QString filePath) {
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer", args);
}
