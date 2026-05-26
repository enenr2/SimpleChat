#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include"gloal.h"
#include "statewidget.h"
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
    void AddLBGroup(StateWidget*lb);
    void ClearLabelState(StateWidget *lb);
    void handleGlobalMousePress(QMouseEvent*event);
    Ui::ChatDialog *ui;

    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*>_lb_list;
signals:
    void sig_CloseAll();
public slots:
    void changeicon(bool isok);
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString & str);
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // CHATDIALOG_H
