#include "chatpage.h"
#include "ui_chatpage.h"
#include<QPainter>
#include<QStyleOption>
#include"gloal.h"
#include"textbubble.h"
#include"picturebubble.h"
#include"usermgr.h"
#include"textbubble.h"
#include"tcpmgr.h"
ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    QPixmap pix=QPixmap(":/images/res/smile.png");
    QPixmap scaledPix = pix.scaled(ui->emo_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->emo_lb->setPixmap(scaledPix);
    QPixmap pix2=QPixmap(":/images/res/filedir.png");
    scaledPix = pix2.scaled(ui->file_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->file_lb->setPixmap(scaledPix);
    // 2. 标题标签
    ui->title_lb->setStyleSheet(R"(
    #title_lb {
        font-family: "Microsoft YaHei";
        font-size: 18px;
        font-weight: normal;
    }
)");

    // 3. 聊天输入框
    ui->chatEdit->setStyleSheet(R"(
    #chatEdit {
        background: #ffffff;
        border: none;
        font-family: "Microsoft YaHei";
        font-size: 18px;
        padding: 5px;
    }
)");

    // 4. 发送区域背景
    ui->send_wid->setStyleSheet(R"(
    #widget_5 {
        background: #ffffff;
        border: none;
    }
)");

    // 5. 添加按钮 —— 三种状态
//     ui->add_btn->setStyleSheet(R"(
//     #add_btn[state='normal'] {
//         border-image: url(:/images/res/add_friend_normal.png);
//     }
//     #add_btn[state='hover'] {
//         border-image: url(:/images/res/add_friend_hover.png);
//     }
//     #add_btn[state='press'] {
//         border-image: url(:/images/res/add_friend_hover.png);
//     }
// )");

    // 6. 接收按钮
    ui->receive_btn->setStyleSheet(R"(
    #receive_btn[state='normal'] {
        background: #f0f0f0;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
    #receive_btn[state='hover'] {
        background: #d2d2d2;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
    #receive_btn[state='press'] {
        background: #c6c6c6;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
)");

    // 7. 发送按钮
    ui->send_btn->setStyleSheet(R"(
    #send_btn[state='normal'] {
        background: #f0f0f0;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
    #send_btn[state='hover'] {
        background: #d2d2d2;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
    #send_btn[state='press'] {
        background: #c6c6c6;
        color: #2cb46e;
        font-size: 16px;
        font-family: "Microsoft YaHei";
        border-radius: 20px;
    }
)");

    // 8. 工具栏背景
    ui->tool_wid->setStyleSheet(R"(
    #tool_wid {
        background: #ffffff;
        border-bottom: 0.5px solid #ececec;
    }
)");

    // 9. 表情标签（可能是 QLabel）
    ui->emo_lb->setStyleSheet(R"(
    #emo_lb[state='normal'] {
        border-image: url(:/images/res/smile.png);
    }
    #emo_lb[state='hover'] {
        border-image: url(:/images/res/smile_hover.png);
    }
    #emo_lb[state='press'] {
        border-image: url(:/images/res/smile_press.png);
    }
)");

    // 10. 文件标签
    ui->file_lb->setStyleSheet(R"(
    #file_lb[state='normal'] {
        border-image: url(:/images/res/filedir.png);
    }
    #file_lb[state='hover'] {
        border-image: url(:/images/res/filedir_hover.png);
    }
    #file_lb[state='press'] {
        border-image: url(:/images/res/filedir_press.png);
    }
)");
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ChatPage::on_send_btn_clicked()
{
    if (_user_info == nullptr) {
        qDebug() << "friend_info is empty";
        return;
    }

    auto user_info = UserMgr::GetInstance()->GetUserInfo();
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::Self;
    QString userName = user_info->_name;
    QString userIcon = user_info->_icon;

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    QJsonObject textObj;
    QJsonArray textArray;
    int txt_size = 0;

    for(int i=0; i<msgList.size(); ++i)
    {
        //消息内容长度不合规就跳过
        if(msgList[i].content.length() > 1024){
            continue;
        }

        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;

        if(type == "text")
        {
            //生成唯一id
            QUuid uuid = QUuid::createUuid();
            //转为字符串
            QString uuidString = uuid.toString();

            pBubble = new TextBubble(role, msgList[i].content);
            //将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
            //_bubble_map[uuidString] = pBubble;
            txt_size += msgList[i].content.length();
            if(txt_size > 1024 && i < msgList.size() - 1){
                textObj["fromuid"] = user_info->_uid;
                textObj["touid"] = _user_info->_uid;
                textObj["text_array"] = textArray;
                QJsonDocument doc(textObj);
                QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
                txt_size = 0;
                textArray = QJsonArray();
                textObj = QJsonObject();
                emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
            }
            QJsonObject obj;
            QByteArray utf8Message = msgList[i].content.toUtf8();
            obj["content"] = QString::fromUtf8(utf8Message);
            obj["msgid"] = uuidString;
            textArray.append(obj);
            auto txt_msg = std::make_shared<TextChatData>(uuidString, obj["content"].toString(),
                                                          user_info->_uid, _user_info->_uid);
            emit sig_append_send_chat_msg(txt_msg);
        }
        else if(type == "image")
        {
            //pBubble = new PictureBubble(QPixmap(msgList[i].content) , role);
        }
        else if(type == "file")
        {

        }
        //发送消息
        if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }

    }

    qDebug() << "textArray is " << textArray ;
    //发送给服务器
    textObj["text_array"] = textArray;
    textObj["fromuid"] = user_info->_uid;
    textObj["touid"] = _user_info->_uid;
    QJsonDocument doc(textObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    //发送并清空之前累计的文本列表
    txt_size = 0;
    textArray = QJsonArray();
    textObj = QJsonObject();
    //发送tcp请求给chat server
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
}



void ChatPage::SetUserInfo(std::shared_ptr<UserInfo> user_info)
{
    _user_info=user_info;
    ui->title_lb->setText(_user_info->_name);
    ui->chat_data_list->removeAllItem();
    for(auto &msg:user_info->_chat_msgs){
        AppendChatMsg(msg);
    }
}



void ChatPage::AppendChatMsg(std::shared_ptr<TextChatData> msg)
{
    auto self_info = UserMgr::GetInstance()->GetUserInfo();
    ChatRole role;
    //todo... 添加聊天显示
    if (msg->_from_uid == self_info->_uid) {
        role = ChatRole::Self;
        ChatItemBase* pChatItem = new ChatItemBase(role);

        pChatItem->setUserName(self_info->_name);
        pChatItem->setUserIcon(QPixmap(self_info->_icon));
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_list->appendChatItem(pChatItem);
    }
    else {
        role = ChatRole::Other;
        ChatItemBase* pChatItem = new ChatItemBase(role);
        auto friend_info = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
        if (friend_info == nullptr) {
            return;
        }
        pChatItem->setUserName(friend_info->_name);
        pChatItem->setUserIcon(QPixmap(friend_info->_icon));
        QWidget* pBubble = nullptr;
        pBubble = new TextBubble(role, msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_list->appendChatItem(pChatItem);
    }
}
