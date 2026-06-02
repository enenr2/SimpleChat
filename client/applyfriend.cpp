#include "applyfriend.h"
#include "qscrollbar.h"
#include "ui_applyfriend.h"
#include"usermgr.h"
#include<QGraphicsDropShadowEffect>
#include"tcpmgr.h"
ApplyFriend::ApplyFriend(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->name_ed->setPlaceholderText(tr("恋恋风辰"));
    ui->lb_ed->setPlaceholderText("搜索、添加标签");
    ui->back_ed->setPlaceholderText("燃烧的胸毛");

    // 对话框阴影
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 60));
    shadow->setOffset(0, 4);
    this->setGraphicsEffect(shadow);

    ui->lb_ed->SetMaxLength(21);
    ui->lb_ed->move(2, 2);
    ui->lb_ed->setFixedHeight(20);
    ui->lb_ed->setMaxLength(10);
    ui->input_tip_wid->hide();

    _tip_cur_point = QPoint(5, 5);

    _tip_data = { "同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                 "父与子学Python","nodejs开发指南","go 语言开发指南",
                 "游戏伙伴","金融投资","微信读书","拼多多拼友" };

    // 关闭按钮
    connect(ui->close_lb, &ClickedOnceLabel::clicked, this, [this](QString) {
        this->hide();
        deleteLater();
    });

    connect(ui->more_lb, &ClickedOnceLabel::clicked, this, &ApplyFriend::ShowMoreLabel);
    InitTipLbs();
    //链接输入标签回车事件
    connect(ui->lb_ed, &CustomizeEdit::returnPressed, this, &ApplyFriend::SlotLabelEnter);
    connect(ui->lb_ed, &CustomizeEdit::textChanged, this, &ApplyFriend::SlotLabelTextChange);
    connect(ui->lb_ed, &CustomizeEdit::editingFinished, this, &ApplyFriend::SlotLabelEditFinished);
    connect(ui->tip_lb, &ClickedOnceLabel::clicked, this, &ApplyFriend::SlotAddFirendLabelByClickTip);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->sure_btn->SetState("normal","hover","press");
    ui->cancel_btn->SetState("normal","hover","press");
    //连接确认和取消按钮的槽函数
    connect(ui->cancel_btn, &QPushButton::clicked, this, &ApplyFriend::SlotApplyCancel);
    connect(ui->sure_btn, &QPushButton::clicked, this, &ApplyFriend::SlotApplySure);
    QString styleSheet = R"(#ApplyFriend{
    border: 1px solid #e0e0e0;
    border-radius: 8px;
    font-size: 14px;
    background: #ffffff;
    }

#title_bar_widget {
    background: #fafafa;
    border-top-left-radius: 8px;
    border-top-right-radius: 8px;
}

#close_lb {
    font-size: 18px;
    color: #999;
    border-radius: 15px;
}
#close_lb:hover {
    color: #1e1e1e;
    background: #f0f0f0;
}

#friend_preview_widget {
    background: #f7f8fa;
}

#friend_name_lb {
    font-size: 16px;
    font-weight: bold;
    color: #1e1e1e;
}

    #scrollArea{
    background: #ffffff;
    border: none;
    }

    #scrollAreaWidgetContents{
     background: #ffffff;
    }

    #apply_wid QLabel{
    color:rgb(120,120,120);
    font-size: 14px;
    font-family: "Microsoft YaHei";
    height: 25px;
    }

    #apply_wid #name_ed, #apply_wid #back_ed{
    border: 1px solid #e0e0e0;
    border-radius: 4px;
    padding: 4px 8px;
    font-size: 14px;
    font-family: "Microsoft YaHei";
    background: #ffffff;
    }
    #apply_wid #name_ed:focus, #apply_wid #back_ed:focus {
    border: 1px solid #07C160;
    }

    #apply_wid #lb_ed {
       border: 1px solid #e0e0e0;
       border-radius: 4px;
       padding: 2px 6px;
       font-size: 14px;
       font-family: "Microsoft YaHei";
       background: #ffffff;
}

#apply_wid #more_lb{
    border-image: url(:/res/arowdown.png);
}

#apply_wid #tipslb[state='normal'] {
    padding: 4px 10px;
    background: #e8e8e8;
    color: #333;
    border-radius: 12px;
    font-size: 12px;
}

#apply_wid #tipslb[state='hover'] {
    padding: 4px 10px;
    background: #d0d0d0;
    color: #333;
    border-radius: 12px;
    font-size: 12px;
}

#apply_wid #tipslb[state='pressed'] {
    padding: 4px 10px;
    background: #d0d0d0;
    color: #48bf56;
    border-radius: 12px;
    font-size: 12px;
}

#apply_wid #tipslb[state='selected_normal'] {
    padding: 4px 10px;
    background: #daf6e7;
    color: #07C160;
    border-radius: 12px;
    font-size: 12px;
}

#apply_wid #tipslb[state='selected_hover'] {
    padding: 4px 10px;
    background: #c0ebd0;
    color: #07C160;
    border-radius: 12px;
    font-size: 12px;
}

#apply_wid #tipslb[state='selected_pressed'] {
    padding: 4px 10px;
    background: #c0ebd0;
    color: #1e1e1e;
    border-radius: 12px;
    font-size: 12px;
}

#input_tip_wid {
    background: #f0f5fb;
    border-radius: 4px;
    padding: 4px;
}

#apply_wid #FriendLabel {
    background: #daf6e7;
    color: #07C160;
    border-radius: 10px;
    padding: 2px 8px;
    font-size: 12px;
}

#apply_wid #tip_lb {
    padding-left: 4px;
    color:rgb(153,153,153);
    font-size: 13px;
    font-family: "Microsoft YaHei";
}

#gridWidget {
   background: #fafafa;
   border: 1px solid #eee;
   border-radius: 4px;
}

#apply_sure_wid #sure_btn[state='normal']  {
    background: #07C160;
    color: #ffffff;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: none;
}

#apply_sure_wid #sure_btn[state='hover'] {
    background: #06AD56;
    color: #ffffff;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: none;
}

#apply_sure_wid #sure_btn[state='press'] {
    background: #059A4C;
    color: #ffffff;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: none;
}

#apply_sure_wid #cancel_btn[state='normal']  {
    background: #f5f5f5;
    color: #666;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: 1px solid #e0e0e0;
}

#apply_sure_wid #cancel_btn[state='hover'] {
    background: #e8e8e8;
    color: #333;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: 1px solid #d0d0d0;
}

#apply_sure_wid #cancel_btn[state='press'] {
    background: #dcdcdc;
    color: #333;
    font-size: 15px;
    font-family: "Microsoft YaHei";
    border-radius: 20px;
    border: 1px solid #d0d0d0;
}

#scrollArea QScrollBar:vertical {
    background: transparent;
    width: 6px;
    margin: 0;
}
#scrollArea QScrollBar::handle:vertical {
    background: #c0c0c0;
    min-height: 30px;
    border-radius: 3px;
}
#scrollArea QScrollBar::handle:vertical:hover {
    background: #a0a0a0;
}
#scrollArea QScrollBar::add-line:vertical,
#scrollArea QScrollBar::sub-line:vertical {
    height: 0;
    border: none;
    background: transparent;
}
#scrollArea QScrollBar::add-page:vertical,
#scrollArea QScrollBar::sub-page:vertical {
    background: none;
}

#apply_wid #name_ed, #apply_wid #back_ed, #apply_wid #lb_ed {
    color: #333;
})";
    this->setStyleSheet(styleSheet);
}

ApplyFriend::~ApplyFriend()
{
    qDebug()<< "ApplyFriend destruct";
    delete ui;
}

void ApplyFriend::InitTipLbs()
{
    int lines = 1;
    for(int i = 0; i < _tip_data.size(); i++){

        auto* lb = new ClichedLabel(ui->lb_list);
        //lb->SetState("normal", "hover", "pressed", "selected_normal",
        //"selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClichedLabel::clicked, this, [this, lb]() {
            SlotChangeFriendLabelByTip(lb->text(), lb->GetCurState());
        });

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->lb_list->width()) {
            lines++;
            if (lines > 2) {
                delete lb;
                return;
            }

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        auto next_point = _tip_cur_point;

        AddTipLbs(lb, _tip_cur_point,next_point, textWidth, textHeight);

        _tip_cur_point = next_point;
    }

}

void ApplyFriend::AddTipLbs(ClichedLabel* lb, QPoint cur_point, QPoint& next_point, int text_width, int text_height)
{
    lb->move(cur_point);
    lb->show();
    _add_labels.insert(lb->text(), lb);
    _add_label_keys.push_back(lb->text());
    next_point.setX(lb->pos().x() + text_width + 15);
    next_point.setY(lb->pos().y());
}

bool ApplyFriend::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->scrollArea && event->type() == QEvent::Enter)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if (obj == ui->scrollArea && event->type() == QEvent::Leave)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(obj, event);
}

void ApplyFriend::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    _si = si;
    auto apply = UserMgr::GetInstance();
    auto applyname=apply->GetName();
    auto bakname = si->_name;
    ui->name_ed->setText(applyname);
    ui->back_ed->setText(bakname);

    // 填充好友预览区
    ui->friend_name_lb->setText(si->_name);
    QPixmap pixmap(":/images/res/head_1.jpg");
    ui->friend_avatar_lb->setPixmap(pixmap.scaled(
        ui->friend_avatar_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ApplyFriend::ShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->more_lb_wid->hide();

    ui->lb_list->setFixedWidth(325);
    _tip_cur_point = QPoint(5, 5);
    auto next_point = _tip_cur_point;
    int textWidth;
    int textHeight;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.horizontalAdvance(added_lb->text()); // 获取文本的宽度
        textHeight = fontMetrics.height(); // 获取文本的高度

        if(_tip_cur_point.x() +textWidth + tip_offset > ui->lb_list->width()){
            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
        }
        added_lb->move(_tip_cur_point);

        next_point.setX(added_lb->pos().x() + textWidth + 15);
        next_point.setY(_tip_cur_point.y());

        _tip_cur_point = next_point;

    }

    //添加未添加的
    for(int i = 0; i < _tip_data.size(); i++){
        auto iter = _add_labels.find(_tip_data[i]);
        if(iter != _add_labels.end()){
            continue;
        }

        auto* lb = new ClichedLabel(ui->lb_list);
        // lb->SetState("normal", "hover", "pressed", "selected_normal",
        //              "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClichedLabel::clicked, this, [this, lb]() {
            SlotChangeFriendLabelByTip(lb->text(), lb->GetCurState());
        });

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->lb_list->width()) {

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        next_point = _tip_cur_point;

        AddTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);

        _tip_cur_point = next_point;

    }

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();
    //ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+diff_height);
}

void ApplyFriend::resetLabels()
{
    auto max_width = ui->gridWidget->width();
    auto label_height = 0;
    for(auto iter = _friend_labels.begin(); iter != _friend_labels.end(); iter++){
        //todo... 添加宽度统计
        if( _label_point.x() + iter.value()->width() > max_width) {
            _label_point.setY(_label_point.y()+iter.value()->height()+6);
            _label_point.setX(2);
        }

        iter.value()->move(_label_point);
        iter.value()->show();

        _label_point.setX(_label_point.x()+iter.value()->width()+2);
        _label_point.setY(_label_point.y());
        label_height = iter.value()->height();
    }

    if(_friend_labels.isEmpty()){
        ui->lb_ed->move(_label_point);
        return;
    }

    if(_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->gridWidget->width()){
        ui->lb_ed->move(2,_label_point.y()+label_height+6);
    }else{
        ui->lb_ed->move(_label_point);
    }
}

void ApplyFriend::addLabel(QString name)
{
    if (_friend_labels.find(name) != _friend_labels.end()) {
        return;
    }

    auto tmplabel = new FriendLabel(ui->gridWidget);
    tmplabel->SetText(name);
    tmplabel->setObjectName("FriendLabel");

    auto max_width = ui->gridWidget->width();
    //todo... 添加宽度统计
    if (_label_point.x() + tmplabel->width() > max_width) {
        _label_point.setY(_label_point.y() + tmplabel->height() + 6);
        _label_point.setX(2);
    }
    else {

    }


    tmplabel->move(_label_point);
    tmplabel->show();
    _friend_labels[tmplabel->Text()] = tmplabel;
    _friend_label_keys.push_back(tmplabel->Text());

    connect(tmplabel, &FriendLabel::sig_close, this, &ApplyFriend::SlotRemoveFriendLabel);

    _label_point.setX(_label_point.x() + tmplabel->width() + 2);

    if (_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->gridWidget->width()) {
        ui->lb_ed->move(2, _label_point.y() + tmplabel->height() + 2);
    }
    else {
        ui->lb_ed->move(_label_point);
    }

    ui->lb_ed->clear();

    if (ui->gridWidget->height() < _label_point.y() + tmplabel->height() + 2) {
        ui->gridWidget->setFixedHeight(_label_point.y() + tmplabel->height() * 2 + 2);
    }
}

void ApplyFriend::SlotLabelEnter()
{
    if(ui->lb_ed->text().isEmpty()){
        return;
    }

    auto text = ui->lb_ed->text();
    addLabel(ui->lb_ed->text());

    ui->input_tip_wid->hide();
    auto find_it = std::find(_tip_data.begin(), _tip_data.end(), text);
    //找到了就只需设置状态为选中即可
    if (find_it == _tip_data.end()) {
        _tip_data.push_back(text);
    }

    //判断标签展示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if (find_add != _add_labels.end()) {
        find_add.value()->SetCurState(ClickLbState::Selected);
        return;
    }

    //标签展示栏也增加一个标签, 并设置绿色选中
    auto* lb = new ClichedLabel(ui->lb_list);
    // lb->SetState("normal", "hover", "pressed", "selected_normal",
    //              "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClichedLabel::clicked, this, [this, lb]() {
        SlotChangeFriendLabelByTip(lb->text(), lb->GetCurState());
    });
    qDebug() << "ui->lb_list->width() is " << ui->lb_list->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth + tip_offset + 3 > ui->lb_list->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

    AddTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurState(ClickLbState::Selected);

    //ui->scrollArea->setFixedHeight(ui->scrollcontent->height() + diff_height);
}

void ApplyFriend::SlotRemoveFriendLabel(QString name)
{
    qDebug() << "receive close signal";

    _label_point.setX(2);
    _label_point.setY(6);

    auto find_iter = _friend_labels.find(name);

    if(find_iter == _friend_labels.end()){
        return;
    }

    auto find_key = _friend_label_keys.end();
    for(auto iter = _friend_label_keys.begin(); iter != _friend_label_keys.end();
         iter++){
        if(*iter == name){
            find_key = iter;
            break;
        }
    }

    if(find_key != _friend_label_keys.end()){
        _friend_label_keys.erase(find_key);
    }


    delete find_iter.value();

    _friend_labels.erase(find_iter);

    resetLabels();

    auto find_add = _add_labels.find(name);
    if(find_add == _add_labels.end()){
        return;
    }

    find_add.value()->ResetNormalState();
}

//点击标已有签添加或删除新联系人的标签
void ApplyFriend::SlotChangeFriendLabelByTip(QString lbtext, ClickLbState state)
{
    auto find_iter = _add_labels.find(lbtext);
    if(find_iter == _add_labels.end()){
        return;
    }

    if(state == ClickLbState::Selected){
        //编写添加逻辑
        addLabel(lbtext);
        return;
    }

    if(state == ClickLbState::Normal){
        //编写删除逻辑
        SlotRemoveFriendLabel(lbtext);
        return;
    }

}

void ApplyFriend::SlotLabelTextChange(const QString& text)
{
    if (text.isEmpty()) {
        ui->tip_lb->setText("");
        ui->input_tip_wid->hide();
        return;
    }

    auto iter = std::find(_tip_data.begin(), _tip_data.end(), text);
    if (iter == _tip_data.end()) {
        auto new_text = add_prefix + text;
        ui->tip_lb->setText(new_text);
        ui->input_tip_wid->show();
        return;
    }
    ui->tip_lb->setText(text);
    ui->input_tip_wid->show();
}

void ApplyFriend::SlotLabelEditFinished()
{
    ui->input_tip_wid->hide();
}

void ApplyFriend::SlotAddFirendLabelByClickTip(QString text)
{
    int index = text.indexOf(add_prefix);
    if (index != -1) {
        text = text.mid(index + add_prefix.length());
    }
    addLabel(text);

    auto find_it = std::find(_tip_data.begin(), _tip_data.end(), text);
    //找到了就只需设置状态为选中即可
    if (find_it == _tip_data.end()) {
        _tip_data.push_back(text);
    }

    //判断标签展示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if (find_add != _add_labels.end()) {
        find_add.value()->SetCurState(ClickLbState::Selected);
        return;
    }

    //标签展示栏也增加一个标签, 并设置绿色选中
    auto* lb = new ClichedLabel(ui->lb_list);
    lb->SetState("normal", "hover", "pressed", "selected_normal",
                 "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClichedLabel::clicked, this, [this, lb]() {
        SlotChangeFriendLabelByTip(lb->text(), lb->GetCurState());
    });
    qDebug() << "ui->lb_list->width() is " << ui->lb_list->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth+ tip_offset+3 > ui->lb_list->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

    AddTipLbs(lb, _tip_cur_point, next_point, textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurState(ClickLbState::Selected);

    //ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+ diff_height );
}

void ApplyFriend::SlotApplyCancel()
{
    qDebug() << "Slot Apply Cancel";
    this->hide();
    deleteLater();
}

void ApplyFriend::SlotApplySure()
{
    qDebug()<<"Slot Apply Sure called, touid=" << _si->_uid;
    QJsonObject jsonobj;
    auto uid = UserMgr::GetInstance()->GetUid();
    jsonobj["uid"] = uid;
    auto name = ui->name_ed->text();
    if(name.isEmpty()){
        name = ui->name_ed->placeholderText();
    }
    jsonobj["applyname"] = name;
    auto bakname = ui->back_ed->text();
    if(bakname.isEmpty()){
        bakname = ui->back_ed->placeholderText();
    }
    jsonobj["bakname"] = bakname;
    jsonobj["touid"] = _si->_uid;
    QJsonDocument doc(jsonobj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_ADD_FRIEND_REQ, jsonData);
    this->hide();
    deleteLater();
}
