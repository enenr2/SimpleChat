#ifndef LONGINDIALOG_H
#define LONGINDIALOG_H

#include "gloal.h"
#include <QDialog>

namespace Ui {
class LonginDialog;
}

class LonginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LonginDialog(QWidget *parent = nullptr);
    ~LonginDialog();
signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);
private slots:
    void slot_forget_pwd();
    void on_pushButton_2_clicked();
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);
    void slot_tcp_con_finish(bool bsuccess);
private:
    Ui::LonginDialog *ui;
    bool checkUserValid();
    bool checkPwdValid();
    void enableBtn(bool ok);
    void initHttpHandlers();

    int _uid;
    QString _token;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
};

#endif // LONGINDIALOG_H
