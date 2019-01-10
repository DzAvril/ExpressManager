#ifndef PERSONDATABAHANDLER_H
#define PERSONDATABAHANDLER_H

#include "sqldatabasehandler.h"
#include "sqltablemodel.h"
#include "sqlrecord.h"
#include <QList>

class PersonDatabaHandler : public SqlDatabaseHandler {
  public:
    PersonDatabaHandler(const QString &dbname, QObject *parent = nullptr);
    SqlTableModel *personTableModel() const;
  private:
    SqlTableModel *m_personTableModel;
    QList<QList<SqlRecord>> m_personRows;
    bool rowExists(const QList<SqlRecord> &row) const;
};

#endif // PERSONDATABAHANDLER_H
