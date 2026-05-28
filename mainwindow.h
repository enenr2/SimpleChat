#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<longindialog.h>
#include<registerdialong.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ResetDialong;
class ChatDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    LonginDialog *longin;
    RegisterDialong *reg;
    ResetDialong *reset;
    ChatDialog* chat;
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
public slots:
    void slotswitchreg();
    void sigSwithLogin();
    void slotswitchReset();
    void slotswitchlogin2();
    void slotswitchchat();
    void CloseAll();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
