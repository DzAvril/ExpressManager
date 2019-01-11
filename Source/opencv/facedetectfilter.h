#ifndef FACEDETECTFILTER_H
#define FACEDETECTFILTER_H

#include <QAbstractVideoFilter>
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <QTemporaryFile>
#include <QMutex>

class FaceDetectFilter : public QAbstractVideoFilter {
    Q_OBJECT
  public:
    QVideoFilterRunnable *createFilterRunnable();
  signals:
    void objectDetected(float x, float y, float w, float h);

  public slots:
};

class FaceDetectFilterRunnable : public QVideoFilterRunnable {
  public:
    FaceDetectFilterRunnable(FaceDetectFilter *creator) {
        filter = creator;
    }
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

  private:
    void dft(cv::InputArray input, cv::OutputArray output);
    FaceDetectFilter *filter;
};

#endif // FACEDETECTFILTER_H
