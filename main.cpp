#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "startup.h"
#include "queryexpinfo.h"
#include "dboperate.h"
#include "speech.h"
#include "fileio.h"
#include "facedetectfilter.h"
#include "commonhelper.h"
#include <QZXing.h>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    Startup startup;
    startup.SystemUp();

    QQmlApplicationEngine engine;
    QueryExpInfo queryExpInfo;
    Speech *speech = Speech::getInstance();
    FileIo *fileIo = FileIo::getInstance();
    CommonHelper *commonHelper = CommonHelper::getInstance();
    engine.rootContext()->setContextProperty(QStringLiteral("queryExpInfo"),
            &queryExpInfo);
    engine.rootContext()->setContextProperty(QStringLiteral("speech"),
            speech);
    engine.rootContext()->setContextProperty(QStringLiteral("fileIo"),
            fileIo);
    engine.rootContext()->setContextProperty(QStringLiteral("commonHelper"),
            commonHelper);
    qmlRegisterType<DbOperate>("DbOperate", 1, 0, "DbOperate");
    qmlRegisterType<FaceDetectFilter>("opencv.lib", 1, 0, "FaceDetectFilter");

    QZXing::registerQMLTypes();

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *object = component.create();
    startup.SetObject(object);

    return app.exec();
}
