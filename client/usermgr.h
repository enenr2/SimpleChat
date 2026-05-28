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
    void SetUid(int uid);
    void SetToken(QString token);
    int GetUid();
    QString GetName();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
};

#endif // USERMGR_H
