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
  private:
    DbOperate *db;
    Speech *speech;
    FileIo *fileIo;
    CommonHelper *commonHelper;
  signals:

  public slots:
};

#endif // INOUTOPERATOR_H
