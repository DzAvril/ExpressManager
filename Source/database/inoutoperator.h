#ifndef INOUTOPERATOR_H
#define INOUTOPERATOR_H

#include <QObject>
#include "dboperate.h"
#include "speech.h"
#include "fileio.h"
#include "commonhelper.h"

class InOutOperator : public QObject {
    Q_OBJECT
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
    DbOperate *expDb() const;
  private:
    DbOperate *db;
    SqlTableModel *model;
    Speech *speech;
    FileIo *fileIo;
    CommonHelper *commonHelper;
    QString filterString, filterBarcode, filterOutDate, filterStartOutDate, filterEndOutDate;
    void PackFilterFrame();
    QList<QString> filters;
  signals:
    void outSuccess();
  public slots:
};

#endif // INOUTOPERATOR_H
