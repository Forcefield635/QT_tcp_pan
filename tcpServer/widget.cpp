#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QtDebug>
#include <QFileInfo>
#include<QMessageBox>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{


    ui->setupUi(this);
    //初始化
    int port=8888;
    ui->lineEdit->setText("Monitoring!");
    ui->lineEditFlag->setText("X");
    ui->lineEditPort->setText(QString::number(port));
    flag=0;
    server =new QTcpServer;//创建监听套接字

    if(!server->listen(QHostAddress::AnyIPv4,port)){
        qDebug()<<"listen:"<<server->errorString();
        return;
    }
    //connect：关联信号与槽的函数
    //参数：谁产生信号      信号      谁处理信号       槽函数
    connect(server,SIGNAL(newConnection()),this,SLOT(acceptClient()));


}

Widget::~Widget()
{
    delete ui;
}

void Widget::acceptClient()
{
    //接受客户端的链接
//    if(server->hasPendingConnections())//判断是否有链接
//    {
        //接收客户端的链接，返回通信套接字
    socket=server->nextPendingConnection();
    ui->lineEdit->setText("There is a connection...");
    qDebug()<<"There is a connection...";
//    }
    if(!socket->isValid())
    {
        ui->lineEdit->setText("connection failed !");
        qDebug()<<"connection failed !";
        return ;
    }
    ui->lineEdit->setText("Waiting for data ");
    connect(socket,SIGNAL(readyRead()),this,SLOT(recvData()));


}


/*  flag
    0   识别指令 list get put 并改变flag
    1   客户端下载文件  file: S->C
    2   客户端上传文件  file: C->S
    3   服务器保存文件
    4   服务器保存文件

*/
void Widget::recvData()
{
    ui->lineEdit->setText("recvData...");
    qDebug()<<"recvData";
    char order[5];
    char filename[100]="../pandServer/";
    char filename0[100];
    char buf[1024*1024];
    char len[10];
    int filelen;
    if(0==flag){
        ui->lineEditFlag->setText(QString::number(flag));
        qDebug()<<flag;
        socket->read(order,sizeof(order));
        if(!strcmp(order,"list")){//strcmp相等返回0  前<后 <0
            QDir dir("../pandServer/");
            if(!dir.exists()){
                socket->write("end",100);
                return;
            }
            dir.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
            QFileInfoList list=dir.entryInfoList();
            for(int i=0;i<list.size();i++)
            {
                QFileInfo fileInfo=list.at(i);
                qDebug()<<fileInfo.fileName();
                socket->write(fileInfo.fileName().toStdString().c_str(),100);
            }
            socket->write("end",100);
        }else if(!strcmp(order,"get"))
        {
            qDebug()<<"enter get S->C";
            flag=1;
            ui->lineEditFlag->setText("1");
        }else if(!strcmp(order,"put"))
        {
            qDebug()<<"enter put";
            flag=2;
            ui->lineEditFlag->setText("3");
        }
        else{
            return;
        }
    }
    if(flag==1){
        ui->lineEditFlag->setText("1");
        qDebug()<<flag;
        socket->read(filename0,100);
        strcat(filename,filename0);
        file.setFileName(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug()<<filename<<":No such file!";
            socket->write("N",2);
            flag=0;
            ui->lineEditFlag->setText(QString::number(flag));
            return;
        }
        socket->write("Y",2);
        sprintf(len,"%lld",file.size());
        socket->write(len,sizeof(len));
        int ret;
        int allret;
        while(!file.atEnd())
        {
            ret=file.read(buf,sizeof(buf));
            socket->write(buf,ret);
            allret+=ret;
        }
        file.close();
        qDebug()<<"get over! Total bytes sent:"<<allret;
        flag=0;
        ui->lineEditFlag->setText(QString::number(flag));
    }
    if(2==flag){
        ui->lineEditFlag->setText(QString::number(flag));
        qDebug()<<flag;
        if(socket->bytesAvailable()<sizeof(filename0))
            return;
        socket->read(filename0,sizeof(filename0));
        strcat(filename,filename0);
        qDebug()<<"filename:"<<filename0;
        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
        {
            socket->write("N",2);
            qDebug()<<"file.open N";
            flag=0;
            return;
        }
        socket->write("Y",2);
        qDebug()<<"file.open Y";
        flag=3;

    }
    if(flag==3){
        ui->lineEditFlag->setText(QString::number(flag));
        qDebug()<<flag;
        if(socket->bytesAvailable()<sizeof(len))
            return;
        socket->read(len,sizeof(len));
        filelen=atoi(len);
        flag=4;
        ui->lineEditFlag->setText(QString::number(flag));
    }
    if(4==flag){
        ui->lineEditFlag->setText(QString::number(flag));
        qDebug()<<flag;

        qDebug()<<"socket->bytesAvailable"<<socket->bytesAvailable();
        if(socket->bytesAvailable()<filelen)
            return;
        socket->read(buf,filelen);
        qDebug()<<"filelen:"<<filelen;
        file.write(buf,filelen);
        file.close();
        flag=0;
        ui->lineEditFlag->setText(QString::number(flag));
    }


}

void Widget::on_pushButton_clicked()
{

    this->close();
}


