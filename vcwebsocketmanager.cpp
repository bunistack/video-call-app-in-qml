#include "vcwebsocketmanager.h"
#include <QJsonDocument>
#include <QJsonObject>

VCWebSocketManager::VCWebSocketManager(QObject *parent) : QObject(parent)
{

    ws = new QWebSocket();
    retryConnectionTimer = new QTimer(this);    

    //connections for ws
    connect(ws, SIGNAL(connected()), this, SLOT(connected()));
    connect(ws, SIGNAL(disconnected()), this, SLOT(closed()));
    connect(ws, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(ws, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
    connect(retryConnectionTimer, SIGNAL(timeout()), this, SLOT(wsConnect()));

    this->wsConnect();
}

/*
 * sends a websocket message to ws server
*/
void VCWebSocketManager::wsSend(QString message)
{

    if(ws->isValid()){
        QByteArray data;
        data.append(message);

        qint64 bytesSent = 0;

        //the while loop resends the message incase it fails to send
        int counter = 0;

        while(bytesSent < 1 && counter < 5){
            bytesSent = ws->sendBinaryMessage(data);
            counter++;
        }

        //qDebug() << "Sending message: " << message << "\n";

    }else{
        qDebug() << "Invaid ws..." << "\n";
    }    
}

/*
 * returns true if ws is ready for read or write, false otherwise
*/
bool VCWebSocketManager::isValid()
{
    return ws->isValid();
}

/*
 * opens connection to a dedicated ws websocket server
*/
void VCWebSocketManager::wsConnect()
{
    retryConnectionTimer->stop();

    if(!ws->isValid()){
        ws->open(QUrl(wsUrl));
    }
}

void VCWebSocketManager::wsReconnect()
{
    retryConnectionTimer->stop();

    ws->close();

    if(!ws->isValid()){
        ws->open(QUrl(wsUrl));
    }
}

/*
 * called when connection is made to the ws server
*/
void VCWebSocketManager::connected()
{
    qDebug() << "Connection established to ws server";

    emit stateChanged(1);
}

/*
 * called when connection to ws server is lost
*/
void VCWebSocketManager::closed()
{
    qDebug() << "Connection to ws server closed";

    emit stateChanged(0);

    retryConnectionTimer->start(reconnectionTimeout);
}

/*
 * called when an error occurs from the ws server
*/
void VCWebSocketManager::error(QAbstractSocket::SocketError error)
{
    qDebug() << error << " (ws server): " << ws->errorString();
}

/*
 * called when text message is received from the ws server
*/
void VCWebSocketManager::textMessageReceived(QString message)
{

    //qDebug() << " (ws server) " << message << "\n";

    QByteArray byteArray;
    byteArray.append(message);
    QJsonObject json = QJsonDocument::fromJson(byteArray).object();

    const QString type = json["type"].toString();
    const QString data = json["data"].toString();

    if(type == "audio"){        
        emit audioReceived(data);
    }else {
        emit imageReceived(data);
    }

    //end of function
}
