#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include "qwidget.h"
#include <QObject>

class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(QWidget *parent = nullptr);

signals:
};

#endif // CHATITEMBASE_H
