#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QObject>

class CommonHelper : public QObject
{
    Q_OBJECT
public:
    static CommonHelper *getInstance();
    Q_INVOKABLE void delay(int n);
private:
    explicit CommonHelper(QObject *parent = nullptr);
    CommonHelper(const CommonHelper&);
    CommonHelper &operator=(const CommonHelper &);
    static CommonHelper *instance;
signals:

public slots:
};

#endif // COMMONHELPER_H
