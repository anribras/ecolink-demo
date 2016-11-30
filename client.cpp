
#include "client.h"
#include "mainwindow.h"
#include "stream.h"

extern MainWindow* window;


static  HSSocket hssocket;

static  int bi,fi;

int send_response(QString msg, QString para)
{
    QJsonObject send;
    send.empty();
    send.insert("BI",bi);
    send.insert("FI",fi);
    send.insert("PN",msg);
    send.insert("PP",para);
    hssocket.HSSocketAddSender("HMIAPP",send);

}

int send_response_navi(QString msg, QJsonObject para)
{
    QJsonObject send;
    send.empty();
    send.insert("BI",bi);
    send.insert("FI",fi);
    send.insert("PN",msg);
    send.insert("PP",para);
    hssocket.HSSocketAddSender("HMIAPP",send);
}


void dealData(QJsonObject &obj)
{
    qDebug()<<"In EcoLink hssocket";
    QString rtstate = QString("RTState");
    QString msg;
    QString msg_para;
    QJsonObject send;

    bi = obj.find("BI").value().toInt();
    qDebug()<<"bi = "<< bi ;
    fi = obj.find("FI").value().toInt();
    qDebug()<<"fi = "<< fi ;
    msg = obj.find("PN").value().toString();
    rtstate = msg + rtstate;
    qDebug()<<"msg = "<< msg ;

    msg_para = obj.find("PP").value().toString();
    qDebug()<<"msg_paras = "<< msg_para ;
    if(!QString::compare(msg,"startEcoLink")){
        //window->show_ecolink(true);
        send_response(rtstate,"1"); //always ok
    }
    if(!QString::compare(msg,"exitEcoLink")){
         //window->hide_ecolink(true);
         send_response(rtstate,"1"); //always ok
    }
    if(!QString::compare(msg,"setWndDisplay"))
    {
        if(!QString::compare(msg_para,"1"))
        {
            window->show_ecolink(true);
            qDebug()<<"show";
        }
        if(!QString::compare(msg_para,"2"))
        {
            window->hide_ecolink(true);
            qDebug()<<"hide";
        }
        send_response(rtstate,"1"); //always ok
    }
    if(!QString::compare(msg,"parkStateNotify"))
    {
        send_response(rtstate,"1");
    }
    if(!QString::compare(msg,"setLanguage"))
    {
        send_response(rtstate,"1");
    }
    if(!QString::compare(msg,"accStateNotify"))
    {
        send_response(rtstate,"1");
    }
    if(!QString::compare(msg,"hardkeyPressed"))
    {
        if(!QString::compare(msg_para,"back"))
        {
            qDebug()<<"press back";
        }
        if(!QString::compare(msg_para,"next"))
        {
            qDebug()<<"press next";
        }
        if(!QString::compare(msg_para,"prev"))
        {
            qDebug()<<"press prev";
        }
        send_response(rtstate,"1");
    }

}


void start_mainclient()
{
    char *conf_path = NULL;
    conf_path = getenv("HSSOCKETMODULEPATH");
    if(conf_path == NULL)
    {
        DBG("can't find env HSSOCKETMODULEPATH\n");
        exit(-1);
    }
    /*
     * Create Ecolink Server.    */
    hssocket.HSSocketCreate("EcoLink",conf_path,dealData);
}
