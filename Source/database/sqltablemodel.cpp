#include "sqltablemodel.h"
#include <QtDebug>

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db) {

}

QVariant SqlTableModel::data(const QModelIndex &idx, int role) const {
    if (idx.row() >= rowCount()) {
        return QString();
    }
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(idx, role);
    }
    QModelIndex modelIndex = index(idx.row(), role - Qt::UserRole - 1);
    return QSqlTableModel::data(modelIndex, Qt::EditRole);
}

QHash<int, QByteArray> SqlTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    const int ncols = columnCount();
    qDebug() << "Column numbers is " << ncols;
    for (int ix = 0; ix < ncols; ++ix) {
        roles.insert(Qt::UserRole + ix + 1,
                     QVariant(headerData(ix, Qt::Horizontal).toString())
                     .toByteArray());
        qDebug() << "roles: " << QVariant(headerData(ix, Qt::Horizontal).toString());
    }
    return roles;
}
