#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include <QObject>
#include "bubbleframe.h"
class QTextEdit;
class TextBubble : public BubbleFrame
{
    Q_OBJECT
public:
    TextBubble(ChatRole role,const QString &text,QWidget*parent);
    void setPlainText(const QString &text);
    void adjustTextHeight();
    void initStyleSheet();
private:
    QTextEdit *m_pTextEdit;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // TEXTBUBBLE_H
