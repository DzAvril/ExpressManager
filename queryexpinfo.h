#ifndef QUERYEXPINFO_H
#define QUERYEXPINFO_H

#include <QObject>

typedef struct {
    QString senderName;
    QString senderPhone;
    //todo

} EXP_INFO_t;

class QueryExpInfo : public QObject
{
    Q_OBJECT
public:
    explicit QueryExpInfo(QObject *parent = nullptr);
//    Q_INVOKABLE bool QueryByBarcode(QString barcode);
    Q_PROPERTY(QString QsenderName READ QsenderName WRITE setQsenderName NOTIFY senderNameChanged)
    Q_PROPERTY(QString QsenderPhone READ QsenderPhone WRITE setQsenderPhone NOTIFY senderPhoneChanged)

    QString senderName, senderPhone;
    QString QsenderName();
    QString QsenderPhone();
    void setQsenderName(QString);
    void setQsenderPhone(QString);
private:

signals:
    void senderNameChanged();
    void senderPhoneChanged();

public slots:
    bool queryByBarcode(QString barcode);
};

#endif // QUERYEXPINFO_H
