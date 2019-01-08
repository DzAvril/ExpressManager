#include "sqldatabasehandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QUrl>

SqlDatabaseHandler::SqlDatabaseHandler(const QString &dbname, QObject *parent)
    : QObject(parent) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_tables.clear();
    m_db.setDatabaseName(dbname);

    connect(this, &SqlDatabaseHandler::inserted, this, [&]() {
        emit transactionDone();
    });
    connect(this, &SqlDatabaseHandler::updated, this, [&]() {
        emit transactionDone();
    });
    connect(this, &SqlDatabaseHandler::deleted, this, [&]() {
        emit transactionDone();
    });
}

QString SqlDatabaseHandler::databaseName() const {
    return m_db.databaseName();
}

void SqlDatabaseHandler::addTable(const SqlTable &table) {
    const int nTables = m_tables.size();
    for (int ix = 0; ix < nTables; ++ix) {
        if (m_tables.at(ix).name() == table.name()) {
            return;
        }
    }
    m_tables.append(table);
}

void SqlDatabaseHandler::setDatabaseName(const QString &dbname) {
    m_db.setDatabaseName(dbname);
}

bool SqlDatabaseHandler::createAllTables(QString *errorString) {
    const int nTables = m_tables.size();
    for (int ix = 0; ix < nTables; ++ix) {
        if (!m_tables.at(ix).createIfNotExists(errorString)) {
            return false;
        }
    }
    return true;
}

bool SqlDatabaseHandler::configure(QString *errorString) {
    if (m_db.open()) {
        if (createAllTables(errorString)) {
            return true;
        } else {
            if (errorString) {
                *errorString = m_db.lastError().text();
            }
        }
    } else {
        if (errorString) {
            *errorString = m_db.lastError().text();
        }
    }
    return false;
}

bool SqlDatabaseHandler::insert(const QString &table, const QList<SqlRecord> &records, QString *errorString) {
    int idx = -1;
    const int nTables = m_tables.size();
    for (int ix = 0; ix < nTables; ++ix) {
        if (m_tables.at(ix).name() == table) {
            idx = ix;
            break;
        }
    }
    if (idx != -1) {
        if (m_tables.at(idx).insert(records, errorString)) {
            emit inserted();
            return true;
        }
    }
}
