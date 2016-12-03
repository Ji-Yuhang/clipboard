#include "history.h"
#include "ui_history.h"
#include <QDebug>
#include <QFileDialog>
 History* History::g_history = NULL;
History::History(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::History)
{

    ui->setupUi(this);
    sortModel_.setSourceModel(&listModel_);
    ui->listView->setModel(&sortModel_);
//    ui->listView->setSortingEnabled(true);
//    treeView->setModel(model);
    QSettings settings("clipboard/history");
    datas = settings.value("words",QStringList()).toStringList();
    listModel_.setStringList(datas);
    qDebug() << "load history "<< datas;
    g_history = this;
}

History::~History()
{
    QSettings settings("clipboard/history");
    settings.setValue("words",datas);
    qDebug()<<"save history "<< datas;
    delete ui;
}

History *History::singleton()
{
    if (!g_history) new History;
    return g_history;
}

void History::append(const QString &str0, const QString &str1, const QString &str2)
{
    datas << str0;
    listModel_.setStringList(datas);
//    listModel_
}

void History::on_lineEdit_textChanged(const QString &arg1)
{
    sortModel_.setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive,
                                                 QRegExp::FixedString));
//    sortModel_->setFilterKeyColumn(0);
}

void History::on_button_clear_clicked()
{
    datas.clear();
    listModel_.setStringList(datas);

}

void History::on_button_export_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "选择保存的文件");
    qDebug() << "save file"<<filename;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::WriteOnly);
    Q_FOREACH(QString one ,datas) {
        file.write(one.toUtf8() + "\n");
    }

    // write to stderr
    file.close();

}
