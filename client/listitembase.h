#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H
#include<QWidget>
#include <QObject>
#include"gloal.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);
    void SetItemType(ListItemType itemType);
    ListItemType GetItemType();
private:
    ListItemType _itemType;

signals:

};

#endif // LISTITEMBASE_H
