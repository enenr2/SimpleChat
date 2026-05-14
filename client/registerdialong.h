#ifndef REGISTERDIALONG_H
#define REGISTERDIALONG_H

#include <QDialog>
#include<gloal.h>
namespace Ui {
class RegisterDialong;
}

class RegisterDialong : public QDialog
{
    Q_OBJECT
private:
    void initHttphandlers();
    void showTip(QString str,bool b_ok);
    bool checkUserValid();
    bool checEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
public:
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);
    explicit RegisterDialong(QWidget *parent = nullptr);
    ~RegisterDialong();

private slots:
    void on_pushButton_2_clicked();
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void changeTip();
signals:
    void sigSwithLogin();
private:
    QMap<TipErr,QString> _tip_errs;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _hanlders;
    Ui::RegisterDialong *ui;
};

#endif // REGISTERDIALONG_H
