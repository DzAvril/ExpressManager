#ifndef DBOPERATE_H
#define DBOPERATE_H
#include <QObject>
#include <QSqlDatabase>
#include "fileio.h"

class DbOperate : public QObject {
    Q_OBJECT
  public:
    bool OpenDB(const QString &path);
    Q_INVOKABLE bool InsertItem(QString &barcode, QString &name ,QString &phone);
    Q_INVOKABLE bool UpdateItemPhotoUrl(QString &barcode, QString &photoUrl);
    static DbOperate *getInstance();
    bool IsItemExist(QString &barcode);
  private:
    explicit DbOperate(QObject *parent = nullptr);
    DbOperate(const DbOperate&);
    DbOperate &operator=(const DbOperate &);
    static DbOperate *instance;
    QSqlDatabase db;
    bool IsRecordTableExist();
    bool CreateRecordTable();
    int GetItemsCount();
    FileIo *fileIo;
  signals:

  public slots:
};

#endif // DBOPERATE_H
