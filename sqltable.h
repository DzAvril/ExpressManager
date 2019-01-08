#ifndef SQLTABLE_H
#define SQLTABLE_H

#include <QList>
#include <QString>
#include "sqlfield.h"
#include "sqlrecord.h"

class SqlTable {
  public:
    SqlTable();
    QString sql() const;
    void setName(const QString &name);
    QString name() const;
    bool addField(const SqlField &field, QString *errorString = nullptr);
    bool createIfNotExists(QString *errorString = nullptr) const;
    bool insert(const QList<SqlRecord> &records, QString *errorString = nullptr) const;
  private:
    QString m_name;
    QList<SqlField> m_fields;
};

#endif // SQLTABLE_H
