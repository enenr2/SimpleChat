#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H
#include <QObject>
#include<QEvent>
#include<QScrollEvent>
#include<memory.h>
#include<QDebug>
#include<QWheelEvent>
#include"userdata.h"
#include<QListWidget>

class ConUserItem;
class ContactUserList:public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget*parent=nullptr);
    void ShowRedPoint(bool bshow = true);
    std::shared_ptr<UserInfo> GetInfo();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override ;

private:
    void addContactUserList();

public slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_add_auth_firend(std::shared_ptr<AuthInfo>);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
signals:
    void sig_loading_contact_user();
    void sig_switch_apply_friend_page();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo> user_info);
private:
    ConUserItem* _add_friend_item;
    QListWidgetItem * _groupitem;
    bool _load_pending;
    std::shared_ptr<UserInfo> _info;
};

#endif // CONTACTUSERLIST_H
