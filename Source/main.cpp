#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "startup.h"
#include "speech.h"
#include "fileio.h"
#include "facedetectfilter.h"
#include "commonhelper.h"
#include <QZXing.h>
#include "inoutoperator.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    Startup startup;
    startup.SystemUp();

    QQmlApplicationEngine engine;
    Speech *speech = Speech::getInstance();
    FileIo *fileIo = FileIo::getInstance();
    CommonHelper *commonHelper = CommonHelper::getInstance();
    InOutOperator *inOutOperator = new InOutOperator();
    engine.rootContext()->setContextProperty(QStringLiteral("speech"),
            speech);
    engine.rootContext()->setContextProperty(QStringLiteral("fileIo"),
            fileIo);
    engine.rootContext()->setContextProperty(QStringLiteral("commonHelper"),
            commonHelper);
    engine.rootContext()->setContextProperty(QStringLiteral("inOutOperator"),
            inOutOperator);
    engine.rootContext()->setContextProperty("expModel", inOutOperator->expDb()->expTableModel());

    qmlRegisterType<FaceDetectFilter>("opencv.lib", 1, 0, "FaceDetectFilter");

    QZXing::registerQMLTypes();

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/Source/view/main.qml")));
    QObject *object = component.create();
    startup.SetObject(object);

    return app.exec();
}
