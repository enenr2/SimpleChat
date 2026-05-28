#ifndef RESETDIALONG_H
#define RESETDIALONG_H

#include "gloal.h"
#include <QDialog>

namespace Ui {
class ResetDialong;
}

class ResetDialong : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialong(QWidget *parent = nullptr);
    ~ResetDialong();

private slots:
    void slot_reset_mod_finish(ReqId id,QString res,ErrorCodes err);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void switchlogin();
private:
    Ui::ResetDialong *ui;

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVarifyValid();

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    void showTip(QString str,bool b_ok);

    void initHandlers();
    QMap<TipErr, QString> _tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
};

#endif // RESETDIALONG_H
