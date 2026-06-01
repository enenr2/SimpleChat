#include "longindialog.h"
#include "gloal.h"
#include"httpmgl.h"
#include "ui_longindialog.h"
#include"QMessageBox"
#include"tcpmgr.h"
LonginDialog::LonginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LonginDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&LonginDialog::switchRegister);
    ui->label_4->setCursor(Qt::PointingHandCursor);
    ui->label_4->ChangeCur(ClickLbState::Text);
    connect(ui->label_4,&ClichedLabel::clicked,this,&LonginDialog::slot_forget_pwd);
    initHttpHandlers();
    connect(Httpmgl::GetInstance().get(),&Httpmgl::sig_http_finish,this,&LonginDialog::slot_login_mod_finish);
    //连接tcp连接请求的信号和槽函数
    connect(this, &LonginDialog::sig_connect_tcp, TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    //连接tcp管理者发出的连接成功信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_con_success, this, &LonginDialog::slot_tcp_con_finish);
}

LonginDialog::~LonginDialog()
{
    delete ui;
}

void LonginDialog::slot_forget_pwd()
{
    qDebug()<<"创建界面:忘记密码";
    emit switchReset();
}

void LonginDialog::on_pushButton_2_clicked()
{
    qDebug()<<"处理请求：登录";
    if(checkUserValid()==false)
        return;
    if(checkPwdValid()==false)
        return;
    enableBtn(false);
    auto email=ui->lineEdit->text();
    auto pwd=ui->lineEdit_2->text();

    QJsonObject json_obj;
    json_obj["user"]=email;
    json_obj["email"]=email;
    json_obj["passwd"]=xorString(pwd);
    Httpmgl::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj,ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}

void LonginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{

    if(err != ErrorCodes::SUCCESS){
        QMessageBox::warning(this,"错误","网络请求错误");
        return;
    }

    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull()){
        QMessageBox::warning(this,"错误","Json解析错误");
        return;
    }

    //json解析错误
    if(!jsonDoc.isObject()){
        QMessageBox::warning(this,"错误","Json解析错误");
        return;
    }


    //调用对应的逻辑,根据id回调。
    _handlers[id](jsonDoc.object());

    return;
}

void LonginDialog::slot_tcp_con_finish(bool bsuccess)
{
    if(bsuccess){
        QMessageBox::information(this,"成功","连接成功，正在登录中......");
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;
        QJsonDocument doc(jsonObj);
        QByteArray jsonString = doc.toJson(QJsonDocument::Indented);
        //发送tcp请求给chat server
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonString);
    }else{
        QMessageBox::warning(this,"错误","网络错误");
        enableBtn(true);
    }
}

bool LonginDialog::checkUserValid()
{
    auto email =ui->lineEdit->text();
    if(email.isEmpty()){
        qDebug()<<"邮箱地址为空";
        QMessageBox::warning(this,"错误","邮箱地址不能为空");
        return false;
    }
    return true;
}

bool LonginDialog::checkPwdValid()
{
    auto pwd = ui->lineEdit_2->text();
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "密码应该在6到15位之间";
        QMessageBox::warning(this,"错误","密码应该在6到15位之间");
        return false;
    }

    return true;
}

void LonginDialog::enableBtn(bool ok)
{
    ui->pushButton_2->setEnabled(ok);
    ui->pushButton_3->setEnabled(ok);
}

void LonginDialog::initHttpHandlers()
{
    //注册获取登录回包逻辑
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            QMessageBox::warning(this,"错误","Json出现错误");
            enableBtn(true);
            return;
        }
        auto user = jsonObj["user"].toString();
        //发送信号通知tcpMgr发送长链接
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();
        _uid = si.Uid;
        _token = si.Token;
        qDebug()<< "user is " << user << " uid is " << si.Uid <<" host is "
                 << si.Host << " Port is " << si.Port << " Token is " << si.Token;
        emit sig_connect_tcp(si);
    });
}