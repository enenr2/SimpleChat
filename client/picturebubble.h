#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include <QMainWindow>
#include <QObject>
#include "bubbleframe.h"

class PictureBubble : public BubbleFrame
{
    Q_OBJECT
public:
    PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent);
};

#endif // PICTUREBUBBLE_H
