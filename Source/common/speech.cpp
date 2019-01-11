#include "speech.h"
#include <QtDebug>

Speech::Speech(QObject *parent) : QObject(parent), m_speech(nullptr) {
    m_speech = new QTextToSpeech(this);
    commonHelper = CommonHelper::getInstance();
}

//Speech::Speech(const Speech &) {
//    return;
//}

void Speech::say(const QString &content) {
    while (m_speech->state() == QTextToSpeech::Speaking) {
        commonHelper->delay(500);
    }
    m_speech->say(content);
}

Speech *Speech::instance = new Speech();
Speech *Speech::getInstance() {
    return instance;
}
