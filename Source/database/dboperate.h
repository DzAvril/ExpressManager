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
    ~DbOperate();
    bool OpenDB(const QString &path);
    bool InsertRecord(const QString &barcode, QString &name, QString &phone);
    bool UpdateClientPhotoUrl(const QString &barcode, QString &photoUrl);
    bool UpdateOrderPhotoUrl(const QString &barcode, QString &photoUrl);
    bool UpdateIsTaken(const QString &barcode, int isTaken);
    bool UpdateOutDate(const QString &barcode, const QString &outDate);
    bool IsItemExist(const QString &barcode);
    bool IsItemOut(const QString &barcode);
    inline SqlTableModel *GetRecordModel() const {
        return m_recordModel;
    }
    inline SqlTableModel *GetYearModel() const {
        return m_yearModel;
    }
    inline SqlTableModel *GetMonthModel() const {
        return m_monthModel;
    }
    inline SqlTableModel *GetDayModel() const {
        return m_dayModel;
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
    bool IsTableExist(QString tableName);
    bool CreateRecordTable();
    bool CreateYearTable();
    bool CreateMonthTable();
    bool CreateDayTable();
    int GetItemsCount();
    FileIo *fileIo;
    SqlTableModel *m_recordModel;
    SqlTableModel *m_yearModel;
    SqlTableModel *m_monthModel;
    SqlTableModel *m_dayModel;
  signals:

  public slots:
};

#endif // DBOPERATE_H
