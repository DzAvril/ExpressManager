#include "dboperate.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <stdio.h>
#include <string>
#include <QGuiApplication>

using namespace std;
DbOperate::DbOperate(QObject *parent) : QObject(parent)
{
    fileIo = FileIo::getInstance();
    const QString path = QGuiApplication::applicationDirPath() + DB_PATH;
    if (!fileIo->IsPathExist(path)) {
        fileIo->CreateDir(path);
    }
    if (OpenDB(path + DB_NAME)) {
        qDebug("Open database successed.");
        if (!IsRecordTableExist()) {
            qDebug("Table record is not exist, creat now.");
            CreateRecordTable();
        } else {
            qDebug("Table record is exist.");
        }
    }
}

DbOperate::DbOperate(const DbOperate &)
{

}

DbOperate &DbOperate::operator=(const DbOperate &)
{

}

DbOperate *DbOperate::instance = new DbOperate();
DbOperate *DbOperate::getInstance()
{   
    return instance;
}

bool DbOperate::OpenDB(const QString &path)
{
    qDebug( )  << "QSqlDatabase :: Drivers ()==" << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE", "project");
    db.setDatabaseName(path);
    db = QSqlDatabase::database("project");

    if (!db.open()) {
        qDebug() << "Open Database Error:" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::IsRecordTableExist()
{
    QSqlQuery query(db);
    int tempIdx = 0;
    query.exec("select count(*)  from sqlite_master where type='table' and name = 'record'")
    ;

    if (query.next()) {
        tempIdx =  query.value(0).toInt();
    }

    return tempIdx ? true : false;
}

bool DbOperate::CreateRecordTable()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE record ("
               "Barcode char(30) PRIMARY KEY, "
               "Name char(20), "
               "Phone char(10),"
               "InDate char(50),"
               "OutDate char(50),"
               "IsTaken int(1) NOT NULL,"
               "ExpOrderPhotoUrl char(100)),"
               "ClientPhotoUrl char(100))");

    if (!query.isActive()) {
        qDebug() << "Create Database Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::InsertItem(QString &barcode, QString &name, QString &phone)
{
    qDebug() << "Start insert item " << barcode << "to db.";
    QSqlQuery query(db);

    query.prepare("INSERT INTO record (Barcode, Name, Phone, InDate)"
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(barcode);
    query.addBindValue(name) ;
    query.addBindValue(phone);
    QDateTime timestamp = QDateTime::currentDateTime();
    query.addBindValue(timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();

    if (!query.isActive()) {
        qDebug() << "Insert Database Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::UpdateItemPhotoUrl(QString &barcode, QString &photoUrl)
{
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("UPDATE record SET PhotoUrl ='%s'WHERE Barcode='%s'",
                    photoUrl.toStdString().c_str(),
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Update photo url Error" << db.lastError().text();
        fileIo->DeleteFile(photoUrl);
        return false;
    }
    return true;
}

int DbOperate::GetItemsCount()
{
    QSqlQuery query(db);
    int tempIdx = 0;
    query.exec("SELECT COUNT(*) FROM record");

    if (query.next()) {
        tempIdx =  query.value(0).toInt();
    }

    return tempIdx;
}

bool DbOperate::IsItemExist(QString &barcode)
{
    QSqlQuery query(db);
    QString strTemp;
    int tempCount = 0;
    strTemp.sprintf("select count(*) from record where Barcode='%s'",
                    barcode.toStdString().c_str());
    query.prepare(strTemp);
    query.exec();
    if (query.next()) {
        tempCount =  query.value(0).toInt();
    } else {
        qDebug() << "Query Database Error" << db.lastError().text();
    }
    if (tempCount > 0) {
        return true;
    } else {
        return false;
    }
}

