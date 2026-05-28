#include "chatpage.h"
#include "ui_chatpage.h"
#include<QPainter>
#include<QStyleOption>
#include"gloal.h"
#include"textbubble.h"
#include"picturebubble.h"
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
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::Self;
    QString userName = QStringLiteral("测试气泡");
    QString userIcon = ":/images/res/1c47ffaa3fa0851c66c281627e8de64b.jpg";

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for(int i=0; i<msgList.size(); ++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if(type == "text")
        {
            pBubble = new TextBubble(role,msgList[i].content,this);
        }
        else if(type == "image")
        {
            pBubble = new PictureBubble(QPixmap(msgList[i].content) , role,this);
        }
        else if(type == "file")
        {

        }
        if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }
    }
}

