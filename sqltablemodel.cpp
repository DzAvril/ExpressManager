#include "sqltablemodel.h"


SqlTableModel::SqlTableModel(QObject *parent)
    : QSqlTableModel(parent) {

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
    for (int ix = 0; ix < ncols; ++ix) {
        roles.insert(Qt::UserRole + ix + 1,
                     QVariant(headerData(ix, Qt::Horizontal).toString())
                     .toByteArray());
    }
    return roles;
}
