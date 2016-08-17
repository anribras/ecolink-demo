#ifndef STREAM_PLAY_H
#define STREAM_PLAY_H
/**
 * @file stream-play.h
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-07-25
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

typedef enum 
{
	NULLSTATE = 0,
	INITIALIZED,
	PLAYING,
	PAUSED,
	RELEASED 
} PLAYING_STATE;



void gstreamer_init(int need_scale);
int get_steam_data_cb(char*data, int length);
void gstreamer_play();
void gstreamer_pause();
void gstreamer_release();
int gstreamer_get_status();

#endif
