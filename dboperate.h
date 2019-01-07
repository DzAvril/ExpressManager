#ifndef DBOPERATE_H
#define DBOPERATE_H
#include <QObject>
#include <QSqlDatabase>
#include "fileio.h"

class DbOperate : public QObject {
    Q_OBJECT
  public:
    explicit DbOperate(QObject *parent = nullptr);
    bool OpenDB(const QString &path);
    bool InsertItem(const QString &barcode, QString &name ,QString &phone);
    bool UpdateClientPhotoUrl(const QString &barcode, QString &photoUrl);
    bool UpdateIsTaken(const QString &barcode, int isTaken);
    bool UpdateOutDate(const QString &barcode);
    bool IsItemExist(const QString &barcode);
    bool IsItemOut(const QString &barcode);
  private:
    QSqlDatabase db;
    bool IsRecordTableExist();
    bool CreateRecordTable();
    int GetItemsCount();
    FileIo *fileIo;
  signals:

  public slots:
};

#endif // DBOPERATE_H
