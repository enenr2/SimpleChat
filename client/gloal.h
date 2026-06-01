#ifndef GLOAL_H
#define GLOAL_H
#include<QWidget>
#include<functional>
#include"QStyle"
#include<QRegularExpression>
#include<memory>
#include<iostream>
#include<mutex>
#include<QByteArray>
#include<QNetworkReply>
#include<QJsonObject>
#include<QDir>
#include<QSettings>
extern void  repolish(QWidget*w);
extern std::function<QString(QString)> xorString;

enum ReqId{
    ID_GET_VARIFY_CODE = 1001,       //获取验证码
    ID_REG_USER = 1002,              //注册用户
    ID_RESET_PWD = 1003,             //重置密码
    ID_LOGIN_USER = 1004,            //用户登录
    ID_CHAT_LOGIN = 1005,            //登陆聊天服务器
    ID_CHAT_LOGIN_RSP= 1006,         //登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007,       //用户搜索请求
    ID_SEARCH_USER_RSP = 1008,       //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,        //添加好友申请
    ID_ADD_FRIEND_RSP = 1010,        //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011, //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,       //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,       //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015,//通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,    //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,    //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
    ID_NOTIFY_OFF_LINE_REQ = 1021,   //通知用户下线
    ID_HEART_BEAT_REQ = 1023,        //心跳请求
    ID_HEARTBEAT_RSP = 1024,         //心跳回复
    ID_LOAD_CHAT_THREAD_REQ = 1025,  //加载聊天线程
    ID_LOAD_CHAT_THREAD_RSP = 1026,  //加载聊天线程回复
    ID_CREATE_PRIVATE_CHAT_REQ = 1027, //创建私聊请求
    ID_CREATE_PRIVATE_CHAT_RSP = 1028, //创建私聊回复
    ID_LOAD_CHAT_MSG_REQ = 1029,     //加载聊天消息
    ID_LOAD_CHAT_MSG_RSP = 1030,     //加载聊天消息
};

enum Modules{
    REGISTERMOD=0,
    RESETMOD=1,
    LOGINMOD=2
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ErrorCodes{
    SUCCESS=0,
    ERR_JSON=1,    //json 解析失败
    ERR_NETWORK=2, //网络错误

};

enum ClickLbState{
    Normal = 0,
    Selected = 1,
    Text = 3
};

enum ChatUIMode{
    SearchMode,
    ChatMode,
    ContactMode,
};

enum ListItemType{
    CHAT_USER_ITEM,       // 聊天用户
    CONTACT_USER_ITEM,    // 联系人用户
    SEARCH_USER_ITEM,     // 搜索到的用户
    ADD_USER_TIP_ITEM,    // 提示添加用户
    INVALID_ITEM,         // 不可点击条目
    GROUP_TIP_ITEM,       // 分组提示条目
    APPLY_FRIEND_ITEM
};

enum class ChatRole{
    Self,
    Other
};

struct MsgInfo{
    QString msgFlag;//"text,image,file"
    QString content;//表示文件和图像的url,文本信息
    QPixmap pixmap;//文件和图片的缩略图
};

struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

inline std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

inline std::vector<QString> heads = {
    ":/images/res/1b27103fc30ee170ccc0f601dd20ca79.jpg",
    ":/images/res/771931129d0cb386762bc3c33742dff9.jpg",
    ":/images/res/59bc6e781a87beb3f61bc174b85a4ac7.jpg",
    ":/images/res/473e4cc830e8c5f795647248578fdade.jpg",
    ":/images/res/88843ce0e3e38325d108e44b10dbd243.jpg"
};

inline std::vector<QString> names = {
    "马自豪",
    "蒋玉辉",
    "薛佳乐",
    "支帝翔",
    "韩沅廷",
    "王雪冰",
    "王兆宇",
    "袁一凡"
};


//申请好友标签输入框最低长度
const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "添加标签 ";

const int  tip_offset = 5;

extern QString gate_url_prefix;

const int CHAT_COUNT_PER_PAGE = 13;

#endif // GLOAL_H
