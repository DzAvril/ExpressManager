#ifndef SQLDATABASEHANDLER_H
#define SQLDATABASEHANDLER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include "sqltable.h"
#include "sqlrecord.h"

class SqlDatabaseHandler : public QObject {
    Q_OBJECT
  public:
    SqlDatabaseHandler(const QString &dbname, QObject *parent = nullptr);
    QString databaseName() const;
    void addTable(const SqlTable &table);

    template<typename ConfigurationFunction>
    void start(bool *dbStarted, QString *errorString,
               ConfigurationFunction configurationFunction) {
        configurationFunction(this);
        if (dbStarted) {
            *dbStarted = configure(errorString);
            if (*dbStarted) {
                emit started();
            }
        } else {
            if (configure(errorString)) {
                emit started();
            }
        }
    }
  signals:
    void started();
    void inserted();
    void updated();
    void deleted();
    void transactionDone();
  protected:
    QSqlDatabase m_db;
    QList<SqlTable> m_tables;

    void setDatabaseName(const QString &dbname);
    bool createAllTables(QString *errorString = nullptr);
    bool configure(QString *errorString = nullptr);
    bool insert(const QString &table,
                const QList<SqlRecord> &records,
                QString *errorString = nullptr);
};

#endif // SQLDATABASEHANDLER_H
