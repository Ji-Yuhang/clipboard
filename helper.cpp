#include "helper.h"
#include <QDebug>
Helper::Helper(QObject *parent, QSystemTrayIcon *tray) : QObject(parent),tray_(tray)
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

void Helper::onReadyRead()
{
    qDebug() << "helper onDisconnected";

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket ) return;
    QString str = socket->readAll().trimmed();
//    if (tray_ && !str.isEmpty()) tray_->showMessage("剪贴板callback: ",str, QSystemTrayIcon::Information, 2000);

}
