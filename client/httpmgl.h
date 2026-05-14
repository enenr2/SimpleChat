#ifndef HTTPMGL_H
#define HTTPMGL_H
#include"singletion.h"
#include<QString>
#include<QObject>
#include<QUrl>
#include<QNetworkAccessManager>
#include<QJsonObject>
#include<QJsonDocument>

class Httpmgl:public QObject,public singletion<Httpmgl>,public std::enable_shared_from_this<Httpmgl>
{
    Q_OBJECT;

public:
    ~Httpmgl();
    void PostHttpReq(QUrl url,QJsonObject json,ReqId req_id,Modules mod);
private:
    friend class singletion<Httpmgl>;
    Httpmgl();
    QNetworkAccessManager _manager;

private slots:
    void slot_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
signals:
    void sig_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_reset_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_login_finish(ReqId id,QString res,ErrorCodes err);
};

#endif // HTTPMGL_H
