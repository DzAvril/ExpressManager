#include "persondatabahandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PersonDatabaHandler::PersonDatabaHandler(const QString &dbname, QObject *parent)
    : SqlDatabaseHandler{dbname, parent} {
    QList<SqlRecord> row1;
    QList<SqlRecord> row2;
    QList<SqlRecord> row3;
    QList<SqlRecord> row4;
    QList<SqlRecord> row5;

    row1.append(SqlRecord::makeRecord("age", 23));
    row1.append(SqlRecord::makeRecord("name", "Ana"));

    row2.append(SqlRecord::makeRecord("name", "Pedro"));
    //        row2.append(SqlRecord::makeRecord("age", 22));

    row3.append(SqlRecord::makeRecord("name", "Juan"));
    //        row3.append(SqlRecord::makeRecord("age", 25));

    row4.append(SqlRecord::makeRecord("name", "Luis"));
    //        row4.append(SqlRecord::makeRecord("age", 18));

    row5.append(SqlRecord::makeRecord("name", "Lisa"));
    //        row5.append(SqlRecord::makeRecord("age", 11));

    m_personRows.append(row1);
    m_personRows.append(row2);
    m_personRows.append(row3);
    m_personRows.append(row4);
    m_personRows.append(row5);

    m_personTableModel = new OutExpModel(this);
    connect(this, &PersonDatabaHandler::started, this, [&]() {
        //insert values into person table
        const int nPersonRows = m_personRows.size();
        for (int ix = 0; ix < nPersonRows; ++ix) {
            if (!rowExists(m_personRows.at(ix))) {
                QString err;
                qDebug() << insert("person", m_personRows.at(ix), &err);
                qDebug() << err;
            }
        }
        m_personTableModel->setTable("person");
        m_personTableModel->select();
    });
}

OutExpModel *PersonDatabaHandler::personTableModel() const {
    return m_personTableModel;
}

bool PersonDatabaHandler::rowExists(const QList<SqlRecord> &row) const {
    QSqlQuery query;
    if (query.prepare("select id from person where name like :name and age = :age")) {
        query.bindValue(":name", row.at(0).value());
        query.bindValue(":age", row.at(1).value());
        if (query.exec()) {
            return query.next();
        }
    }
    return false;
}
