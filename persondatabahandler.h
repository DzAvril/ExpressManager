#ifndef PERSONDATABAHANDLER_H
#define PERSONDATABAHANDLER_H

#include "sqldatabasehandler.h"
#include "outexpmodel.h"
#include "sqlrecord.h"
#include <QList>

class PersonDatabaHandler : public SqlDatabaseHandler {
  public:
    PersonDatabaHandler(const QString &dbname, QObject *parent = nullptr);
    OutExpModel *personTableModel() const;
  private:
    OutExpModel *m_personTableModel;
    QList<QList<SqlRecord>> m_personRows;
    bool rowExists(const QList<SqlRecord> &row) const;
};

#endif // PERSONDATABAHANDLER_H
