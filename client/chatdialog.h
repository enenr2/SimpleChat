#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include"gloal.h"
#include "qlistwidget.h"
#include "statewidget.h"
#include "userdata.h"
#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void addChatUserList();
    void SetSelectChatItem(int uid=0);
    void SetSelectChatPage(int uid=0);
private:
    void ShowSearch(bool bsearch);
    void AddLBGroup(StateWidget*lb);
    void ClearLabelState(StateWidget *lb);
    void handleGlobalMousePress(QMouseEvent*event);
    void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>>msgdata);
    void loadMoreChatUser();
    void loadMoreConUser();
    QMap<int ,QListWidgetItem*>_chat_items_added;
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*>_lb_list;
    int _cur_chat_uid;
    QWidget* _last_widget=nullptr;
signals:
    void sig_CloseAll();
public slots:
    void changeicon(bool isok);
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString & str);
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_add_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_friend_info_page(std::shared_ptr<UserInfo> user_info);
    void slot_loading_contact_user();
    void slot_switch_apply_friend_page();
    void slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info);
    void slot_item_clicked(QListWidgetItem*item);
    void slot_append_send_chat_msg(std::shared_ptr<TextChatData>msgdata);
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // CHATDIALOG_H
