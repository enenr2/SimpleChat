#ifndef USERMGR_H
#define USERMGR_H
#include"singletion.h"
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
private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
};

#endif // USERMGR_H
