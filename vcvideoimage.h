#ifndef VCVIDEOIMAGE_H
#define VCVIDEOIMAGE_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

class VCVideoImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(FillMode fillMode MEMBER m_fillMode NOTIFY fillModeChanged)
public:
    VCVideoImage(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    Q_INVOKABLE void updateImage(QString data);

    static void registerQMLType();

    enum FillMode {
        PreserveAspectCrop = 0,
        PreserveAspectFit = 1
    };

    Q_ENUMS(FillMode)

signals:
    void fillModeChanged();

private:
    QImage image;
    bool isUpdating = false;
    FillMode m_fillMode = PreserveAspectFit;    
};

#endif // VCVIDEOIMAGE_H
