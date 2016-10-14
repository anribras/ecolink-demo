/**
 * @file ecolink_sdk.h
 * @brief 
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-08-11
 */
#ifndef __ECOLINK_SDK_H
#define __ECOLINK_SDK_H


#define IN
#define OUT
#define NULL 0

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	/*
	 * Mostly show init.jpg 
	 * */
	DeviceOffline = 0,

	/*
	 * Mostly show connecting.jpg 
	 * */
	AndroidOnline, 
	
	/*
	 * Mostly show install.jpg 
	 * */
	AndroidAppLunchFail,

	/*
	 * Mostly show init.jpg 
	 * */
	AndroidPlugoutWhenInstalling = 3,

	/*
	 * Mostly show android-help.jpg 
	 * */
	AndroidAdbNotOpened,

	/*
	 * Mostly stop the data transferring to save power 
	 * */
	AndroidScreenOff,

	/*
	 * Mostly start the data transferring to save power 
	 * */
	AndroidScreenOn = 6,

	/*
	 * Android home button disapear 
	 * */
	AndroidAppForeground,


	/*IOS resume data transferring  */
	IosAppForeground,

	/*
	 * show home button that used to go back to homepage of ecolink app
	 * */
	AndroidAppBackground,

	/*
	 * 	connected status ,set before connected_cb
	 * 	* */
	AndroidConnected,

	/*
	 * 	disconnected status ,set after disconnected_cb
	 * 	* */
	AndroidDisconnected = 11,

	/*
	 * Mostly show init.jpg
	 * */
	IosPlugIn,
	
	/*
	 * Mostly show init.jpg
	 * */
	IosPlugOut,

	/*
	 * Must show the iphone backgroundtip.jpg
	 * */
	IosAppBackground = 14,

	/*
	 * Must show the iphone callingin.jpg
	 * */
	IosCallingin,

	/*
	 * Mostly show help-ios.jpg
	 * */
	IosAppNotReady,

	/*
	 * 	connected status ,set before connected_cb
	 * 	* */
	IosConnected,

	/*
	 * 	disconnected status ,set after disconnected_cb
	 * 	* */
	IosDisconnected ,


	Reserved,

}ECOLINK_ST;

typedef enum
{
	JPEG = 0,
	STREAM,
}ECOLINK_MODE;

typedef enum
{
	EcolinkMainpage = 0,
	PhoneDesktopBtn,
	PhoneReturnBtn,
	PhoneMenuBtn,
	/*
	 * Reserved
	 * */
}EXTRA_EVENTS;


typedef struct 
{
	void (*link_connected_cb)();
	void (*link_disconnected_cb)();
	void (*link_status_changed_cb)(OUT int* status);
	void (*link_phone_screen_data_cb)(OUT int* mode ,OUT char* buffer, OUT int* size);
	int view_base_x;
	int view_base_y;
	int view_width;
	int view_height;
	int sc_width;
	int sc_height;
} ECOLINK_CFG;


/**
 * @brief init_link 
 * @param cb
 * @return 
 */
int init_link(IN ECOLINK_CFG* cb);

/**
 * @brief release_link 
 * @return 
 */
int release_link();

/**
 * @brief get_link_status 
 * @return 
 */
int get_link_status();
/**
 * @brief enable_link_transfer 
 * @return 
 */
int enable_link_transfer(); 

/**
 * @brief disable_link_transfer 
 * @return 
 */
int disable_link_transfer(); 

/**
 * @brief get_link_transfer_status 
 * @return 
 */
int get_link_transfer_status();

/**
 * @brief init_touchevent 
 * @param dev
 * @return 
 */
int init_touchevent(const char* dev); 
/**
 * @brief enable_touchevent 
 * @return 
 */
int enable_touchevent(); 
/**
 * @brief disable_touchevent 
 * @return 
 */
int disable_touchevent(); 

/**
 * @brief extra_event 
 * @param e
 * @return 
 */
int extra_event(EXTRA_EVENTS e);
/**
 * @brief enable_link_iosstream_transfer 
 * @return 
 */
int enable_link_iosstream_transfer();


#ifdef __cplusplus
}
#endif


#endif
