#include"gloal.h"

QString gate_url_prefix="";

void repolish(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
}

std::vector<QString> strs = {"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
    ":/images/res/1b27103fc30ee170ccc0f601dd20ca79.jpg",
    ":/images/res/771931129d0cb386762bc3c33742dff9.jpg",
    ":/images/res/59bc6e781a87beb3f61bc174b85a4ac7.jpg",
    ":/images/res/473e4cc830e8c5f795647248578fdade.jpg",
    ":/images/res/88843ce0e3e38325d108e44b10dbd243.jpg"
};

std::vector<QString> names = {
    "马自豪",
    "蒋玉辉",
    "薛佳乐",
    "支帝翔",
    "韩沅廷",
    "王雪冰",
    "王兆宇",
    "袁一凡"
};

std::function<QString(QString)> xorString=[](QString input){
    QString result=input;
    int lenght=input.length();
    lenght = lenght % 255;
    for(int i=0;i<lenght;++i)
        result[i]=QChar(static_cast<ushort>(input[i].unicode()^static_cast<ushort>(lenght)));
    return result;
};