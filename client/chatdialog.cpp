#include "chatdialog.h"
#include "ui_chatdialog.h"
#include"customizeedit.h"
#include<QRandomGenerator>
#include"chatuserwid.h"
#include"loadingdlg.h"
#include"chatuserlist.h"
#include<QIcon>
#include<QPixmap>
#include<QMouseEvent>



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
    ui->side_chat_lb->setStyleSheet("border-image: url(:/images/res/chat_icon_press.png) 0 0 0 0 stretch;");
    ui->side_contact_lb->setStyleSheet("border-image: url(:/images/res/contact_list.png) 0 0 0 0 stretch;");
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
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);
    addChatUserList();
    connect(ui->pushButton,&ClickedBtn::sig_changeIcon,this,&ChatDialog::changeicon);

    QPixmap pixmap(":/images/res/a8e6197d37215df2cd7f1e135ab18e93.jpg");
    ui->side_head_lb->setPixmap(pixmap); // 将图片设置到QLabel上
    QPixmap scaledPixmap = pixmap.scaled( ui->side_head_lb->size(), Qt::KeepAspectRatio); // 将图片缩放到label的大小
    ui->side_head_lb->setPixmap(scaledPixmap); // 将缩放后的图片设置到QLabel上
    ui->side_head_lb->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_lb->setProperty("state","normal");

    ui->side_chat_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    ui->side_contact_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);

    connect(ui->side_chat_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);
    connect(ui->lineEdit,&QLineEdit::textChanged,this,&ChatDialog::slot_text_changed);
    this->installEventFilter(this);
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

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::ClearLabelState(StateWidget *lb)
{
    for(auto & ele: _lb_list){
        if(ele == lb){
            continue;
        }

        ele->ClearState();
    }
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    // 实现点击位置的判断和处理逻辑
    // 先判断是否处于搜索模式，如果不处于搜索模式则直接返回
    if( _mode != ChatUIMode::SearchMode){
        return;
    }

    // 将鼠标点击位置转换为搜索列表坐标系中的位置
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPosition().toPoint());
    // 判断点击位置是否在聊天列表的范围内
    if (!ui->search_list->rect().contains(posInSearchList)) {
        // 如果不在聊天列表内，清空输入框
        ui->lineEdit->clear();
        ShowSearch(false);
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

void ChatDialog::slot_side_chat()
{
    qDebug()<< "receive side chat clicked";
    ClearLabelState(ui->side_chat_lb);

    _state = ChatUIMode::ChatMode;
    _mode = ChatUIMode::ChatMode;

    // 切换到聊天页面
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

    // 直接管理列表显隐
    ui->chat_user_list->show();
    ui->search_list->hide();
    ui->con_user_list->hide();
}

void ChatDialog::slot_side_contact()
{
    qDebug()<<"receive side contact clicked";
    ClearLabelState(ui->side_contact_lb);

    _state = ChatUIMode::ContactMode;
    _mode = ChatUIMode::ContactMode;

    // 切换到联系人页面
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);

    // 直接管理列表显隐
    ui->chat_user_list->hide();
    ui->search_list->hide();
    ui->con_user_list->show();
}

void ChatDialog::slot_text_changed(const QString &str)
{
    if(!str.isEmpty()){
        ShowSearch(true);
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent*mouseEvent=static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched,event);
}
