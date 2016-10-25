
#include "client.h"
#include "mainwindow.h"
#include "stream.h"

extern MainWindow* window;


static  HSSocket *hs_socket_ptr = NULL;

static  QString bi,fi;

int send_response(QString msg, QString para)
{
    QJsonObject send;
    send.empty();
    send.insert("BI",bi);
    fi = "0";
    send.insert("FI",fi);
    send.insert("PN",msg);
    send.insert("PP",para);
    if(hs_socket_ptr)
        hs_socket_ptr->HSSocketAddSender("HMIAPP",send);
    else
        qDebug()<< "hs_socket_pty NULL";
}


void dealData(QJsonObject &obj)
{
    qDebug()<<"In EcoLink hssocket";
    QString rtstate = QString("RTState");
    QString msg;
    QString msg_para;
    QJsonObject send;
    if(obj.find("BI").value().type() == QJsonValue::String)
        bi = obj.find("BI").value().toString();
    else

    qDebug()<<"bi = "<< bi ;
    fi = obj.find("FI").value().toString();
    qDebug()<<"fi = "<< fi ;
    msg = obj.find("PN").value().toString();
    rtstate = msg + rtstate;
    qDebug()<<"msg = "<< msg ;
    msg_para = obj.find("PP").value().toString();
    qDebug()<<"msg_paras = "<< msg_para ;
    if(!QString::compare(msg,"startEcoLink")){
        window->show_ecolink(true);
        send_response(rtstate,"1"); //always ok
    }
    if(!QString::compare(msg,"exitEcoLink")){
         window->hide_ecolink(true);
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
    hs_socket_ptr = HSSocket::getInstance();
    if(hs_socket_ptr == NULL){
        DBG("hssocket failed\n");
        exit(-1);
    }
    /*
     * Create Ecolink Server.    */
    hs_socket_ptr->HSSocketCreate("EcoLink",conf_path,dealData);
}
