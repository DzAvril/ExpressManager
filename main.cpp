#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "startup.h"
#include "queryexpinfo.h"
#include "dboperate.h"
#include "speech.h"
#include "fileio.h"
#include "facedetectfilter.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    Startup startup;
    startup.SystemUp();

    QQmlApplicationEngine engine;
    QueryExpInfo queryExpInfo;
    Speech *speech = Speech::getInstance();
    FileIo *fileIo = FileIo::getInstance();
    engine.rootContext()->setContextProperty(QStringLiteral("queryExpInfo"),
            &queryExpInfo);
    engine.rootContext()->setContextProperty(QStringLiteral("speech"),
            speech);
    engine.rootContext()->setContextProperty(QStringLiteral("fileIo"),
            fileIo);
    qmlRegisterType<DbOperate>("DbOperate", 1, 0, "DbOperate");
    qmlRegisterType<FaceDetectFilter>("opencv.lib", 1, 0, "FaceDetectFilter");

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *object = component.create();
    startup.SetObject(object);

    return app.exec();
}
