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


//#define SAVE_STREAM_TO_FILE

extern MainWindow* window;

static int test_fd;
static int test_fd1;

void connect()
{
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
	gstreamer_init(0);
    gstreamer_play();
}

void disconnect()
{
	DBG("callback ->%s\n",__FUNCTION__);
	window->paint_image(FULL_PATH(init.jpg));
	disable_link_transfer();
	gstreamer_pause();
	gstreamer_release();
	window->show_ecolink();
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
			DBG("AndroidAdbNotOpened\n");
            window->paint_image(FULL_PATH(help-android.jpg));
			break;
		case AndroidOnline:
			DBG("AndroidOnline\n");
            window->paint_image(FULL_PATH(connecting.jpg));
			break;
		case AndroidAppLunchFail:
			DBG("AndroidAppLunchFail\n");
            window->paint_image(FULL_PATH(install.jpg));
			break;
		case DeviceOffline:
			DBG("DeviceOffline\n");
            window->paint_image(FULL_PATH(init.jpg));
			break;
		case AndroidPlugoutWhenInstalling:
			DBG("AndroidPlugoutWhenInstalling\n");
			window->paint_image(FULL_PATH(init.jpg));
			break;
		case IosPlugIn:
			DBG("IosPlugIn\n");
			window->paint_image(FULL_PATH(connecting.jpg));
            window->show_ecolink();
			break;
		case IosPlugOut:
			DBG("IosPlugOut\n");
			window->paint_image(FULL_PATH(init.jpg));
			break;
		case IosAppNotReady:
			DBG("IosAppNotReady\n");
            window->paint_image(FULL_PATH(help-ios.jpg));
			break;
		case IosDisconnected:
			DBG("IosDisconnected\n");
			break;
		case IosAppBackground:
			DBG("IosAppBackground\n");
			//gstreamer_pause();
			//gstreamer_release();
			window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
            window->paint_image(FULL_PATH(backgroundtip.jpg));
			window->show_ecolink();
			break;
		case IosCallingin:
			DBG("IosCallingin\n");
			window->m_fbc.Alpha("/dev/fb0",1,255);//show pic
            window->paint_image(FULL_PATH(backgroundtip.jpg));
			window->show_ecolink();
			break;
		case AndroidAppForeground:
			DBG("AndroidAppForeground\n"); 
            window->disable_transparentBgd();
			break;
		case IosAppForeground:
			DBG("IosAppForeground\n"); 
			//if(gstreamer_get_status() != PLAYING){
				//gstreamer_init(0);
                //gstreamer_play();
			//}
            window->disable_transparentBgd();
			break;
		case AndroidScreenOn:
			DBG("AndroidScreenOn\n");
			break;
		case AndroidScreenOff:
			DBG("AndroidScreenOff\n");
			break;
		case AndroidAppBackground:
			DBG("AndroidAppBackground\n");
            window->enable_transparentBgd();
            break;
		case AndroidDisconnected:
			DBG("AndroidDisconnected\n");
			break;
		case IosConnected:
			DBG("IosConnected\n");
			window->paint_image(FULL_PATH(black.jpg));
			break;
		case AndroidConnected:
			window->paint_image(FULL_PATH(black.jpg));
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


#endif

