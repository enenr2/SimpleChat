#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QListWidget>
#include <QObject>
#include<QDebug>
#include<QWheelEvent>
#include<QScrollEvent>

class ChatUserList : public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget*parent=nullptr);

signals:
    void sig_loading_chat_user();

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // CHATUSERLIST_H
