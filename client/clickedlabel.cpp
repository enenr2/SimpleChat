#include "clickedlabel.h"
#include<QMouseEvent>
#include<QLabel>
#include<QPixmap>
#include<QPalette>
ClichedLabel::ClichedLabel(QWidget *parent):QLabel (parent),_curstate(ClickLbState::Normal){}

void ClichedLabel::ChangeCur(ClickLbState now)
{
    this->_curstate=now;
}

void ClichedLabel::enterEvent(QEnterEvent *event)
{
    // 在这里处理鼠标悬停进入的逻辑
    // if(_curstate == ClickLbState::Normal){
    //     qDebug()<<"enter , change to normal hover: "<< _normal_hover;
    //     setProperty("state",_normal_hover);
    //     repolish(this);
    //     update();

    // }else if(_curstate==ClickLbState::Selected){
    //     qDebug()<<"enter , change to selected hover: "<< _selected_hover;
    //     setProperty("state",_selected_hover);
    //     repolish(this);
    //     update();
    // }
    if(_curstate==ClickLbState::Text)
    {
    qDebug()<<"鼠标进入";
    QPalette Palette=this->palette();
    Palette.setColor(QPalette::WindowText,Qt::blue);
    this->setPalette(Palette);
    //repolish(this);
    update();
    }
    QLabel::enterEvent(event);
}

void ClichedLabel::leaveEvent(QEvent *event)
{
    // 在这里处理鼠标悬停离开的逻辑
    // if(_curstate == ClickLbState::Normal){
    //     qDebug()<<"leave , change to normal : "<< _normal;
    //     setProperty("state",_normal);
    //     repolish(this);
    //     update();

    // }else{
    //     qDebug()<<"leave , change to normal hover: "<< _selected;
    //     setProperty("state",_selected);
    //     repolish(this);
    //     update();
    // }
    if(_curstate==ClickLbState::Text){
    qDebug()<<"鼠标离开";
    QPalette Palette=this->palette();
    Palette.setColor(QPalette::WindowText,Qt::black);
    this->setPalette(Palette);;
    //repolish(this);
    update();
    }
    QLabel::leaveEvent(event);
}

void ClichedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{

}

ClickLbState ClichedLabel::GetCurState()
{

}

void ClichedLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(_curstate == ClickLbState::Normal){
            qDebug()<<"clicked , change to selected hover: "<< _selected_hover;
            _curstate = ClickLbState::Selected;
            setProperty("state",_selected_hover);
            repolish(this);
            ChangeIron(ClickLbState::Selected);
            update();

        }else if(_curstate == ClickLbState::Selected){
            qDebug()<<"clicked , change to normal hover: "<< _normal_hover;
            _curstate = ClickLbState::Normal;
            setProperty("state",_normal_hover);
            repolish(this);
            ChangeIron(ClickLbState::Normal);
            update();
        }
        emit clicked();
        return;
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(event);
}

void ClichedLabel::ChangeIron(ClickLbState now)
{
    if(now==Selected){
        this->clear();
        QPixmap pix(":/images/res/visible_hover.png");
        this->setPixmap(pix);
    }else{
        this->clear();
        QPixmap pix(":/images/res/visible.png");
        this->setPixmap(pix);
    }
}