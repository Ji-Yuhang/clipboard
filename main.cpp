#include <QApplication>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QClipboard>
#include <QMenu>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDialog>
#include <QLabel>
#include "helper.h"
#include "history.h"
static QAction* s_action_open = NULL;
static QAction* s_action_close = NULL;
static QString last_str0_;
static QString last_str1_;
static QString last_str2_;
static QTimer* s_timer_ = NULL;
static bool s_is_listen = true;
static QTcpServer* s_tcpserver = NULL;
//static QList<QTcpSocket*> s_socketlist;
static Helper* s_helper = NULL;
static bool s_is_notify = true;
static QSystemTrayIcon* s_tray = NULL;
//History* s_history = NULL;
#include <QJsonDocument>
#include <QJsonObject>
void onChange()
{
    QClipboard *board = QApplication::clipboard();
    QString str0 = board->text(QClipboard::Clipboard);
    QString str1 = board->text(QClipboard::Selection);// only linux x11
    QString str2 = board->text(QClipboard::FindBuffer);
//    qDebug() << str0 << str1 <<str2;


    {
        //TODO: callback
        if (last_str0_ != str0 || last_str1_ != str1 || last_str2_ != str2) {
            qDebug() << str0 << str1 <<str2;


            if (last_str0_ != str0) {
                if (History::singleton()) {
                    History::singleton()->append(str0,str1,str2);
                }
                // showMessage
                if (s_tray) s_tray->showMessage("剪贴板变化: " + str0,str0, QSystemTrayIcon::Information, 500);
                // SOqDebugCKET
//                if (s_helper && !s_helper->s_socketlist.isEmpty()) {
//                    Q_FOREACH(QTcpSocket* socket, s_helper->s_socketlist) {
//                        if (socket && socket->isWritable()) {
//                            socket->write(str0.toUtf8() + "\n");
//                            socket->flush();
//                        }
//                    }
//                }




            }

            {
                // json
                QJsonObject json;
                json.insert("Clipboard", str0);
                json.insert("Selection", str1);
                json.insert("FindBuffer", str2);
                QString changed;
                if (last_str0_ != str0 )
                    changed = "Clipboard";
                if (last_str1_ != str1 )
                    changed = "Selection";
                if (last_str2_ != str2)
                    changed = "FindBuffer";
                json.insert("changed",changed);

                QJsonDocument document;
                document.setObject(json);
                QByteArray byte_array = document.toJson(QJsonDocument::Compact);
                QString json_str(byte_array);

                if (s_helper && !s_helper->s_socketlist.isEmpty()) {
                    Q_FOREACH(QTcpSocket* socket, s_helper->s_socketlist) {
                        if (socket && socket->isWritable()) {
                            socket->write(json_str.toUtf8() + "\n");
                            socket->flush();
                        }
                    }
                }

            }
            // system call
            QString call_back_path = QDir::homePath() + QDir::separator()+".clipboard_callback";
            if (QFile::exists(call_back_path)) {
                QStringList args;
                args << str0 << str1 << str2;
                bool is_callback = QProcess::startDetached(call_back_path, args );
                qDebug() << "system call ~/.clipboard_callback "<< is_callback;

            }

        }
    }
    last_str0_ = str0;
    last_str1_ = str1;
    last_str2_ = str2;
}

void onDataChanged()
{
    if (s_is_listen) onChange();
}
void onSelectionChanged()
{
    if (s_is_listen) onChange();
}
void openListen()
{
    s_is_listen = true;
    if (s_action_open) s_action_open->setEnabled(false);
    if (s_action_close) s_action_close->setEnabled(true);
    if (s_timer_) s_timer_->start(200);

}
void closeListen()
{
    s_is_listen = false;
    if (s_action_open) s_action_open->setEnabled(true);
    if (s_action_close) s_action_close->setEnabled(false);
    if (s_timer_) s_timer_->stop();
}
//void onConnected()
//{
////    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
////    if (!socket ) return;


////    s_socketlist << socket;
//    qDebug() << "append " << 1 << "socket";
//    qDebug() << "current socket: "<< s_socketlist.size();

//}

//void onDisconnected()
//{

////    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());
////    if (!socket ) return;
//    int number = s_socketlist.removeAll(socket);
//    qDebug() << "remove " << number << "socket";
//    qDebug() << "current socket: "<< s_socketlist.size();
//}
void onNewConnection()
{
    if (!s_tcpserver) return;
    QTcpSocket* socket = s_tcpserver->nextPendingConnection();
    s_helper->s_socketlist << socket;
//    QObject::connect(socket, &QTcpSocket::connected, &onConnected);
//    QObject::connect(socket, &QTcpSocket::disconnected, &onDisconnected);
    QObject::connect(socket, &QTcpSocket::connected, s_helper, &Helper::onConnected);
    QObject::connect(socket, &QTcpSocket::disconnected, s_helper, &Helper::onDisconnected);
    QObject::connect(socket, &QTcpSocket::readyRead, s_helper, &Helper::onReadyRead);

    qDebug() << "append " << 1 << "socket";
    qDebug() << "current socket: "<< s_helper->s_socketlist.size();
//    s_helper
}

void ontimeout()
{
    if (s_is_listen) onChange();
}
void helpWidget() {
    QLabel* label = new QLabel;
    label->resize(600,300);
    label->setText(""
                   "1. 使用 TCP 连接端口 11011, 剪贴板变化时候 会发送一条带回车的utf-8字符串。使用 readline阻塞即可\n"
                   "2. 在系统目录下建立 ~/.clipboard_callback 文件, 剪贴板变化时会被调用\n"
                   "");
    label->show();

    label->setWindowModified(true);

}
void showHistory() {
    History::singleton()->activateWindow();
    History::singleton()->showNormal();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Ji-Yuhang");
    QCoreApplication::setOrganizationDomain("iamyuhang.com");
    QCoreApplication::setApplicationName("clipboard");
    a.setWindowIcon(QIcon(":/ico/qt_128px_1174036_easyicon.net.ico"));
    a.setQuitOnLastWindowClosed(false);
    QSystemTrayIcon* tray = new QSystemTrayIcon;
    s_tray = tray;
    tray->setIcon(QIcon(":/ico/qt_128px_1174036_easyicon.net.ico"));
    tray->show();
    History history;// = new History;
//    s_history = history;
    QMenu* menu = new QMenu();
    menu->addAction("帮助", &helpWidget);
    menu->addAction("查看历史记录", &showHistory);
    menu->addAction("退出程序",&a.quit);
    QAction* action_open = menu->addAction("开启监听", &openListen);
    action_open->setEnabled(false);
    QAction* action_close = menu->addAction("关闭监听", &closeListen);
    s_action_open = action_open;
    s_action_close = action_close;

    tray->setContextMenu(menu);
    tray->setToolTip("剪贴板监听工具");
    QClipboard *board = QApplication::clipboard();
    bool c0 = QObject::connect(board, &QClipboard::changed, &onChange);
    bool c1 = QObject::connect(board, &QClipboard::dataChanged, &onDataChanged);
//    bool c2 = connect(board, &QClipboard::findBufferChanged, this, &MainWindow::onFindBufferChanged);
    bool c3 = QObject::connect(board, &QClipboard::selectionChanged, &onSelectionChanged);
//    qDebug() << c0<< c1<< c2<<c3;
    QTimer* timer = new QTimer;
    s_timer_ = timer;
    QObject::connect(timer, &QTimer::timeout, &ontimeout);
#ifdef Q_OS_OSX
    timer->start(200);
#endif
    QTcpServer* tcpserver = new QTcpServer;
    s_tcpserver = tcpserver;
    bool listen_satus =  tcpserver->listen(QHostAddress::Any, 11011);
    Helper* helper = new Helper(0,tray);
    s_helper = helper;
    QObject::connect(tcpserver, &QTcpServer::newConnection, &onNewConnection);
    if (!listen_satus) {
        tray->showMessage("监听 11011 端口出错","监听 11011 端口出错, 3秒自动退出");
        QTimer::singleShot(3000,&a, &QApplication::quit);
    }


    qDebug() << c1;
//    MainWindow w;
//    w.show();
//    w.setWindowFlags(Qt::WindowStaysOnTopHint);
    return a.exec();
}
