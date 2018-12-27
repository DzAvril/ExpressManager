#ifndef Startup_H
#define Startup_H
#include "dboperate.h"
#include <QObject>

class Startup : public QObject
{
    Q_OBJECT
public:
    explicit Startup(QObject *parent = nullptr);
    void SetObject(QObject *obj);
    void SystemUp();

private:
    QObject *object;
    DbOperate *dbOp;

signals:

public slots:
};

#endif // Startup_H
