#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = 0);

    void onConnected();
    void onDisconnected();

signals:

public slots:
public:
    QList<QTcpSocket*> s_socketlist;
};

#endif // HELPER_H
