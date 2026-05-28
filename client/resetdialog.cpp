#include "resetdialog.h"
#include "ui_ResetDialog.h"



ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
}

ResetDialog::~ResetDialog()
{
    delete ui;
}
