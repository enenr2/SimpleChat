#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H
#include<QPushButton>

class ClickedBtn:public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget*parent=nullptr);
    ~ClickedBtn();
    void SetState(QString normal,QString hover,QString press);

protected:
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void sig_changeIcon(bool isok);
private:
    QString _normal;
    QString _hover;
    QString _press;
};


#endif // CLICKEDBTN_H
