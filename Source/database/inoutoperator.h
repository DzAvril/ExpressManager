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
    Q_PROPERTY(QString earliestDate READ earliestDate NOTIFY earliestYearChanged)
    Q_PROPERTY(QString mostYearNumber READ mostYearNumber NOTIFY mostYearNumberChanged)
    Q_PROPERTY(QString mostMonthNumber READ mostMonthNumber NOTIFY mostMonthNumberChanged)
    Q_PROPERTY(QString mostDayNumber READ mostDayNumber NOTIFY mostDayNumberChanged)
    Q_PROPERTY(QString mostYear READ mostYear NOTIFY mostYearChanged)
    Q_PROPERTY(QString mostMonth READ mostMonth NOTIFY mostMonthChanged)
    Q_PROPERTY(QString mostDay READ mostDay NOTIFY mostDayChanged)
  public:
    explicit InOutOperator(QObject *parent = nullptr);
    ~InOutOperator();
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
    Q_INVOKABLE bool deleteRow(int row);
    Q_INVOKABLE bool markUnOut(int row);
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
    inline QString earliestDate() const {
        return m_earliestDate;
    }
    inline QString mostYearNumber() const {
        return m_mostYearNumber;
    }
    inline QString mostMonthNumber() const {
        return m_mostMonthNumber;
    }
    inline QString mostDayNumber() const {
        return m_mostDayNumber;
    }
    inline QString mostYear() const {
        return m_mostYear;
    }
    inline QString mostMonth() const {
        return m_mostMonth;
    }
    inline QString mostDay() const {
        return m_mostDay;
    }
  private:
    DbOperate *db;
    SqlTableModel *recordModel;
    SqlTableModel *yearModel;
    SqlTableModel *monthModel;
    SqlTableModel *dayModel;
    Speech *speech;
    FileIo *fileIo;
    CommonHelper *commonHelper;
    QString filterString, filterBarcode, filterOutDate, filterStartOutDate, filterEndOutDate;
    void PackFilterFrame();
    QList<QString> filters;
    QStringList  m_yearList;
    QLineSeries *m_lineYear;
    QLineSeries *m_lineMonth;
    QString m_earliestDate;
    void GetEarliestExpDate();
    void CreateLines();
    void UpdateCount(bool isAdd);
    QString m_mostYearNumber;
    QString m_mostMonthNumber;
    QString m_mostDayNumber;
    QString m_mostYear;
    QString m_mostMonth;
    QString m_mostDay;
    void GetMost();
  signals:
    void updateDatabaseDone();
    void lineYearChanged();
    void lineMonthChanged();
    void yearListChanged();
    void earliestYearChanged();
    void mostYearNumberChanged();
    void mostMonthNumberChanged();
    void mostDayNumberChanged();
    void mostYearChanged();
    void mostMonthChanged();
    void mostDayChanged();
  public slots:
};

#endif // INOUTOPERATOR_H
