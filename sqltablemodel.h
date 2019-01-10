#ifndef OUTEXPMODEL_H
#define OUTEXPMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QHash>
#include <QVariant>
#include <QByteArray>
#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel {
    Q_OBJECT
  public:
    SqlTableModel(QObject *parent = nullptr);
  protected:
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
  private:

};

#endif // OUTEXPMODEL_H
