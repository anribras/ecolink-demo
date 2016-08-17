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


extern MainWindow* window;

void connect()
{
	window->set_mirror_status(1);
	DBG("callback ->%s\n",__FUNCTION__);
	enable_link_transfer();
	gstreamer_init(0);
	gstreamer_play();
}

void disconnect()
{
	DBG("callback ->%s\n",__FUNCTION__);
	gstreamer_pause();
	gstreamer_release();
	window->paint_image(FULL_PATH(init.jpg));
	window->show_ecolink();
	window->set_mirror_status(0);
	disable_link_transfer();
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
            window->show_ecolink();
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
			gstreamer_pause();
			gstreamer_release();
            window->paint_image(FULL_PATH(backgroundtip.jpg));
			window->show_ecolink();
			window->set_mirror_status(0);
			break;
		case AppForeground:
			DBG("AppForeground\n");
			window->set_mirror_status(1);
			if(gstreamer_get_status() != PLAYING){
				gstreamer_init(0);
				gstreamer_play();
			}
			break;
		case AndroidScreenOn:
			DBG("AndroidScreenOn\n");
			break;
		case AndroidScreenOff:
			DBG("AndroidScreenOff\n");
			break;
		case AndroidAppBackground:
			DBG("AndroidAppBackground\n");
			break;
		case AndroidDisconnected:
			DBG("AndroidDisconnected\n");
			break;
		default:
			DBG("not defined state st = %d\n",*st);
			break;
	}
}

void data_gotten(int* mode ,const char* buffer, int* size)
{
	//DBG("callback ->%s mode = %d\n",__FUNCTION__, *mode);
	get_steam_data_cb(buffer,size);
}


#endif

