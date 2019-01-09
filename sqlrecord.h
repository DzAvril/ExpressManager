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
    static SqlRecord makeRecord(const QString &field, const QVariant &value);
  private:
    QString m_field;
    QVariant m_value;
};

#endif // SQLRECORD_H
