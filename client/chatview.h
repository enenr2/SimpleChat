#ifndef CHATVIEW_H
#define CHATVIEW_H

#include "qwidget.h"
#include "userdata.h"
#include <QObject>
#include<QTimer>
#include<QScrollArea>

class QVBoxLayout;

class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);
    void appendChatItem(QWidget*item);
    void prependChatItem(QWidget*item);
    void removeAllItem();
    void insertChatItem(QWidget*before,QWidget*item);
signals:

    // QObject interface
public:
    virtual bool eventFilter(QObject *o, QEvent *e) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
protected slots:
    void onVScorllBarMoved(int min,int max);
private:
    //QWidget *m_pCenterWidget;
    QVBoxLayout *m_pVl;
    QScrollArea *m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
