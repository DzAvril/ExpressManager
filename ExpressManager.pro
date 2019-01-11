QT += quick
CONFIG += c++11
QT += sql
CONFIG+=release
QT += texttospeech
QT += multimedia
CONFIG += qzxing_qml
CONFIG += qzxing_multimedia

include(C:/qzxing/src/QZXing.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    startup.cpp \
    dboperate.cpp \
    speech.cpp \
    fileio.cpp \
    facedetectfilter.cpp \
    commonhelper.cpp \
    inoutoperator.cpp \
#    sqlfield.cpp \
#    sqlrecord.cpp \
#    sqltable.cpp \
#    sqldatabasehandler.cpp \
#    persondatabahandler.cpp \
    sqltablemodel.cpp

RESOURCES += qml.qrc

INCLUDEPATH += C:/opencv-4.0.1/build/install/include

LIBS += libopencv_core401.dll \
-lopencv_imgproc401.dll \
libopencv_imgcodecs401.dll \
libopencv_videoio401.dll \
libopencv_flann401.dll \
libopencv_highgui401.dll \
libopencv_features2d401.dll \
libopencv_photo401.dll \
libopencv_video401.dll \
libopencv_calib3d401.dll \
libopencv_objdetect401.dll \
libopencv_stitching401.dll \
libopencv_ml401.dll\

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    startup.h \
    dboperate.h \
    config.h \
    speech.h \
    fileio.h \
    facedetectfilter.h \
    commonhelper.h \
    inoutoperator.h \
#    sqlfield.h \
#    sqlrecord.h \
#    sqltable.h \
#    sqldatabasehandler.h \
#    persondatabahandler.h \
    sqltablemodel.h

#qml_scenes.depends = $$PWD/resources/scene1.qml $$PWD/resources/scene2.qml ...
#qml_scenes.commands =
#QMAKE_EXTRA_TARGETS += qml_scenes
