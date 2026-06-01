#ifndef USERMGR_H
#define USERMGR_H
#include"singletion.h"
#include "userdata.h"
#include <QObject>
#include<memory>

class UserMgr:public QObject,public singletion<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class singletion<UserMgr>;
    ~UserMgr();
    void SetName(QString name);
    bool AlreadyApply(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo> app);
    void SetUid(int uid);
    int GetUid();
    QString GetName();
    void SetToken(QString token);
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void AppendApplyList(QJsonArray array);
    bool CheckFriendById(int uid);
    void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);
    void AddFriend(std::shared_ptr<AuthInfo> auth_info);
    std::shared_ptr<FriendInfo> GetFriendById(int uid);
    void AppendFriendList(QJsonArray array);

    std::vector<std::shared_ptr<FriendInfo>> GetChatListPerPage();
    bool IsLoadChatFin();
    void UpdateChatLoadedCount();
    std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
    void UpdateContactLoadedCount();
    bool IsLoadConFin();
    std::shared_ptr<UserInfo> GetUserInfo();
    void AppendFriendChatMsg(int friend_id, std::vector<std::shared_ptr<TextChatData> >msgs);
private:
    UserMgr();
    QString _token;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    std::shared_ptr<UserInfo> _user_info;
    QMap<int ,std::shared_ptr<FriendInfo>>_friend_map;
    std::vector<std::shared_ptr<FriendInfo>>_friend_list;
    int _chat_loaded;
    int _contact_loaded;
};

#endif // USERMGR_H
