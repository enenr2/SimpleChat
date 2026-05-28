#ifndef CLICKEDONCELABEL_H
#define CLICKEDONCELABEL_H

#include <QLabel>
#include <QObject>
#include<QMouseEvent>

class ClickedOnceLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedOnceLabel(QWidget*parent=nullptr);

    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void clicked(QString);
};

#endif // CLICKEDONCELABEL_H
