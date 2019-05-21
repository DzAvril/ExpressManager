#include "dboperate.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <stdio.h>
#include <string>
#include <QGuiApplication>
#include "config.h"
#include "QDateTime"

using namespace std;
DbOperate::DbOperate(QObject *parent) : QObject(parent) {
    fileIo = FileIo::getInstance();
    const QString path = QGuiApplication::applicationDirPath() + DB_PATH;
    if (!fileIo->IsPathExist(path)) {
        fileIo->CreateDir(path);
    }
    if (OpenDB(path + DB_NAME)) {
        qDebug("Open database successed.");
        if (!IsTableExist("record")) {
            qDebug("Table record is not exist, creat now.");
            CreateRecordTable();
        } else {
            //            qDebug("Table record is exist.");
        }
        if (!IsTableExist("year")) {
            qDebug("Table year is not exist, creat now.");
            CreateYearTable();
        } else {
            //            qDebug("Table year is exist.");
        }
        if (!IsTableExist("month")) {
            qDebug("Table month is not exist, creat now.");
            CreateMonthTable();
        } else {
            //            qDebug("Table month is exist.");
        }
        if (!IsTableExist("day")) {
            qDebug("Table day is not exist, creat now.");
            CreateDayTable();
        } else {
            //            qDebug("Table day is exist.");
        }
        m_recordModel = new SqlTableModel(this, db);
        m_recordModel->setTable("record");
        m_recordModel->setSort(OUTDATE, Qt::SortOrder::DescendingOrder);
        m_yearModel = new SqlTableModel(this, db);
        m_yearModel->setTable("year");
        m_yearModel->setSort(0, Qt::SortOrder::DescendingOrder);
        m_monthModel = new SqlTableModel(this, db);
        m_monthModel->setTable("month");
        m_monthModel->setSort(0, Qt::SortOrder::DescendingOrder);
        m_dayModel = new SqlTableModel(this, db);
        m_dayModel->setTable("day");
        m_dayModel->setSort(0, Qt::SortOrder::DescendingOrder);
        RefreshModel();
    }
}

DbOperate::~DbOperate() {
    delete m_recordModel;
    delete m_yearModel;
    delete m_monthModel;
    delete m_dayModel;
}

bool DbOperate::OpenDB(const QString &path) {
    //    qDebug()  << "QSqlDatabase :: Drivers ()==" << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE", "project");
    db.setDatabaseName(path);
    db = QSqlDatabase::database("project");

    if (!db.open()) {
        qDebug() << "Open Database Error:" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::IsTableExist(QString tableName) {
    QSqlQuery query(db);
    int tempIdx = 0;
    query.exec(QString("select count(*)  from sqlite_master where type='table' and name = '%1'").arg(tableName));

    if (query.next()) {
        tempIdx =  query.value(0).toInt();
    }

    return tempIdx ? true : false;
}

bool DbOperate::CreateRecordTable() {
    QSqlQuery query(db);
    query.exec("CREATE TABLE record ("
               "Barcode char(30) PRIMARY KEY, "
               "Name char(20), "
               "Phone char(10),"
               "InDate char(50),"
               "OutDate char(50),"
               "IsTaken int(1) NOT NULL,"
               "ExpOrderPhotoUrl char(100),"
               "ClientPhotoUrl char(100))");

    if (!query.isActive()) {
        qDebug() << "Create table record Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::CreateYearTable() {
    QSqlQuery query(db);
    query.exec("CREATE TABLE year ("
               "Year TEXT PRIMARY KEY, "
               "Count int NOT NULL)");

    if (!query.isActive()) {
        qDebug() << "Create table year Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::CreateMonthTable() {
    QSqlQuery query(db);
    query.exec("CREATE TABLE month ("
               "Month TEXT PRIMARY KEY, "
               "Count int NOT NULL)");

    if (!query.isActive()) {
        qDebug() << "Create table month Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::CreateDayTable() {
    QSqlQuery query(db);
    query.exec("CREATE TABLE day ("
               "Day TEXT PRIMARY KEY, "
               "Count int NOT NULL)");

    if (!query.isActive()) {
        qDebug() << "Create table day Error" << db.lastError().text();
        return false;
    }
    return true;
}

bool DbOperate::InsertRecord(const QString &barcode, QString &name, QString &phone) {
    qDebug() << "Start insert item " << barcode << "to db.";
    QSqlQuery query(db);

    query.prepare("INSERT INTO record (Barcode, Name, Phone, InDate, IsTaken)"
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(barcode);
    query.addBindValue(name) ;
    query.addBindValue(phone);
    QDateTime timestamp = QDateTime::currentDateTime();
    query.addBindValue(timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(0);
    query.exec();

    if (!query.isActive()) {
        qDebug() << "Insert Database Error" << db.lastError().text();
        return false;
    }
    RefreshModel();
    return true;
}

bool DbOperate::UpdateClientPhotoUrl(const QString &barcode, QString &clientPhotoUrl) {
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("UPDATE record SET ClientPhotoUrl ='%s'WHERE Barcode='%s'",
                    clientPhotoUrl.toStdString().c_str(),
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Update photo url Error" << db.lastError().text();
        return false;
    }
    RefreshModel();
    return true;
}

bool DbOperate::UpdateOrderPhotoUrl(const QString &barcode, QString &photoUrl) {
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("UPDATE record SET ExpOrderPhotoUrl ='%s'WHERE Barcode='%s'",
                    photoUrl.toStdString().c_str(),
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Update photo url Error" << db.lastError().text();
        return false;
    }
    RefreshModel();
    return true;
}

bool DbOperate::UpdateIsTaken(const QString &barcode, int isTaken) {
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("UPDATE record SET IsTaken ='%d'WHERE Barcode='%s'",
                    isTaken,
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Update IsTaken Error" << db.lastError().text();
        return false;
    }
    RefreshModel();
    return true;
}

bool DbOperate::UpdateOutDate(const QString &barcode, const QString &outDate) {
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("UPDATE record SET OutDate ='%s'WHERE Barcode='%s'",
                    outDate.toStdString().c_str(),
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Update OutDate Error" << db.lastError().text();
        return false;
    }
    RefreshModel();
    return true;
}

bool DbOperate::IsItemOut(const QString &barcode) {
    QSqlQuery query(db);
    QString strTemp;
    strTemp.sprintf("SELECT IsTaken FROM record WHERE Barcode='%s'",
                    barcode.toStdString().c_str());
    query.exec(strTemp);
    if (!query.isActive()) {
        qDebug() << "Get is item out Error" << db.lastError().text();
        return false;
    }
    if (query.first()) {
        return query.value(0) == 1 ? true : false;
    }
    return false;
}

int DbOperate::GetItemsCount() {
    QSqlQuery query(db);
    int tempIdx = 0;
    query.exec("SELECT COUNT(*) FROM record");

    if (query.next()) {
        tempIdx =  query.value(0).toInt();
    }

    return tempIdx;
}

void DbOperate::RefreshModel() {
    if (m_recordModel) {
        m_recordModel->select();
    }
    if (m_yearModel) {
        m_yearModel->select();
    }
    if (m_monthModel) {
        m_monthModel->select();
    }
    if (m_dayModel) {
        m_dayModel->select();
    }
}

void DbOperate::SetFilter(QString &filterStr) {
    if (m_recordModel) {
        m_recordModel->setFilter(filterStr);
        qDebug() << "filter string is " << m_recordModel->filter();
        m_recordModel->select();
    }
}

bool DbOperate::IsItemExist(const QString &barcode) {
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

