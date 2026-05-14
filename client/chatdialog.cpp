#include "chatdialog.h"
#include "ui_chatdialog.h"
#include"customizeedit.h"
#include<QRandomGenerator>
#include"chatuserwid.h"
#include"loadingdlg.h"
#include"chatuserlist.h"
#include<QIcon>

std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
    ":/images/res/1b27103fc30ee170ccc0f601dd20ca79.jpg",
    ":/images/res/771931129d0cb386762bc3c33742dff9.jpg",
    ":/images/res/59bc6e781a87beb3f61bc174b85a4ac7.jpg",
    ":/images/res/473e4cc830e8c5f795647248578fdade.jpg",
    ":/images/res/88843ce0e3e38325d108e44b10dbd243.jpg"
};

std::vector<QString> names = {
    "马自豪",
    "蒋玉辉",
    "薛佳乐",
    "支帝翔",
    "韩沅廷",
    "王雪冰",
    "王兆宇",
    "袁一凡"
};


ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog),_mode(ChatUIMode::ChatMode),_state(ChatUIMode::ChatMode),_b_loading(false)
{
    ui->setupUi(this);
    ui->side_bar->setStyleSheet("background-color: rgb(46,46,46);");
    ui->search_wid->setStyleSheet("background-color:rgb(247,247,247)");
    ui->search_list->setStyleSheet("border:2px solid #f1f1f1");
    //ui->chat_user_wid->setStyleSheet("background-color:rgb(0,0,0)");
    ui->chat_user_list->setStyleSheet("background-color:rgb(247,237,248);border:none;");
    ui->chat_user_list->setObjectName("chat_user_list");
    ui->chat_user_list->setStyleSheet(R"(
    #chat_user_list::item:selected {
        background-color: #d3d7d4;
        border: none;
        outline: none;
    }

    #chat_user_list::item:hover {
        background-color: rgb(206,207,208);
        border: none;
        outline: none;
    }

    #chat_user_list:focus {
        border: none;
        outline: none;
    }
    )");

    ui->chat_user_list->setStyleSheet(R"(
    #chatUserWidget {
        background-color: rgb(247,247,247);
        border: none;
    }

    #user_chat_lb {
        color: rgb(153,153,153);
        font-size: 12px;
        font-family: "Microsoft YaHei";
    }

    #user_name_lb {
        color: rgb(0,0,0);
        font-size: 14px;
        font-weight: normal;
        font-family: "Microsoft YaHei";
    }

    #time_lb {
        color: rgb(140,140,140);
        font-size: 12px;
        font-family: "Microsoft YaHei";
    }

    /* 只改变 #chat_user_list 内部的垂直滚动条 */
    #chat_user_list QScrollBar:vertical {
        background: transparent;
        width: 8px;
        margin: 0px;
    }

    #chat_user_list QScrollBar::handle:vertical {
        background: rgb(173,170,169);
        min-height: 10px;
        border-radius: 4px;
    }

    #chat_user_list QScrollBar::add-line:vertical,
    #chat_user_list QScrollBar::sub-line:vertical {
        height: 0px;
        border: none;
        background: transparent;
    }

    #chat_user_list QScrollBar::add-page:vertical,
    #chat_user_list QScrollBar::sub-page:vertical {
        background: none;
    }
    )");

    ui->lineEdit->SetMaxLength(15);

    QAction *searchAction=new QAction(ui->lineEdit);
    searchAction->setIcon(QIcon(":/images/res/search.png"));
    ui->lineEdit->addAction(searchAction,QLineEdit::LeadingPosition);
    ui->lineEdit->setPlaceholderText(QStringLiteral("搜索"));

    QAction*clearAction=new QAction(ui->lineEdit);
    clearAction->setIcon(QIcon(":/images/res/close_transparent.png"));
    ui->lineEdit->addAction(clearAction,QLineEdit::TrailingPosition);
    connect(ui->lineEdit,&QLineEdit::textChanged,[clearAction](const QString &text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/images/res/close_search.png"));
        }else{
            clearAction->setIcon(QIcon(":/images/res/close_transparent.png"));
        }
    });
    connect(clearAction,&QAction::triggered,[this,clearAction](){
        ui->lineEdit->clear();
        clearAction->setIcon(QIcon(":/images/res/close_transparent.png"));
        ui->lineEdit->clearFocus();
        ShowSearch(false);
    });
    ShowSearch(false);
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);
    addChatUserList();
    connect(ui->pushButton,&ClickedBtn::sig_changeIcon,this,&ChatDialog::changeicon);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::addChatUserList()
{
    // 创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *chat_user_wid = new ChatUserWid();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

void ChatDialog::ShowSearch(bool bsearch)
{
    if(bsearch){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }else if(_state == ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    }else if(_state == ChatUIMode::ContactMode){
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::changeicon(bool isok)
{
    if(isok==true)
        ui->pushButton->setIcon(QIcon(":/images/res/add_friend.png"));
    if(isok==false)
        ui->pushButton->setIcon(QIcon(":/images/res/add_friend_normal.png"));
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){
        return;
    }

    _b_loading = true;
    LoadingDlg *loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    addChatUserList();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::closeEvent(QCloseEvent *event)
{
    emit sig_CloseAll();
    QDialog::closeEvent(event);
}