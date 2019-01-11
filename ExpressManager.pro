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
        Source/main.cpp \
    Source/startup.cpp \
    Source/database/dboperate.cpp \
    Source/common/speech.cpp \
    Source/common/fileio.cpp \
    Source/opencv/facedetectfilter.cpp \
    Source/common/commonhelper.cpp \
    Source/database/inoutoperator.cpp \
    Source/database/sqltablemodel.cpp

RESOURCES += qml.qrc

INCLUDEPATH += C:/opencv-4.0.1/build/install/include
INCLUDEPATH += Source/common \
Source/database \
Source/opencv \

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
    Source/startup.h \
    Source/database/dboperate.h \
    Source/common/config.h \
    Source/common/speech.h \
    Source/common/fileio.h \
    Source/opencv/facedetectfilter.h \
    Source/common/commonhelper.h \
    Source/database/inoutoperator.h \
    Source/database/sqltablemodel.h

#qml_scenes.depends = $$PWD/resources/scene1.qml $$PWD/resources/scene2.qml ...
#qml_scenes.commands =
#QMAKE_EXTRA_TARGETS += qml_scenes
