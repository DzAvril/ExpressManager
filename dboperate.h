#ifndef DBOPERATE_H
#define DBOPERATE_H
#include "config.h"
#include <QObject>
#include <QSqlDatabase>
#include "speech.h"
#include "fileio.h"

class DbOperate : public QObject
{
    Q_OBJECT
public:
    explicit DbOperate(QObject *parent = nullptr);
    bool OpenDB(const QString &path);
    Q_INVOKABLE bool insertItem(QString barcode, QString name = nullptr,
                                QString phone = nullptr);
    Q_INVOKABLE bool updateItemPhotoUrl(QString barcode, QString photoUrl);
private:
    QSqlDatabase db;
    bool IsRecordTableExist();
    bool CreateRecordTable();
    int GetItemsCount();
    Speech *speech;
    bool IsItemExist(QString &barcode);
    FileIo *fileIo;
signals:

public slots:
};

#endif // DBOPERATE_H
