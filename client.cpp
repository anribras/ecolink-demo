
#include "client.h"
#include "mainwindow.h"
#include "stream.h"
#include "sdk.h"

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

QJsonObject ObjectFromString(const QString& in)
{
	QJsonParseError err;
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8(),&err);
    // check validity of the document
	if (err.error == QJsonParseError::NoError) {
		if(!doc.isNull()){
			if(doc.isObject()){
				obj = doc.object();        
			}
			else
				qDebug() << "Not an object" << endl;
		}else{
			qDebug() << "Invalid QJsonObject\n" << in << endl;
		} 
		//qDebug() << "recv QJsonObject" << obj << endl;
	} else {
		qDebug() << err.errorString() << endl;
	}

	return obj;
}

extern int fjson;
void navinfo_data(char* json, int b)
{
	//DBG("navi : %s\n",json);
	//DBG("navi len: %d\n",strlen(json));
	//DBG("length : %d\n",b);
	//char line = '\n';
	//write(fjson,json,strlen(json));
	//write(fjson ,&line,1);
	
	//QString str(json);
	QString str = QString::fromUtf8(json ,strlen(json));
	//qDebug()<< "QString json"<< str  ;
	QJsonObject obj = ObjectFromString(str);
	QJsonValue parameter = obj.find("Parameter").value();
	if(parameter.isObject()){
		//DBG("got Parameter jsonObject\n");
		QJsonObject objPara = parameter.toObject();
#if 0
		QJsonObject::Iterator it;
		for (it = objPara.begin(); it != objPara.end(); it++) {
			QString key = it.key();
			if(it.value().isString()){
				QString value = it.value().toString();
				//qDebug()<< "key"<< key << ":"<< value;  
			}
			if(it.value().isDouble()){
				int value = it.value().toInt();
				//qDebug()<< "key"<< key << ":"<< value;  
			}
		}
#else
	 	send_response_navi("navInfo", objPara);
#endif
	} else {
		DBG("parse QJsonObject Parameter error\n");
	}
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
	if(!QString::compare(msg,"startNavTrans")){
		register_navinfo(navinfo_data);
        send_response(rtstate,"1"); //always ok
    }
	if(!QString::compare(msg,"exitNavTrans")){
		unregister_navinfo();
        send_response(rtstate,"1"); //always ok
    }
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
