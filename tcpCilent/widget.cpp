#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->nameTxt->setReadOnly(true);
    ui->bytesTxt->setReadOnly(true);
    ui->refresh->setEnabled(false);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), Qt::white);
    this->setPalette(palette);
    this->setFixedSize(877,504);
    //test
    this->setWindowIcon(QIcon(":/img/Qt2.jpg"));

    socket=new QTcpSocket;//创建通信套接字
    socket->connectToHost("127.0.0.1",8888);//主动连接服务器
    //ui->send->setEnabled(false);
    connect(socket,SIGNAL(connected()),this,SLOT(showConnect()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showConnect()
{
qDebug()<<"successful!";
ui->refresh->setEnabled(true);
//ui->lineEdit->setText("连接成功！");
//ui->connect->setEnabled(false);
//ui->send->setEnabled(true);
}


//暂时不管
void Widget::on_send_clicked()
{

}
//


//连接
void Widget::on_connect_clicked()
{
//socket->connectToHost("127.0.0.1",9999);//主动连接服务器
}
//

void Widget::on_upLoad_clicked()//上传
{
    //ui-> ->setVlue(0);
    QString filePath =QFileDialog::getOpenFileName(this,"选择你需要上传的文件", "/home/jana", "txt Files (*.png *.jpg *.bmp *.txt)");
    QString fileName;
    QFileInfo info;
    info=QFileInfo(filePath);
    fileName=info.fileName();
    ui->getfileInfo->setText(fileName);
    qDebug()<<filePath;
    qDebug()<<fileName;
    char len[10];
    char buf[1024];
    int ret;
    if(fileName.isEmpty()){
        QMessageBox::warning(this,"警告","上传文件不能为空");
        return;
    }
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"警告","打不开该文件，上传失败");
        return;
    }
    char order[5]="put";
    socket->write(order,sizeof(order));
    socket->write(fileName.toStdString().c_str(),100);
    while(socket->bytesAvailable()<2){
        socket->waitForReadyRead(10);
    }
    socket->read(buf,2);
    qDebug()<<"buf:"<<buf;
    if('N'==buf[0]){
        QMessageBox::warning(this,"警告","服务器拒绝写访问");
        file.close();
        file.remove();
        return;
    }
    sprintf(len,"%lld",file.size());
    qDebug()<<len;
    socket->write(len,sizeof(len));
    int count=0;
    while(!file.atEnd()){
        ret=file.read(buf,sizeof(buf));
        socket->write(buf,ret);
        count+=ret;
        //ui->  ->setValue(count/file.size*100);
        update();
    }
    file.close();
    QMessageBox::information(this,"提示","上传成功");


}

void Widget::on_refresh_clicked()//刷新
{
    ui->nameTxt->clear();
    char filename[100];
    char order[5]="list";
    qDebug()<<socket->write(order,sizeof(order));
    while(1){
        qDebug()<<"refresh";
        while(socket->bytesAvailable()<sizeof(filename))
            socket->waitForReadyRead(10);
        socket->read(filename,sizeof(filename));

        if(!strcmp(filename,"end")){

            break;}

        ui->nameTxt->append(filename);
    }
}

void Widget::on_download_clicked()//下载
{
    QString filename=ui->getFilename->text();
    if(filename.isEmpty()){
        QMessageBox::warning(this,"提示","输入不能为空!");
        return;
    }
    char order[5]="get";
    int filelen;
    char len[10];
    char buf[1024];
    socket->write(order,sizeof(order));
    socket->write(filename.toStdString().c_str(),100);
    while(socket->bytesAvailable()<2)
        socket->waitForReadyRead(10);
    socket->read(buf,2);
    if('N'==buf[0]){
        QMessageBox::warning(this,"错误","服务器拒绝了你的下载");
        return;
    }
    filename="."+filename;
    qDebug()<<"filename: "<<filename;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"错误","你无法创建这个文件");
        return;
    }
    while(socket->bytesAvailable()<sizeof(len))
        socket->waitForReadyRead();
    socket->read(len,sizeof(len));
    filelen=atoi(len);
    qDebug()<<"filelen: "<<filelen;
    int count=0;
    int ret;
    while(count<filelen){
        if((filelen-count)>=sizeof(buf)){

          ret=socket->read(buf,sizeof(buf));
    }
        else
            ret=socket->read(buf,filelen-count);
        file.write(buf,ret);
        count+=ret;
        qDebug()<<"count:"<<count;
    }
    file.close();
    QMessageBox::information(this,"提示","下载成功");
}
