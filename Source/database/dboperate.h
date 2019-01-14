#ifndef DBOPERATE_H
#define DBOPERATE_H
#include <QObject>
#include <QSqlDatabase>
#include "fileio.h"
#include "sqltablemodel.h"

class DbOperate : public QObject {
    Q_OBJECT
  public:
    explicit DbOperate(QObject *parent = nullptr);
    bool OpenDB(const QString &path);
    bool InsertItem(const QString &barcode, QString &name, QString &phone);
    bool UpdateClientPhotoUrl(const QString &barcode, QString &photoUrl);
    bool UpdateOrderPhotoUrl(const QString &barcode, QString &photoUrl);
    bool UpdateIsTaken(const QString &barcode, int isTaken);
    bool UpdateOutDate(const QString &barcode, const QString &outDate);
    bool IsItemExist(const QString &barcode);
    bool IsItemOut(const QString &barcode);
    inline SqlTableModel *expTableModel() const {
        return m_expTableModel;
    }
    void RefreshModel();
    void SetFilter(QString &filterStr);
    typedef enum {
        BARCODE = 0,
        NAME,
        PHONE,
        INDATE,
        OUTDATE,
        ISTAKEN,
        EXPORDERPHOTOURL,
        CLIENTPHOTURL
    } COLUMN_NAME;

  private:
    QSqlDatabase db;
    bool IsRecordTableExist();
    bool CreateRecordTable();
    int GetItemsCount();
    FileIo *fileIo;
    SqlTableModel *m_expTableModel;
  signals:

  public slots:
};

#endif // DBOPERATE_H
