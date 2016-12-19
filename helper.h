#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTcpServer>
#include <QTcpSocket>
class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = 0, QSystemTrayIcon* tray = 0);

    void onConnected();
    void onDisconnected();
    void onReadyRead();
signals:

public slots:
public:
    QList<QTcpSocket*> s_socketlist;
private:
    QSystemTrayIcon* tray_;
};

#endif // HELPER_H
