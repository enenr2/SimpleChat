#include "mainwindow.h"

#include <QApplication>
#include<QFile>
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/qss/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open success");
        QString style=QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("open failed");
    }

    QString app_path = QCoreApplication::applicationDirPath();
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path +
                                                   QDir::separator() + fileName);

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();

    qDebug()<<config_path;
    qDebug()<<gate_host<<"  "<<gate_port;
    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    qDebug()<<gate_url_prefix;

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
