#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include"gloal.h"
#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void addChatUserList();
private:
    void ShowSearch(bool bsearch);
    Ui::ChatDialog *ui;

    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
signals:
    void sig_CloseAll();
public slots:
    void changeicon(bool isok);
    void slot_loading_chat_user();
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};


#endif // CHATDIALOG_H
