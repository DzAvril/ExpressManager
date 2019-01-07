#ifndef INOUTOPERATOR_H
#define INOUTOPERATOR_H

#include <QObject>
#include "dboperate.h"
#include "speech.h"

class InOutOperator : public QObject
{
    Q_OBJECT
public:
    explicit InOutOperator(QObject *parent = nullptr);
    Q_INVOKABLE bool in(QString barcode, QString name = nullptr,
            QString phone = nullptr);
    Q_INVOKABLE bool out(QString barcode, QString photoUrl);
private:
    DbOperate *db;
    Speech* speech;

signals:

public slots:
};

#endif // INOUTOPERATOR_H
