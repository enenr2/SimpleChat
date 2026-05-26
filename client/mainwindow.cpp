#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"resetdialong.h"
#include"chatdialog.h"
#include"tcpmgr.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    longin=new LonginDialog(this);
    setCentralWidget(longin);

    connect(longin,&LonginDialog::switchRegister,this,&MainWindow::slotswitchreg);
    longin->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(longin,&LonginDialog::switchReset,this,&MainWindow::slotswitchReset);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_switc_chatlg,this,&MainWindow::slotswitchchat);

    emit TcpMgr::GetInstance()->sig_switc_chatlg();

}

MainWindow::~MainWindow()
{
    qDebug()<<"登陆界面被析构";
    delete ui;
}

void MainWindow::slotswitchreg()
{
    reg=new RegisterDialong(this);
    reg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(reg,&RegisterDialong::sigSwithLogin,this,&MainWindow::sigSwithLogin);
    setCentralWidget(reg);
    longin->hide();
    reg->show();
}

void MainWindow::sigSwithLogin()
{
    longin=new LonginDialog(this);
    reg->hide();
    longin->show();
    setCentralWidget(longin);
    connect(longin,&LonginDialog::switchRegister,this,&MainWindow::slotswitchreg);
    connect(longin,&LonginDialog::switchReset,this,&MainWindow::slotswitchReset);
    longin->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

void MainWindow::slotswitchReset()
{
    reset=new ResetDialong(this);
    reset->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    longin->hide();
    reset->show();
    setCentralWidget(reset);
    connect(reset,&ResetDialong::switchlogin,this,&MainWindow::slotswitchlogin2);
}

void MainWindow::slotswitchlogin2()
{
    longin=new LonginDialog(this);
    reset->hide();
    longin->show();
    setCentralWidget(longin);
    connect(longin,&LonginDialog::switchRegister,this,&MainWindow::slotswitchreg);
    connect(longin,&LonginDialog::switchReset,this,&MainWindow::slotswitchReset);
    longin->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

void MainWindow::slotswitchchat()
{
    chat=new ChatDialog(this);
    //chat->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    longin->hide();
    this->hide();
    chat->show();
    setCentralWidget(chat);
    chat->setMinimumSize(QSize(1050,750));
    chat->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    connect(chat,&ChatDialog::sig_CloseAll,this,&MainWindow::CloseAll);
}

void MainWindow::CloseAll()
{
    this->close();
}
