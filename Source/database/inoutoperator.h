#ifndef INOUTOPERATOR_H
#define INOUTOPERATOR_H

#include <QObject>
#include "dboperate.h"
#include "speech.h"
#include "fileio.h"
#include "commonhelper.h"
#include <QVariantList>

class InOutOperator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList  yearList READ yearList)
  public:
    explicit InOutOperator(QObject *parent = nullptr);
    Q_INVOKABLE bool in(QString barcode, QString name = nullptr,
                        QString phone = nullptr);
    Q_INVOKABLE bool out(QString barcode, QString photoUrl);
    Q_INVOKABLE bool isItemAlreadyOut(QString barcode);
    Q_INVOKABLE void setBarcodeFilter(QString barcode);
    Q_INVOKABLE void setOutDateFilter(QString outdate);
    Q_INVOKABLE void setStartOutDateFilter(QString startOutDate);
    Q_INVOKABLE void setEndOutDateFilter(QString endOutDate);
    Q_INVOKABLE void resetFilter();
    Q_INVOKABLE void updateOrderPhoto(QString barcode, QString url);
    Q_INVOKABLE QString getEarliestExpDate() const;
    Q_INVOKABLE int getExpCountFromDateRange(QString start, QString end);
    Q_INVOKABLE QList<QMap<int, int>> getExpCountOfMonth(QString month);
    Q_INVOKABLE int getExpCountOfDay(QString day);
    Q_INVOKABLE QString get(int row, int role) const;
    Q_INVOKABLE bool deleteRow(int row);
    QStringList  yearList();
    inline DbOperate *expDb() const {
        return db;
    }
  private:
    DbOperate *db;
    SqlTableModel *model;
    Speech *speech;
    FileIo *fileIo;
    CommonHelper *commonHelper;
    QString filterString, filterBarcode, filterOutDate, filterStartOutDate, filterEndOutDate;
    void PackFilterFrame();
    QList<QString> filters;
    QStringList  m_yearList;
  signals:
    void updateDatabaseDone();
  public slots:
};

#endif // INOUTOPERATOR_H
