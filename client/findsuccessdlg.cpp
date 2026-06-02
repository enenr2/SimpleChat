#include "findsuccessdlg.h"
#include "applyfriend.h"
#include "ui_findsuccessdlg.h"
#include<QDir>
FindSuccessDlg::FindSuccessDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindSuccessDlg)
    , _parent(parent)
{
    ui->setupUi(this);
    this->setStyleSheet(R"(
#ApplyFriend{
    border: 2px solid #f1f1f1;
    font-size: 14px;
    background: #f7f7f8;
}

#scrollArea{
    background: #f7f7f8;
    border: none;
}

#scrollcontent{
     background: #f7f7f8;
}

#scrollcontent #apply_lb{
    font-family: "Microsoft YaHei";
    font-size: 16px;
    font-weight: normal;
}

#apply_wid QLabel{
    color:rgb(140,140,140);
    font-size: 14px;
    font-family: "Microsoft YaHei";
    height: 25px;
}

#apply_wid #name_ed, #apply_wid #back_ed{
    border: 1px solid #f7f7f8;
    font-size: 14px;
    font-family: "Microsoft YaHei";
}

#apply_wid #lb_ed {
       border: none;
       font-size: 14px;
       font-family: "Microsoft YaHei";
}

#apply_wid #more_lb{
    border-image: url(:/res/arowdown.png);
}

#apply_wid #tipslb[state='normal'] {
    padding: 2px;
    background: #e1e1e1;
    color: #1e1e1e;
    border-radius: 10px;
}

#apply_wid #tipslb[state='hover'] {
    padding: 2px;
    background: #e1e1e1;
    color: #1e1e1e;
    border-radius: 10px;
}

#apply_wid #tipslb[state='pressed'] {
    padding: 2px;
    background: #e1e1e1;
    color: #48bf56;
    border-radius: 10px;
}

#apply_wid #tipslb[state='selected_normal'] {
    padding: 2px;
    background: #e1e1e1;
    color: #48bf56;
    border-radius: 10px;
}

#apply_wid #tipslb[state='selected_hover'] {
    padding: 2px;
    background: #e1e1e1;
    color: #48bf56;
    border-radius: 10px;
}

#apply_wid #tipslb[state='selected_pressed'] {
    padding: 2px;
    background: #e1e1e1;
    color: #1e1e1e;
    border-radius: 10px;
}

#input_tip_wid {
    background: #d3eaf8;
}

#apply_wid #FriendLabel {
    background: #daf6e7;
    color: #48bf56;
    border-radius: 10px;
}

#apply_wid #tip_lb {
    padding-left: 2px;
    color:rgb(153,153,153);
    font-size: 14px;
    font-family: "Microsoft YaHei";
}

#gridWidget {
   background: #fdfdfd;
}

#close_lb[state='normal'] {
    border-image: url(:/res/tipclose.png);
}

#close_lb[state='hover'] {
    border-image: url(:/res/tipclose.png);
}

#close_lb[state='pressed'] {
    border-image: url(:/res/tipclose.png);
}

#close_lb[state='select_normal'] {
    border-image: url(:/res/tipclose.png);
}

#close_lb[state='select_hover'] {
    border-image: url(:/res/tipclose.png);
}

#close_lb[state='select_pressed'] {
    border-image: url(:/res/tipclose.png);
}

#apply_sure_wid #sure_btn[state='normal']  {
    background: #f0f0f0;
    color: #2cb46e;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
}

#apply_sure_wid #sure_btn[state='hover'] {
    background: #d2d2d2;
    color: #2cb46e;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
}

#apply_sure_wid #sure_btn[state='press'] {
    background: #c6c6c6;
    color: #2cb46e;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
}

#apply_sure_wid #cancel_btn[state='normal']  {
    background: #f0f0f0;
    color: #2e2f30;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
}

#apply_sure_wid #cancel_btn[state='hover'] {
    background: #d2d2d2;
    color: #2e2f30;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
}

#apply_sure_wid #cancel_btn[state='press'] {
    background: #c6c6c6;
    color: #2e2f30;
    font-size: 16px; /* 设置字体大小 */
    font-family: "Microsoft YaHei"; /* 设置字体 */
    border-radius: 20px; /* 设置圆角 */
})");
    ui->add_friend_btn->setStyleSheet(R"(
    QPushButton {
        background-color: #07C160;   /* 微信品牌绿 */
        color: white;                /* 白色文字 */
        border: none;                /* 去掉边框 */
        border-radius: 4px;          /* 微圆角，可改为6px更贴近新版本 */
        padding: 8px 16px;           /* 上下左右内边距，控制按钮大小 */
        font-size: 14px;             /* 字体大小 */
        font-weight: bold;           /* 可选加粗，微信常用 */
    }
    QPushButton:hover {
        background-color: #06AD56;   /* 悬停时稍深 */
    }
    QPushButton:pressed {
        background-color: #059A4C;   /* 按下时更深 */
    }
)");
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path +
                                                QDir::separator() + "static"+QDir::separator()+"head_1.jpg");
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_lb->size(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal","hover","press");
    this->setModal(true);
}

FindSuccessDlg::~FindSuccessDlg()
{
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lb->setText(si->_name);
    _si = si;
}

void FindSuccessDlg::on_add_friend_btn_clicked()
{
    this->hide();
    //弹出加好友界面
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->SetSearchInfo(_si);
    applyFriend->setModal(true);
    applyFriend->show();
}
