#include "vcfilter.h"

VCFilter::VCFilter(QObject *parent) : QAbstractVideoFilter(parent)
{    
    workerThread = QtConcurrent::run(this, &VCFilter::work);
}

VCFilter::~VCFilter()
{
    exitThread = true;

    if(!workerThread.isFinished()) {
      workerThread.cancel();
      workerThread.waitForFinished();
    }
}

QVideoFilterRunnable *VCFilter::createFilterRunnable()
{    
    return new VCFilterRunnable(this);
}

void VCFilter::registerQMLType()
{    
    qmlRegisterType<VCFilter>("VCFilter", 1, 0, "VCFilter");
}

QImage VCFilter::videoFrameToImage(QVideoFrame *frame)
{
    if(frame->handleType() == QAbstractVideoBuffer::NoHandle){

        QImage image = qt_imageFromVideoFrame(*frame);

        if(image.isNull()){
            qDebug() << "-- null image from qt_imageFromVideoFrame";
            return QImage();
        }

        if(image.format() != QImage::Format_RGB32){
            image = image.convertToFormat(QImage::Format_RGB32);
        }

        return image;
    }

    if(frame->handleType() == QAbstractVideoBuffer::GLTextureHandle){
        QImage image(frame->width(), frame->height(), QImage::Format_RGB32);
        GLuint textureId = frame->handle().toUInt();//static_cast<GLuint>(frame.handle().toInt());
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1,&fbo);
        GLint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING,&prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, frame->width(), frame->height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(prevFbo));
        return image.rgbSwapped();
    }

    qDebug() << "-- Invalid image format...";
    return QImage();
}

void VCFilter::work()
{

    QImage image;

    while (!exitThread) {

        if(imagesQueue.isEmpty()) continue;

        image = imagesQueue.at(0);
        preprocessImage(image.copy());
        imagesQueue.remove(0);
    }

}

VCFilterRunnable::VCFilterRunnable(VCFilter *filter) : QObject(nullptr), filter(filter)
{

}

VCFilterRunnable::~VCFilterRunnable()
{
    filter = nullptr;
}

QVideoFrame VCFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if(!input || !input->isValid()){
        return QVideoFrame();
    }

    if(isProcessing){
        return * input;
    }

    isProcessing = true;

    QImage image = filter->videoFrameToImage(input);
    filter->imagesQueue.append(image);

    isProcessing = false;

    return * input;
}

void VCFilter::preprocessImage(QImage image)
{    

    //if android, make image upright
#ifdef Q_OS_ANDROID
    QPoint center = image.rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(90);
    image = image.transformed(matrix);
#endif

    double resizedWidth, resizedHeight;
    resizedWidth = 640;
    resizedHeight = ((double)image.height()/(double)image.width()) * resizedWidth;
    image = image.scaled(resizedWidth, resizedHeight, Qt::KeepAspectRatio);


    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    QImageWriter writer(&buffer,QByteArray("JPEG"));
    //image.save(&buffer, "PNG");
    writer.setQuality(50);
    writer.write(image);

    //qDebug() << "pic uncompressed size: " << byteArray.size() << " size: " << image.width();

    QString data = QString::fromStdString(byteArray.toBase64().toStdString());
    emit imageDataReady(data);

}

