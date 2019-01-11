#ifndef SPEECH_H
#define SPEECH_H
#include <QTextToSpeech>

#include <QObject>
#include "commonhelper.h"

class Speech : public QObject {
    Q_OBJECT
  public:
    static Speech *getInstance();
    Q_INVOKABLE void say(const QString &);
  private:
    explicit Speech(QObject *parent = nullptr);
    Speech(const Speech &);
    Speech &operator=(const Speech &);
    QTextToSpeech *m_speech;
    static Speech *instance;
    CommonHelper *commonHelper;

  signals:

  public slots:
};

#endif // SPEECH_H
