#include "vcvideoimage.h"

VCVideoImage::VCVideoImage(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

void VCVideoImage::paint(QPainter *painter)
{

    double rectWidth, rectHeight;

    if(m_fillMode == PreserveAspectCrop){

        if(this->height() > this->width()){
            //height is the independent dimension
            rectHeight = this->height();
            rectWidth = ((double)this->image.width()/(double)this->image.height()) * rectHeight;
        }else {
            //width is the independent dimension
            rectWidth = this->width();
            rectHeight = ((double)this->image.height()/(double)this->image.width()) * rectWidth;
        }

    }else {

        if(this->image.width() > this->image.height()){
            //width is the independent dimension
            rectWidth = this->width();
            rectHeight = ((double)this->image.height()/(double)this->image.width()) * rectWidth;
        }else {
            //height is the independent dimension
            rectHeight = this->height();
            rectWidth = ((double)this->image.width()/(double)this->image.height()) * rectHeight;
        }

    }

    QRect rect(0,0,rectWidth,rectHeight);
    QPoint itemCenter((double)this->width()/2.0, (double)this->height()/2.0);
    rect.moveCenter(itemCenter);

    painter->drawImage(rect,this->image);

    isUpdating = false;
}

void VCVideoImage::updateImage(QString data)
{

    if(isUpdating) {
        //qDebug() << "-- dumping...";
        return;
    }

    isUpdating = true;

    QByteArray base64;
    base64.append(data);
    QByteArray byteArray = QByteArray::fromBase64(base64);

    image = QImage();

    if(image.loadFromData(byteArray,"JPEG")){
        qDebug() << "Image loaded...";
    }else {
        qDebug() << "Error loading image...";
    }

    if(!image.isNull()){
        update();
    }

}

void VCVideoImage::registerQMLType()
{
    qmlRegisterType<VCVideoImage>("VCVideoImage", 1, 0, "VCVideoImage");
}
