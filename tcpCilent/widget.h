#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include<QDebug>
#include<QPalette>//设置窗口背景色
#include<QLabel>//创建控件
#include<QMessageBox>
#include<QDir>
#include<QFile>
#include<iostream>
#include<QFileDialog>
#include<QFileInfo>
#include<QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void showConnect();

    void on_send_clicked();

    void on_connect_clicked();


    void on_upLoad_clicked();

    void on_refresh_clicked();

    void on_download_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;

};

#endif // WIDGET_H
