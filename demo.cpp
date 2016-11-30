/**
 * @file demo.cpp
 * @brief 
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-08-12
 */
#ifndef __DEMO_C
#define __DEMO_C

#include <stdio.h>
#include "debug.h"
#include "stream.h"
#include "mainwindow.h"
#include "sdk.h"
#include "client.h"


//#define SAVE_STREAM_TO_FILE

extern MainWindow* window;

static int test_fd;
static int test_fd1;
static int device = NULL;

typedef enum{
	ANDROID = 0,
	IOS
} Device;


void touch_down_callback(int x, int y)
{
	DBG("screen on\n");
	extra_event(PhoneScreenOn);
}

static int fjson;
void connect()
{
	//fjson  = open("test.json",O_RDWR | O_CREAT |O_TRUNC, 0644);
	//if(fjson < 0 ){
		//DBG("open test.json error");
		//exit(-1);
	//}


	DBG("callback ->%s\n",__FUNCTION__);
	enable_link_transfer();
#ifdef SAVE_STREAM_TO_FILE
	test_fd = open("test.h264",O_RDWR | O_CREAT,0664);
	if(test_fd < 0) {
		DBG("test.h264 open fail\n");
		return;
	} else {
		DBG("test.h264 open ok\n");
	}
	test_fd1 = open("size.txt",O_RDWR | O_CREAT,0664);
	if(test_fd1 < 0) {
		DBG("size.txt open fail\n");
		return;
	} else {
		DBG("size.txt open ok\n");
	}
#endif
    if(gstreamer_get_status() != PLAYING){
		if(device == ANDROID)
			gstreamer_init(STREAM);
		else
			gstreamer_init(JPEG);
        gstreamer_play();
    }
    if(window->get_wnd_display_status() == true)
    {

    }
    else
    {
        DBG("connected but stream no need show\n");
        window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
    }

    send_response("devConnectState","devConnected");
}

void disconnect()
{
    window->hide_ecolink(true);
    send_response("devConnectState","devDisconnected");
	DBG("callback ->%s\n",__FUNCTION__);
	window->paint_image(FULL_PATH(init.jpg));
	disable_link_transfer();
	if(gstreamer_get_status() == PLAYING){
		gstreamer_pause();
		gstreamer_release();
	}
#ifdef SAVE_STREAM_TO_FILE
        if(test_fd)
            close(test_fd);
		if(test_fd1)
            close(test_fd1);
#endif
}

/**
 * @brief st_changed 
 * handle changing state here
 * @param st
 */
void st_changed(int* st)
{
	//DBG("callback ->%s\n",__FUNCTION__);
	/*not all the cases need to be processed
	 *acturally most of processing are pretty simple
	 * */
	switch(*st)
	{
		case AndroidAdbNotOpened:
            send_response("devConnectState","devAdbNotOpened");
			DBG("AndroidAdbNotOpened\n");
            window->paint_image(FULL_PATH(help-android.jpg));
			break;
        case AndroidOnline:
            send_response("devConnectState","devPlugin");
			window->show_ecolink(true);
            //window->show_ecolink();
            DBG("AndroidOnline\n");
			device = ANDROID;
            window->paint_image(FULL_PATH(connecting.jpg));
			break;
		case AndroidAppLunchFail:
			DBG("AndroidAppLunchFail\n");
            window->paint_image(FULL_PATH(install.jpg));
			break;
		case DeviceOffline:
			DBG("DeviceOffline\n");
            send_response("devConnectState","devPlugout");
            window->paint_image(FULL_PATH(init.jpg));
            //window->hide_ecolink(true);
			break;
		case AndroidPlugoutWhenInstalling:
			DBG("AndroidPlugoutWhenInstalling\n");
			window->paint_image(FULL_PATH(init.jpg));
			break;
		case IosPlugIn:
            send_response("devConnectState","devPlugin");
			window->show_ecolink(true);
			DBG("IosPlugIn\n");
			device = IOS;
			window->paint_image(FULL_PATH(connecting.jpg));
            //window->show_ecolink();
			break;
		case IosPlugOut:
			DBG("IosPlugOut\n");
			window->paint_image(FULL_PATH(init.jpg));
			break;
		case IosAppNotReady:
			DBG("IosAppNotReady\n");
            send_response("devConnectState","devIosNotReady");
            window->paint_image(FULL_PATH(help-ios.jpg));
			break;
		case IosDisconnected:
			DBG("IosDisconnected\n");
			break;
		case IosAppBackground:
			DBG("IosAppBackground\n");
            window->paint_image(FULL_PATH(backgroundtip.jpg));
			usleep(100000);
            //window->show_ecolink();
#if 0
			if(gstreamer_get_status() == PLAYING){
                gstreamer_pause();
                gstreamer_release();
			}
#else
            window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
#endif
			break;
        case IosCallingin:
			DBG("IosCallingin\n");
            window->paint_image(FULL_PATH(backgroundtip.jpg));
			usleep(100000);
            //window->show_ecolink();
#if 0
			if(gstreamer_get_status() == PLAYING){
                gstreamer_pause();
                gstreamer_release();
			}
#else
            window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
#endif
			break;
		case AndroidAppForeground:
			DBG("AndroidAppForeground\n"); 
            //window->disable_transparentBgd();
			break;
		case IosAppForeground:
            DBG("IosAppForeground\n");
#if 0
            if(gstreamer_get_status() != PLAYING){
                gstreamer_init(JPEG);
                gstreamer_play();
            }
			if(window->get_wnd_display_status() == false){
				DBG("connected but stream no need show\n");
				window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
			}
#else
            window->m_fbc.Alpha("/dev/fb0",1,gAlpha);//show stream
#endif
			break;
		case AndroidScreenOn:

			DBG("AndroidScreenOn\n");

			unregister_touchevent(TouchDown);
#if 0
			if(gstreamer_get_status() != PLAYING){
                gstreamer_init(STREAM);
                gstreamer_play();
            }
			if(window->get_wnd_display_status() == false){
                DBG("window hide no need show stream");
                window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
            }
#else
			window->m_fbc.Alpha("/dev/fb0",1,gAlpha);//show stream
#endif
            
			break;
		case AndroidScreenOff:
			window->paint_image(FULL_PATH(screenoff.jpg));
			usleep(100000);
#if 0
			if(gstreamer_get_status() == PLAYING){
                gstreamer_pause();
                gstreamer_release();
			}
#else
			window->m_fbc.Alpha("/dev/fb0",1,255);//show stream
#endif
			DBG("AndroidScreenOff\n");
			register_touchevent(TouchDown,touch_down_callback);
			break;
		case AndroidAppBackground:
			DBG("AndroidAppBackground\n");
            //window->enable_transparentBgd();
            break;
		case AndroidDisconnected:
			DBG("AndroidDisconnected\n");
			break;
		case IosConnected:
			DBG("IosConnected\n");
			window->paint_image(FULL_PATH(black.png));
			break;
		case AndroidConnected:
			window->paint_image(FULL_PATH(black.png));
			DBG("AndroidConnected\n");
			break;
		default:
			DBG("not defined state st = %d\n",*st);
			break;
	}
}

void data_gotten(int* mode ,char* buffer, int* size)
{
	//DBG("callback ->%s mode = %d size = %d\n",__FUNCTION__, *mode,*size);
#ifdef SAVE_STREAM_TO_FILE
	char length_str[10] = {'\0'};
	int len;
	sprintf(length_str,"%d\n",*size);
    write(test_fd1,length_str,strlen(length_str));
    write(test_fd,buffer,*size);
#endif
	get_steam_data_cb(buffer,*size);
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
		qDebug() << "recv QJsonObject" << obj << endl;
	} else {
		qDebug() << err.errorString() << endl;
	}

	return obj;
}

void navinfo_data(char* json, int b)
{
	//DBG("navi : %s\n",json);
	//DBG("length : %d\n",b);
	//char line = '\n';
	//write(fjson,json,strlen(json));
	//write(fjson ,&line,1);
	QString str(json);
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


#endif

