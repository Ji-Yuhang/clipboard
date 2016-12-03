#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QStringList>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QSettings>
namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT

public:
    explicit History(QWidget *parent = 0);
    ~History();
    static History* singleton();
    void append(const QString& str0, const QString& str1, const QString& str2);

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_button_clear_clicked();

    void on_button_export_clicked();

private:
    static History* g_history;
    Ui::History *ui;
    QStringList datas;
    QStringListModel listModel_;
    QSortFilterProxyModel sortModel_;
};

#endif // HISTORY_H
