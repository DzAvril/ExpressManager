#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "startup.h"
#include "speech.h"
#include "fileio.h"
#include "facedetectfilter.h"
#include "commonhelper.h"
#include <QZXing.h>
#include "inoutoperator.h"
// #include "outexpmodel.h"
#include "persondatabahandler.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    Startup startup;
    startup.SystemUp();

    QQmlApplicationEngine engine;
    Speech *speech = Speech::getInstance();
    FileIo *fileIo = FileIo::getInstance();
    CommonHelper *commonHelper = CommonHelper::getInstance();
    InOutOperator *inOutOperator = new InOutOperator();
//     OutExpModel *outExpModel = new OutExpModel();

    bool dbStarted;
    QString errorString;
    PersonDatabaHandler db("mydata.db");
    db.start(&dbStarted, &errorString, [&](SqlDatabaseHandler * db) {
        SqlTable personTable;

        SqlField idField;
        SqlField nameField;
        SqlField ageField;
        idField.setName("id");
        idField.setType(SqlField::FieldType::INT);
        idField.SetConstraints(SqlField::FieldConstranints::PRIMARY_KEY
                               | SqlField::FieldConstranints::AUTO_INCREMENT
                               | SqlField::FieldConstranints::NOT_NULL);
        nameField.setName("name");
        nameField.setType(SqlField::FieldType::STRING);
        nameField.SetConstraints(SqlField::FieldConstranints::NOT_NULL);

        ageField.setName("age");
        ageField.setType(SqlField::FieldType::INT);
        ageField.SetConstraints(SqlField::FieldConstranints::NONE);

        personTable.setName("person");
        personTable.addField(idField);
        personTable.addField(ageField);
        personTable.addField(nameField);

        db->addTable(personTable);

    });
    if (dbStarted) {
        qDebug() << "Database is started.";
    } else {
        qDebug() << "error : " << errorString;
        return -1;
    }
//     OutExpModel personModel;
    engine.rootContext()->setContextProperty("personModel", db.personTableModel());

//     engine.rootContext()->setContextProperty(QStringLiteral("outExpModel"),
//             outExpModel);
    engine.rootContext()->setContextProperty(QStringLiteral("speech"),
            speech);
    engine.rootContext()->setContextProperty(QStringLiteral("fileIo"),
            fileIo);
    engine.rootContext()->setContextProperty(QStringLiteral("commonHelper"),
            commonHelper);
    engine.rootContext()->setContextProperty(QStringLiteral("inOutOperator"),
            inOutOperator);
    qmlRegisterType<FaceDetectFilter>("opencv.lib", 1, 0, "FaceDetectFilter");

    QZXing::registerQMLTypes();

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *object = component.create();
    startup.SetObject(object);

    return app.exec();
}
