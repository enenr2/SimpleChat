#include "clickedbtn.h"
#include "gloal.h"

ClickedBtn::ClickedBtn(QWidget *parent):QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor);
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    _hover=hover;
    _normal=normal;
    _press=press;
    setProperty("state",normal);
    repolish(this);
    update();
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
    // setProperty("state",_press);
    // repolish(this);
    // update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    //emit sig_changeIcon(false);
    QPushButton::mouseReleaseEvent(event);
}

void ClickedBtn::enterEvent(QEnterEvent *event)
{
    emit sig_changeIcon(true);
    // setProperty("state",_hover);
    // repolish(this);
    // update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    emit sig_changeIcon(false);
    QPushButton::leaveEvent(event);
}