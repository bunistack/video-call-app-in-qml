#ifndef MYFILTER_H
#define MYFILTER_H

#include <QVideoFilterRunnable>
#include <QDebug>
#include <QQmlEngine>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QWebSocket>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QImageWriter>

#include <private/qvideoframe_p.h>

class VCFilter : public QAbstractVideoFilter {
    Q_OBJECT
friend class VCFilterRunnable;

public:
    explicit VCFilter(QObject *parent = nullptr);
    virtual ~VCFilter();

    QVideoFilterRunnable *createFilterRunnable();

    void static registerQMLType();

signals:
    void imageDataReady(QString data);

private:
    QFuture<void> workerThread;
    QVector<QImage> imagesQueue;
    bool exitThread = false;

    QImage videoFrameToImage(QVideoFrame *frame);   
    void work();
    void preprocessImage(QImage image);
};






class VCFilterRunnable : public QObject, public QVideoFilterRunnable {

public:
    explicit VCFilterRunnable(VCFilter *filter);
    virtual ~VCFilterRunnable();

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);   

private:
    VCFilter *filter;
    bool isProcessing = false;
};


#endif // MYFILTER_H
