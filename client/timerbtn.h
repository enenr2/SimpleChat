#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>

class QTimer;

class TimerBtn : public QPushButton
{
public:
    TimerBtn(QWidget*parent=nullptr);
    ~TimerBtn();
private:
    QTimer *_timer;
    int _counter;

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // TIMERBTN_H
