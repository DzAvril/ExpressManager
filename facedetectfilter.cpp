#include "facedetectfilter.h"
using namespace cv;
//cv::CascadeClassifier classifier;

QVideoFilterRunnable *FaceDetectFilter::createFilterRunnable() {
    return new FaceDetectFilterRunnable(this);
}

QVideoFrame FaceDetectFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat,
        QVideoFilterRunnable::RunFlags flags) {
    Q_UNUSED(flags);
    input->map(QAbstractVideoBuffer::ReadOnly);

    if (surfaceFormat.handleType() == QAbstractVideoBuffer::NoHandle) {
        QImage image(input->bits(),
                     input->width(),
                     input->height(),
                     QVideoFrame::imageFormatFromPixelFormat(input->pixelFormat()));
        image = image.convertToFormat(QImage::Format_RGB888);
        Mat mat(image.height(),
                image.width(),
                CV_8UC3,
                image.bits(),
                image.bytesPerLine());
    }
}
