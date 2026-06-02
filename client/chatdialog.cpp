#include "chatdialog.h"
#include "conuseritem.h"
#include "ui_chatdialog.h"
#include"customizeedit.h"
#include<QRandomGenerator>
#include"chatuserwid.h"
#include"loadingdlg.h"
#include"chatuserlist.h"
#include<QIcon>
#include<QPixmap>
#include<QMouseEvent>
#include"tcpmgr.h"
#include"usermgr.h"


ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog),_mode(ChatUIMode::ChatMode),_state(ChatUIMode::ChatMode),_b_loading(false),_cur_chat_uid(0)
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
    ui->search_list->SetSearchEdit(ui->lineEdit);
    SetSelectChatItem();
    SetSelectChatPage();
    //连接申请添加好友信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_apply_friend);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend,this,&ChatDialog::slot_add_friend);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp,this,&ChatDialog::slot_auth_rsp);
    connect(ui->search_list,&SearchList::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item);
    connect(ui->con_user_list,&ContactUserList::sig_loading_contact_user,this,&ChatDialog::slot_loading_contact_user);
    connect(ui->con_user_list,&ContactUserList::sig_switch_friend_info_page,this,&ChatDialog::slot_friend_info_page);
    connect(ui->friend_info_page,&FriendInfoPage::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item_from_infopage);
    connect(ui->chat_user_list,&QListWidget::itemClicked,this,&ChatDialog::slot_item_clicked);
    connect(ui->chat_page,&ChatPage::sig_append_send_chat_msg,this,&ChatDialog::slot_append_send_chat_msg);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_text_chat_msg,this,&ChatDialog::slot_text_chat_msg);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

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
        auto user_info=std::make_shared<UserInfo>(0,names[name_i],names[name_i],heads[head_i],0,strs[str_i]);
        chat_user_wid->SetInfo(user_info);
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
    loadMoreChatUser();
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

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply>apply)
{
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
             << apply->_name << " desc is " << apply->_desc;

    bool b_already = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
    if(b_already){
        return;
    }

    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->AddNewApply(apply);
}

void ChatDialog::slot_add_friend(std::shared_ptr<AuthInfo>auth_info)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid
             << " name is " << auth_info->_name << " nick is " << auth_info->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_info);

    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_info->_uid, item);

	// 自动跳转到聊天界面
	auto si = std::make_shared<SearchInfo>(auth_info->_uid, auth_info->_name,
		auth_info->_nick, "", auth_info->_sex, auth_info->_icon);
	emit ui->search_list->sig_jump_chat_item(si);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid
             << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_rsp);
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);

	// 自动跳转到聊天界面
	auto si = std::make_shared<SearchInfo>(auth_rsp->_uid, auth_rsp->_name,
		auth_rsp->_nick, "", auth_rsp->_sex, auth_rsp->_icon);
	emit ui->search_list->sig_jump_chat_item(si);
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item " ;
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();
}

void ChatDialog::slot_friend_info_page(std::shared_ptr<UserInfo>user_info)
{
    qDebug()<<"receive switch friend info page sig";
    _last_widget=ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_info_page);
    ui->friend_info_page->SetInfo(user_info);
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent*mouseEvent=static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched,event);
}


void ChatDialog::SetSelectChatItem(int uid)
{
    if(ui->chat_user_list->count() <= 0)
    {
        return;
    }

    if(uid == 0)
    {
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chat_user_list->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if(!con_item){
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;

        return;
    }
}



void ChatDialog::SetSelectChatPage(int uid)
{
    if( ui->chat_user_list->count() <= 0){
        return;
    }

    if (uid == 0) {
        auto item = ui->chat_user_list->item(0);
        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(item);
        if (!widget) {
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        auto friend_info = UserMgr::GetInstance()->GetFriendById(user_info->_uid);
        if (friend_info) {
            user_info->_chat_msgs = friend_info->_chat_msgs;
        }
        ui->chat_page->SetUserInfo(user_info);
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }

    //转为widget
    QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
    if(!widget){
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if(!con_item){
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        auto friend_info = UserMgr::GetInstance()->GetFriendById(user_info->_uid);
        if (friend_info) {
            user_info->_chat_msgs = friend_info->_chat_msgs;
        }
        ui->chat_page->SetUserInfo(user_info);

        return;
    }
}



void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }
}



void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::GetInstance()->GetConListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto *chat_user_wid = new ConUserItem();
            chat_user_wid->SetInfo(friend_ele->_uid,friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->con_user_list->addItem(item);
            ui->con_user_list->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateContactLoadedCount();
    }
}



void ChatDialog::slot_loading_contact_user()
{
    qDebug()<<"slot load contact user";
    if(_b_loading){
        return;
    }
    _b_loading=true;
    LoadingDlg*loadingDialog=new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug()<<"加载新的数据链";
    loadMoreConUser();
    loadingDialog->deleteLater();
    _b_loading=false;
}



void ChatDialog::slot_switch_apply_friend_page()
{
    qDebug()<<"receive switch friend page sig";
    _last_widget=ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_info_page);
}



void ChatDialog::slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info)
{
    qDebug() << "slot jump chat item " ;
    auto find_iter = _chat_items_added.find(user_info->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << user_info->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(user_info->_uid);
        //更新聊天界面信息
        SetSelectChatPage(user_info->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(user_info->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(user_info->_uid);
    //更新聊天界面信息
    SetSelectChatPage(user_info->_uid);
    slot_side_chat();
}



void ChatDialog::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = ui->chat_user_list->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM
        || itemType == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }


    if(itemType == ListItemType::CHAT_USER_ITEM){
        // 创建对话框，提示用户
        qDebug()<< "contact user item clicked ";

        auto chat_wid = qobject_cast<ChatUserWid*>(customItem);
        auto user_info = chat_wid->GetUserInfo();
        //跳转到聊天界面
        ui->chat_page->SetUserInfo(user_info);
        _cur_chat_uid = user_info->_uid;
        return;
    }
}



void ChatDialog::slot_append_send_chat_msg(std::shared_ptr<TextChatData> msgdata)
{
    if (_cur_chat_uid == 0) {
        return;
    }

    auto find_iter = _chat_items_added.find(_cur_chat_uid);
    if (find_iter == _chat_items_added.end()) {
        return;
    }

    //转为widget
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if (!widget) {
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem) {
        qDebug() << "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == CHAT_USER_ITEM) {
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        user_info->_chat_msgs.push_back(msgdata);
        std::vector<std::shared_ptr<TextChatData>>msg;
        msg.push_back(msgdata);
        UserMgr::GetInstance()->AppendFriendChatMsg(_cur_chat_uid,msg);
        return;
    }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    auto find_iter = _chat_items_added.find(msg->_from_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "set chat item msg, uid is " << msg->_from_uid;
        QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatUserWid*>(widget);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录
        UpdateChatMsg(msg->_chat_msgs);
        UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    //查询好友信息
    auto fi_ptr = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
    chat_user_wid->SetInfo(fi_ptr);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    chat_user_wid->updateLastMsg(msg->_chat_msgs);
    UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(msg->_from_uid, item);

}



void ChatDialog::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData> > msgdata)
{
    for(auto & msg : msgdata){
        if(msg->_from_uid != _cur_chat_uid){
            break;
        }

        ui->chat_page->AppendChatMsg(msg);
    }
}

