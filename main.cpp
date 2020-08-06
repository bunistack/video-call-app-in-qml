#include <QApplication>
#include <FelgoApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "vcfilter.h"
#include "vcvideoimage.h"
#include "vcaudio.h"
#include "vcwebsocketmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    FelgoApplication felgo;

    QQmlApplicationEngine engine;
    felgo.initialize(&engine);

    VCWebSocketManager *wsManager = new VCWebSocketManager(&engine);

    VCFilter::registerQMLType();
    VCVideoImage::registerQMLType();
    VCAudio::registerQMLType();

    //register context types
    engine.rootContext()->setContextProperty("ws",wsManager);

    felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));
    engine.load(QUrl(felgo.mainQmlFileName()));

    return app.exec();
}
