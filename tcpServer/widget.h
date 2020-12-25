#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpServer>//监听套接字
#include<QTcpSocket>//通信套接字
#include<QDebug>
#include<QFile>
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
    void acceptClient();
    void recvData();
    void on_pushButton_clicked();
private:
    Ui::Widget *ui;
    int flag;
    QTcpServer *server;
    QTcpSocket *socket;
    QFile file;

};

#endif // WIDGET_H
