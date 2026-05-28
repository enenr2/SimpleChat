#include "adduseritem.h"
#include "ui_adduseritem.h"

AddUserItem::AddUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::AddUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::ADD_USER_TIP_ITEM);
    ui->add_tip->setStyleSheet("border-image: url(:/images/res/add_friend.png) 0 0 0 0 stretch;");

    ui->right_tip->setStyleSheet("border-image: url(:/images/res/right_tip.png) 0 0 0 0 stretch;");
}

AddUserItem::~AddUserItem()
{
    delete ui;
}
