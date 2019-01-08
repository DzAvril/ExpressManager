#include "commonhelper.h"
#include <QTime>
#include <QCoreApplication>
#include "opencv2/opencv.hpp"

CommonHelper::CommonHelper(QObject *parent) : QObject(parent) {

}

CommonHelper::CommonHelper(const CommonHelper &) {

}

CommonHelper &CommonHelper::operator=(const CommonHelper &) {

}

CommonHelper *CommonHelper::instance = new CommonHelper();
CommonHelper *CommonHelper::getInstance() {
    return instance;
}

void CommonHelper::delay(int n) {
    QTime dieTime = QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void CommonHelper::PutTextToImage(QString &photoUrl, QString &barcode, QString &time) {
    cv::Mat image = cv::imread(photoUrl.toStdString());
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontSacle = 1.5;
    int thickness = 1;
    int baseline;
    int lineType = cv::LINE_AA;
    cv::Scalar color(255, 255, 255);
    cv::Point origin;
    origin.x = 5;
    origin.y = image.rows - 5;
    cv::putText(image, time.toStdString(), origin, fontFace, fontSacle, color, thickness, lineType);
    cv::Size textSize = cv::getTextSize(time.toStdString(), fontFace, fontSacle, thickness, &baseline);
    origin.y = image.rows - 8 - textSize.height;
    cv::putText(image, barcode.toStdString(), origin, fontFace, fontSacle, color, thickness, lineType);
    cv::imwrite(photoUrl.toStdString(), image);
}
