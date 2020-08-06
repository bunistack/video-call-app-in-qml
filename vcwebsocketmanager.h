#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QTimer>

class VCWebSocketManager : public QObject
{
    Q_OBJECT
public:
    explicit VCWebSocketManager(QObject *parent = nullptr);

    Q_INVOKABLE void wsSend(QString message);
    Q_INVOKABLE bool isValid();

signals:
    void stateChanged(int status);
    void imageReceived(QString data);
    void audioReceived(QString data);

public slots:
    void wsConnect();
    void wsReconnect();
    void connected();
    void closed();
    void error(QAbstractSocket::SocketError error);
    void textMessageReceived(QString message);

private:
    QWebSocket *ws;
    QTimer *retryConnectionTimer;    
    int reconnectionTimeout = 20000;

#ifdef Q_OS_WIN
    const QString wsUrl = "ws://localhost:9100";
#endif

#ifdef Q_OS_ANDROID
    const QString wsUrl = "ws://192.168.43.128:9100";
#endif

};

#endif // WEBSOCKETMANAGER_H
