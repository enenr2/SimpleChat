#include"gloal.h"

QString gate_url_prefix="";

void repolish(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
}

std::function<QString(QString)> xorString=[](QString input){
    QString result=input;
    int lenght=input.length();
    lenght = lenght % 255;
    for(int i=0;i<lenght;++i)
        result[i]=QChar(static_cast<ushort>(input[i].unicode()^static_cast<ushort>(lenght)));
    return result;
};