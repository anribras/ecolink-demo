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
	 * IOS resume data transferring  
	 * */
	AppForeground,

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
	AndroidDisconnected = 10,

	/*
	 * Mostly show init.jpg
	 * */
	IosPlugIn,
	
	/*
	 * Mostly show init.jpg
	 * */
	IosPlugOut,

	/*
	 * Data transferring will be paused.
	 * we better stop data processing
	 * Must show the iphone backgroundtip.jpg
	 * */
	IosAppBackground = 13,

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
	IosDisconnected = 16,


	Reserved,

}ECOLINK_ST;

typedef enum
{
	JPEG = 0,
	STREAM,
}ECOLINK_MODE;


typedef struct 
{
	void (*link_connected_cb)();
	void (*link_disconnected_cb)();
	void (*link_status_changed_cb)(OUT int* status);
	void (*link_phone_screen_data_cb)(OUT int* mode ,OUT const char* buffer, OUT int* size);
} ECOLINK_CB;


/**
 * @brief get_link_version 
 * @param version
 * @return  version code
 */
int query_sdk_version(char* version);
/**
 * @brief init_link 
 * @param cb
 * @return 
 */
int init_link(IN ECOLINK_CB* cb);

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

#ifdef __cplusplus
}
#endif


#endif
