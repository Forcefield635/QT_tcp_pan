# QT_tcp通信

实现一个网盘的基本的上传和下载功能

1.了解并掌握Qt集成开发环境下使用Qt Designer开发图形界面程序的流程和相关操作；

2.掌握在Qt Designer界面上为菜单项添加工具栏按钮、创建相应槽函数的方法；

3.了解并掌握Qt的信号槽和信号传递机制，并通过具体应用加深理解；

4.了解并掌握TCP通信机制并掌握QTcpSocket及QTcpServer类的具体使用方法；

## tcp_Cilent客户端

### 功能

## tcp_Server服务器端

设置一个flag来选择接收客户端的信息的处理方式

flag = 0 时

识别指令“list”“put”“get”

list:

返回目录信息给客户端，直接处理

get:

客户端下载文件flag设为1

put:

客户端上传文件flag设为2

flag = 1

获取客户端想要下载的文件名称

返回信息 文件是否打开

返回信息 文件大小

传输文件

flag置0；

flag = 2

获取客户端想要上传的文件名称

返回信息 文件是否创建成功

flag=3

获取文件长度

flag=4

传输文件

flag置0；