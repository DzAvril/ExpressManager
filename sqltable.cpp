#include "sqltable.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SqlTable::SqlTable() {
    m_name = "";
    m_fields.clear();
}

QString SqlTable::sql() const {
    QString sqlString;
    sqlString += m_name;
    sqlString += "(";
    const int nFields = m_fields.size();
    if (nFields > 0) {
        sqlString += m_fields.at(0).sql();
        for (int ix = 1; ix < nFields; ++ix) {
            sqlString += ", " + m_fields.at(ix).sql();
        }
    }
    sqlString += ")";
    return sqlString;
}

void SqlTable::setName(const QString &name) {
    m_name = name;
}

QString SqlTable::name() const {
    return  m_name;
}

bool SqlTable::addField(const SqlField &field, QString *errorString) {
    const int nFields = m_fields.size();
    for (int ix = 0; ix < nFields; ++ ix) {
        if (m_fields.at(ix).name() == field.name()) {
            if (errorString) {
                *errorString = "names conflict";
            }
            return false;
        }
        if (m_fields.at(ix).constraints() & SqlField::PRIMARY_KEY) {
            if (field.constraints() & SqlField::PRIMARY_KEY) {
                if (errorString) {
                    *errorString = "primary keys conflict";
                }
                return false;
            }
        }
    }
    m_fields.append(field);
    return true;
}

bool SqlTable::createIfNotExists(QString *errorString) const {
    QSqlQuery query;
    if (query.prepare((QString("create table if not exists %1").arg(sql())))) {
        query.bindValue(":table", m_name);
        if (query.exec()) {
            return true;
        }
    } else {
        if (errorString) {
            *errorString = query.lastError().text();
        }
    }
    return false;
}

bool SqlTable::insert(const QList<SqlRecord> &records, QString *errorString) const {
    const int nRecords = records.size();
    if (nRecords > 0) {
        QString queryString = QString("insert into %1 (").arg(m_name);
        queryString += records.at(0).field();
        for (int ix = 1; ix < nRecords; ++ix) {
            queryString += QString(", %1").arg(records.at(ix).field());
        }
        queryString += ") values (";
        queryString += QString(":%1").arg(records.at(0).field());
        for (int ix = 1; ix < nRecords; ++ix) {
            queryString += QString(", :%1").arg(records.at(ix).field());
        }
        queryString += ")";
        QSqlQuery query;
        if (query.prepare(queryString)) {
            for (int ix = 0; ix < nRecords; ++ix) {
                query.bindValue(QString(":%1").arg(records.at(ix).field()), records.at(ix).value());
            }
            if (query.exec()) {
                return true;
            } else {
                if (errorString) {
                    *errorString = query.lastError().text();
                }
                return false;
            }
        } else {
            if (errorString) {
                *errorString = query.lastError().text();
            }
            return false;
        }
    }
    return false;
}
