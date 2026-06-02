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
    ERR_NETWORK = 1,            // 客户端独有：网络错误
    ERR_JSON = 1001,            // 对齐服务端 Error_Json
    ERR_RPC_FAILED = 1002,      // 对齐 RPCFailed
    ERR_VERIFY_EXPIRED = 1003,  // 对齐 VarifyExpired
    ERR_VERIFY_CODE = 1004,     // 对齐 VarifyCodeErr
    ERR_USER_EXIST = 1005,      // 对齐 UserExist
    ERR_PASSWORD_ERR = 1006,    // 对齐 PasswdErr
    ERR_EMAIL_NOT_MATCH = 1007, // 对齐 EmailNotMatch
    ERR_PASSWORD_UPDATE = 1008, // 对齐 PasswdUpFailed
    ERR_PASSWORD_INVALID = 1009,// 对齐 PasswdInvalid
    ERR_TOKEN_INVALID = 1010,   // 对齐 TokenInvalid
    ERR_UID_INVALID = 1011,     // 对齐 UidInvalid

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

extern std::vector<QString> strs;

extern std::vector<QString> heads;

extern std::vector<QString> names;


//申请好友标签输入框最低长度
const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "添加标签 ";

const int  tip_offset = 5;

extern QString gate_url_prefix;

const int CHAT_COUNT_PER_PAGE = 13;

#endif // GLOAL_H