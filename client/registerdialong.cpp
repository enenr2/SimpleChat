#include "registerdialong.h"
#include "ui_registerdialong.h"
#include"gloal.h"
#include<QMessageBox>
#include<QPalette>
#include<httpmgl.h>
#include<QDebug>
#include<QPixmap>
void RegisterDialong::initHttphandlers()
{
    _hanlders.insert(ReqId::ID_GET_VARIFY_CODE,[this](const QJsonObject&jsonObj){
        int error=jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email=jsonObj["email"].toString();
        showTip("验证码已发送",true);
        qDebug()<<"email is "<<email;
    });

    _hanlders.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug()<< "email is " << email ;
        changeTip();
    });
}

void RegisterDialong::showTip(QString str,bool b_ok=false)
{
    QPalette pat=ui->label_6->palette();
    if(b_ok){
        pat.setColor(QPalette::WindowText,Qt::yellow);
    }else{
        pat.setColor(QPalette::WindowText,Qt::red);
    }
    ui->label_6->clear();
    ui->label_6->setText(str);
    ui->label_6->setPalette(pat);
    repolish(ui->label_6);
}

bool RegisterDialong::checkUserValid()
{
    if(ui->lineEdit->text()==""){
        AddTipErr(TipErr::TIP_USER_ERR,tr("用户名不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialong::checEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->lineEdit_2->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialong::checkPassValid()
{
    auto pass = ui->lineEdit_3->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    };
    return true;

}

bool RegisterDialong::checkConfirmValid()
{
    auto pass =ui->lineEdit_3->text();
    auto confirm=ui->lineEdit_4->text();
    if(confirm.length()<6||confirm.length()>15){
        AddTipErr(TipErr::TIP_CONFIRM_ERR,tr("密码长度应该为6~15"));
            return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    };
    return true;
}

bool RegisterDialong::checkVarifyValid()
{
    auto pass = ui->lineEdit_5->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialong::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te]=tips;
    showTip(tips,false);
}

void RegisterDialong::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->label_6->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}

RegisterDialong::RegisterDialong(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialong)
{
    ui->setupUi(this);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    repolish(ui->label_6);
    connect(Httpmgl::GetInstance().get(),&Httpmgl::sig_reg_mod_finish,this,&RegisterDialong::sig_reg_mod_finish);
    initHttphandlers();
    ui->label_6->clear();

    connect(ui->lineEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });

    connect(ui->lineEdit_2,&QLineEdit::editingFinished,this,[this](){
        checEmailValid();
    });

    connect(ui->lineEdit_3,&QLineEdit::editingFinished,this,[this](){
        checkPassValid();
    });

    connect(ui->lineEdit_4,&QLineEdit::editingFinished,this,[this](){
        checkConfirmValid();
    });

    connect(ui->lineEdit_5, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->label_8->setCursor(Qt::PointingHandCursor);
    ui->label_7->setCursor(Qt::PointingHandCursor);
    ui->label_8->setPixmap(QPixmap(":/images/res/visible.png"));
    ui->label_7->setPixmap(QPixmap(":/images/res/visible.png"));
}

RegisterDialong::~RegisterDialong()
{
    delete ui;
}

void RegisterDialong::on_pushButton_2_clicked()
{
    auto email=ui->lineEdit_2->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        QJsonObject json_obj;
        json_obj["email"]=email;
        qDebug()<<(gate_url_prefix+"/get_variflycode");
        Httpmgl::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),json_obj,ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }else{
        showTip("邮箱地址不正确",false);
        QMessageBox::warning(this,"错误","邮箱地址格式错误");
    }
}

void RegisterDialong::sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS){
        showTip("网络请求错误",false);
        return;
    }
    QJsonDocument jsondoc=QJsonDocument::fromJson(res.toUtf8());
    if(jsondoc.isEmpty()){
        showTip("json解析失败",false);
        return;
    }
    if(!jsondoc.isObject()){
        showTip("json解析失败",false);
        return;
    }

    _hanlders[id](jsondoc.object());
    return;
}


void RegisterDialong::on_pushButton_clicked()
{
    if(ui->lineEdit->text()==QString("")){
        showTip("用户名不能为空",false);
        return;
    }

    if(ui->lineEdit_2->text()==""){
        showTip("邮箱不能为空",false);
        return;
    }

    if(ui->lineEdit_3->text()==""){
        showTip("密码不能为空");
        return;
    }

    if(ui->lineEdit_4->text()==""){
        showTip("确认密码不能为空");
        return;
    }

    if(ui->lineEdit_5->text()==""){
        showTip("验证码不能为空");
        return;
    }

    if(ui->lineEdit_3->text()!=ui->lineEdit_4->text()){
        showTip("密码和确认密码不匹配");
        return;
    }

    QJsonObject json_obj;
    json_obj["user"]=ui->lineEdit->text();
    json_obj["email"]=ui->lineEdit_2->text();
    json_obj["passwd"]=xorString(ui->lineEdit_3->text());
    json_obj["confirm"]=xorString(QString(ui->lineEdit_4->text()));
    json_obj["varifycode"]=ui->lineEdit_5->text();

    Httpmgl::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}


void RegisterDialong::on_pushButton_3_clicked()
{
    emit sigSwithLogin();
}

void RegisterDialong::changeTip()
{
    QMessageBox::information(this,"提示","注册成功\n点击确定返回");
    emit sigSwithLogin();
}

