#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include"ChatItemBase.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
protected:
    void paintEvent(QPaintEvent*event);
private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatPage *ui;
    ChatItemBase*pChatItem;
};

#endif // CHATPAGE_H
