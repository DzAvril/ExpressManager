#ifndef INOUTOPERATOR_H
#define INOUTOPERATOR_H

#include <QObject>
#include "dboperate.h"
#include "speech.h"
#include "fileio.h"
#include "commonhelper.h"
#include <QVariantList>
#include<QtCharts/QLineSeries>
using namespace QtCharts;

class InOutOperator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList  yearList READ yearList NOTIFY yearListChanged)
    Q_PROPERTY(QLineSeries *lineYear READ lineYear WRITE set_lineYear NOTIFY lineYearChanged)
    Q_PROPERTY(QLineSeries *lineMonth READ lineMonth WRITE set_lineMonth NOTIFY lineMonthChanged)
    Q_PROPERTY(QString earliestYear READ earliestYear NOTIFY earliestYearChanged)
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
    Q_INVOKABLE int getExpCountFromDateRange(QString start, QString end);
    Q_INVOKABLE int getExpCountOfMonth(QString year);
    Q_INVOKABLE int getExpCountOfDay(QString year, QString month);
    Q_INVOKABLE QString get(int row, int role) const;
    Q_INVOKABLE bool deleteRow(int row);
    QStringList  yearList();
    inline QLineSeries *lineYear() {
        return m_lineYear;
    }
    void set_lineYear(QLineSeries *line);
    inline QLineSeries *lineMonth() {
        return m_lineMonth;
    }
    void set_lineMonth(QLineSeries *line);
    inline DbOperate *expDb() const {
        return db;
    }
    inline QString earliestYear() const {
        return m_earliestYear;
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
    QLineSeries *m_lineYear;
    QLineSeries *m_lineMonth;
    QString m_earliestYear;
    void GetEarliestExpDate();
    void CreateLines();
  signals:
    void updateDatabaseDone();
    void lineYearChanged();
    void lineMonthChanged();
    void yearListChanged();
    void earliestYearChanged();
  public slots:
};

#endif // INOUTOPERATOR_H
