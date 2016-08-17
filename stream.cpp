#ifndef _STREAM_PLAY_CPP
#define _STREAM_PLAY_CPP

#include "stream.h"
#include "debug.h"


//#define SRC_FROM_FILE



static int state = NULLSTATE;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;
static guint source_id = 0;
static GMainLoop * loop;
static GstAppSrc* src = NULL;
static GstElement* pipeline = NULL;
/**
 * @brief start_loop 
 * @param para
 */
void start_loop(void* para)
{
	DBG("%s\n",__FUNCTION__);
    g_return_if_fail(NULL != loop);
	g_main_loop_run(loop);
	if(state != NULLSTATE){
		DBG("exit looping\n");
		gst_element_set_state (pipeline, GST_STATE_NULL);
		gst_object_unref(GST_OBJECT(pipeline));
		g_main_loop_unref(loop);
	}
	return;
}


/**
 * @brief gstreamer_get_status 
 * @return 
 */
int gstreamer_get_status(){
	return state;
}

void gstreamer_start_loop()
{
    GThread * thread = g_thread_new("loop", (GThreadFunc)start_loop, NULL);
}

/** @brief play  */
void gstreamer_play()
{
	//gstreamer_start_loop();
	if(pipeline)
		gst_element_set_state (pipeline, GST_STATE_PLAYING);
	state = PLAYING;
}

/** @brief pause  */
void gstreamer_pause()
{
	if(pipeline){
		gst_element_set_state (pipeline, GST_STATE_PAUSED);
	}
	state = PAUSED;
	//g_main_loop_quit(loop);
}

/** @brief release  */
void gstreamer_release()
{
	DBG("%s\n",__FUNCTION__);
	state = NULLSTATE;
	if(pipeline)
		gst_element_set_state (pipeline, GST_STATE_NULL);
    //gst_object_unref(GST_OBJECT(pipeline));
	g_main_loop_quit(loop);
    //g_main_loop_unref(loop);
}


/**
 * @brief push_in 
 * @param data
 * @param length
 * @param lock
 * @return 
 */
int push_in(char* data ,int length,pthread_mutex_t* lock){

	//if(length < 500){
		//DUMP_L(data,length);
	//}
	GstFlowReturn signal_status;
	GstBuffer* buffer;
	guint8 *ptr = (guint8 *)g_malloc(length);
	g_assert(ptr);
	memcpy(ptr,data,length);
	buffer = gst_buffer_new();
	gst_buffer_append_memory(buffer,
	gst_memory_new_wrapped(
		GST_MEMORY_FLAG_PHYSICALLY_CONTIGUOUS,
		ptr, length, 0, length, ptr, g_free));
	g_signal_emit_by_name(src, "push-buffer", buffer, &signal_status);
	//DBG("pushed length =%d,status = %d\n",length,signal_status);
	if(GST_FLOW_OK == signal_status) {
		//DBG("push buffer ok\n");
		gst_buffer_unref(buffer);
		pthread_mutex_unlock(lock);
		return true;
	} else {
		DBG("push buffer returned %d for %d bytes \n", signal_status, length);
		gst_buffer_unref(buffer);
		pthread_mutex_unlock(lock);
		return false;
	}
}
/**
 * @brief get_steam_data_cb
 * @param data
 * @param length
 * @return
 */
static char stream_buf[ 1000 * 1024];
static int buffered_len = 0;
static int has_long_frame_buffered = FALSE;
static int start = 0;
static int status = 0;
typedef enum STATUS{
	HAS_START_CODE_f,
	HAS_START_CODE_s,
	MORE_DATA,
	HAS_FRAME_HEAD
};

int get_steam_data_cb(char*data, int length)
{
    //DBG("get stream data ...length = %d\n",size);
	if(state  == PLAYING){
		pthread_mutex_lock(&lock);
#if 0
		int nal_header = *((int*)data);
		DUMP_L(data,10);
		//if meets 0x41 0x65 save into a whole frame first
		switch(status){
			case HAS_FRAME_HEAD:
				//DBG("s:%d buffered_len = %d\n",status,buffered_len);
				push_in(stream_buf,buffered_len,&lock);
				buffered_len = 0;
				//DBG("s:%d buffered_len = %d\n",status,buffered_len);
				status = 0;
				break;
			case MORE_DATA:
				memcpy(stream_buf + buffered_len,data,length);
				buffered_len +=  length;
				//DBG("s:%d buffered_len = %d\n",status,buffered_len);
				status = MORE_DATA;
				pthread_mutex_unlock(&lock);
				break;
			default:
				break;
		}
		if(nal_header == 0x01000000){
			if(data[4] == 0x65 || 
			   data[4] == 0x41 ||
			   data[4] == 0x67){
				memcpy(stream_buf + buffered_len,data,length);
				buffered_len +=  length;
				//DBG("s:%d buffered_len = %d\n",status,buffered_len);
				status = HAS_FRAME_HEAD;
				pthread_mutex_unlock(&lock);
				return;
			}
		}
#else
		//DUMP_L(data,10);
		push_in(data,length,&lock);
#endif

	}
}


static gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data)
{
    switch(GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        DBG("End-of-stream\n");
        //g_main_loop_quit(loop);
        break;
    case GST_MESSAGE_ERROR:
        gchar *debug;
        GError *error;

        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);

        DBG("Error: %s\n", error->message);
        g_error_free(error);

        //g_main_loop_quit(loop);

        break;

    case GST_MESSAGE_WARNING: {
        gchar *debug;
        GError *err;

        gst_message_parse_warning(msg, &err, &debug);
        g_free(debug);
        DBG("Message Warn  %d Error: (%s:%d) %s\n",  (gint32)err->domain, g_quark_to_string(err->domain),err->code, err->message);
        g_error_free(err);

        break;
    }
    case GST_MESSAGE_INFO: {
        DBG(" Message Info\n");
        break;
    }
    case  GST_MESSAGE_TAG: {
        break;
    }
    case GST_MESSAGE_STATE_CHANGED: { //64
        GstState old_state, new_state;

        gst_message_parse_state_changed(msg, &old_state, &new_state, NULL);
#if 0
        DBG (" Element %s changed state from %s to %s\n",
             GST_OBJECT_NAME (msg->src),
             gst_element_state_get_name (old_state),
             gst_element_state_get_name (new_state));
#endif
        break;
    }
    case GST_MESSAGE_STEP_DONE: {
        DBG("Message StepDone\n");
        break;
    }
    case GST_MESSAGE_NEW_CLOCK: {
        DBG("Message NewClock\n");
        break;
    }
    case GST_MESSAGE_STREAM_STATUS: {
        GstStreamStatusType type;
        GstElement  *owner;
        gst_message_parse_stream_status (msg, &type, &owner);
#if 0
        DBG("Message StreamStatus Element %s (%d)\n",  GST_OBJECT_NAME (msg->src), (gint32)type);
#endif
        break;
    }
    default:
        DBG("default,type = %d\n",GST_MESSAGE_TYPE(msg));
        break;
    }

    return true;
}

int len  = 0;
long total = 0;
FILE* fp = NULL ;


static gboolean get_stream_data(void* data , unsigned long length)
{
    DBG("data from file\n");
    int frame_len = 1024 * 50;
    guint8 *ptr = (guint8 *)g_malloc(frame_len);
    g_assert(ptr);
    GstFlowReturn signal_status;
    GstBuffer* buffer;
    if((len = fread(ptr,sizeof(char),frame_len,fp)) != 0) {
        total += len;
        /* wrap and send data to the AppSrc */
        buffer = gst_buffer_new();
        gst_buffer_append_memory(buffer,
                                 gst_memory_new_wrapped(GST_MEMORY_FLAG_READONLY, ptr, len, 0, len, ptr, g_free));
        g_signal_emit_by_name(src, "push-buffer", buffer, &signal_status);
        if(GST_FLOW_OK == signal_status) {
            DBG("push buffer ok\n");
            gst_buffer_unref(buffer);
            return true;
        } else {
            DBG("push buffer returned %d for %d bytes \n", signal_status, len);
            gst_buffer_unref(buffer);
            return false;
        }
    }
}
/**
 * @brief start_feed
 * @param pipeline
 * @param size
 * @param data
 */
static void start_feed(GstElement * pipeline, guint size, gpointer data)
{
    //DBG("start_feed\n");
#ifdef SRC_FROM_FILE
    if (source_id == 0) {
        source_id = g_idle_add((GSourceFunc)get_stream_data, data);
    }
#endif
}
/**
 * @brief stop_feed
 * @param pipeline
 * @param data
 */
static void stop_feed(GstElement * pipeline, gpointer data)
{
	guint64 max = gst_app_src_get_max_bytes(src);
	guint64 cur = gst_app_src_get_current_level_bytes(src);
    DBG("warnning ! stream data full max=%d cur=%d\n",max,cur);
#ifdef SRC_FROM_FILE
    if (source_id != 0) {
        g_source_remove(source_id);
        source_id = 0;
    }
#endif
}



/**
 * @brief gstreamer_init 
 * @param path
 */
void gstreamer_init(int need_scale)
{
    DBG("%s\n",__FUNCTION__);
    GstBus *bus;
    GstElement*	parser= NULL;
    GstElement* decoder = NULL;
	GstElement* converter = NULL;
    GstElement* rate = NULL;
    GstElement* scale = NULL;
    GstElement* filter = NULL;
    GstElement* sink = NULL;
	GValue value;

    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL,false);

    /*init element*/
    pipeline = gst_pipeline_new("h264-fifo-player");
    if(!pipeline ) {
        DBG("create pipeline element fail\n");
    }
    src = (GstAppSrc *)gst_element_factory_make("appsrc","src0");
    if(!src) {
        DBG("create src fail\n");
    }
    g_object_set(G_OBJECT(src), "is-live", TRUE, NULL);
    g_object_set(G_OBJECT(src), "block", TRUE, NULL);

    //g_object_set(G_OBJECT(src), "do-timestamp", TRUE, NULL);
    gst_app_src_set_stream_type(src, GST_APP_STREAM_TYPE_STREAM);
	gst_app_src_set_max_bytes(src,100 * 1024);
    /*byte stream*/
    //g_object_set(G_OBJECT(src), "stream-type", 0, NULL);


    parser = gst_element_factory_make("h264parse","parser");
    if(!parser) {
        DBG("create parser fail\n");
    }
    decoder = gst_element_factory_make("ducatih264decvpe","decoder");
    if(!decoder) {
        DBG("create decoder fail\n");
    }
	
    converter = gst_element_factory_make("videoconvert","converter");
    if(!converter) {
        DBG("create converter fail\n");
    }

	if(need_scale){
		rate = gst_element_factory_make("videorate","rate");
		if(!rate) {
			DBG("create rate fail\n");
		}
		scale = gst_element_factory_make("videoscale","scale");
		if(!scale) {
			DBG("create scale fail\n");
		}
		filter = gst_element_factory_make("capsfilter","filter");
		if(!filter) {
			DBG("create rate fail\n");
		}
	   //GstCaps *caps = gst_caps_from_string ("video/x-raw,framerate=25/1");
	   GstCaps *caps = gst_caps_from_string ("video/x-raw,height=800,width=1280");
	   g_object_set (filter, "caps", caps, NULL);
	}

    sink = gst_element_factory_make("waylandsink","sink");

    if(!sink) {
        DBG("create sink fail\n");
    }
    /*sink sync false*/
    g_object_set(sink, "sync", false, "async",false,NULL);
    /*sink ivi-id value*/
    int ivi_id = 101202;
    memset(&value, 0, sizeof(GValue));
    g_value_init(&value, G_TYPE_INT);
    g_value_set_int(&value, ivi_id);
    g_object_set_property(G_OBJECT(sink), "ivi-id", &value);

    /*register bus*/
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_callback, loop);
    gst_object_unref(bus);

    /*add element to pipeline*/
	if(need_scale){
		gst_bin_add_many(GST_BIN(pipeline),(GstElement* )src,parser,decoder,converter,scale,filter,sink,NULL);
		/*link all*/
		if(!gst_element_link_many((GstElement* )src,parser,decoder,converter,scale,filter,sink,NULL))
			DBG("pipeline  link fail\n");
		else
			DBG("pipeline link ok\n");
	}
	else{
		gst_bin_add_many(GST_BIN(pipeline),(GstElement* )src,parser,decoder,converter,sink,NULL);
		/*link all*/
		if(!gst_element_link_many((GstElement* )src,parser,decoder,converter,sink,NULL))
			DBG("pipeline link fail\n");
		else
			DBG("pipeline link ok\n");
	}

    /*queue need data!*/
    g_signal_connect(src, "need-data", G_CALLBACK(start_feed), (gpointer)NULL);
    /*queue full*/
    g_signal_connect(src, "enough-data", G_CALLBACK(stop_feed), (gpointer)NULL);/**/

    gst_element_set_state(pipeline,GST_STATE_NULL);

#ifdef SRC_FROM_FILE
    if((fp = fopen((char*)path,"rb")) == NULL) {
        DBG("file open error\n");
        exit(-1);
    }
#endif
#if 0
    //gst_element_set_state(pipeline,GST_STATE_PLAYING);
	g_main_loop_run(loop);
	state = NULLSTATE;
	//DBG("exit looping\n");
    //gst_element_set_state (pipeline, GST_STATE_NULL);
    //gst_object_unref(GST_OBJECT(pipeline));
    //g_main_loop_unref(loop);
#else
	gstreamer_start_loop();
	usleep(100000);
#endif

    return ;

}

/** @brief overlay  */
void overlay(void)
{
#if 0
    guint major, minor, micro, nano;
    gst_init (&argc, &argv);
    gst_version (&major, &minor, &micro, &nano);
    DBG("gst version %d-%d-%d-%d\n",major,minor,micro,nano);
    GstElement *playbin, *videosink;
    gchar *uri;
    playbin = gst_element_factory_make ("playbin", "myplaybin");
    videosink = gst_element_factory_make ("waylandsink", "waylandsink0");
    g_object_set (playbin, "video-sink", videosink, NULL);
    g_object_set (playbin, "uri", "/home/root/ecolink-ui/ecolink-ui/image/1280x800.h264", NULL);
    /*before doing this maksure, pipeline playing state =  PAUSE or null*/
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), w.winId());

    GstStateChangeReturn sret = gst_element_set_state (playbin,GST_STATE_PLAYING);
#endif

}
#endif

