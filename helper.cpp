#include "helper.h"
#include <QDebug>
Helper::Helper(QObject *parent) : QObject(parent)
{

}

void  Helper::onConnected()
{
    qDebug() << "helper onconnected";
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket ) return;


//    s_socketlist << socket;
    qDebug() << "append " << 1 << "socket";
    qDebug() << "current socket: "<< s_socketlist.size();

}

void  Helper::onDisconnected()
{
    qDebug() << "helper onDisconnected";

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket ) return;
    int number = s_socketlist.removeAll(socket);
    qDebug() << "remove " << number << "socket";
    qDebug() << "current socket: "<< s_socketlist.size();
}
