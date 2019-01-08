#ifndef SQLRECORD_H
#define SQLRECORD_H

#include <QString>
#include <QVariant>

class SqlRecord {
  public:
    SqlRecord();
    void setField(const QString &field);
    void setValue(const QVariant &value);
    QString field() const;
    QVariant value() const;
  private:
    QString m_field;
    QVariant m_value;
};

#endif // SQLRECORD_H
