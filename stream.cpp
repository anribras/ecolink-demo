#ifndef _STREAM_PLAY_CPP
#define _STREAM_PLAY_CPP

#include "stream.h"
#include "debug.h"




static int state = NULLSTATE;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;
static guint source_id = 0;
static GMainLoop * loop;
static GstAppSrc* src = NULL;
static GstElement* pipeline = NULL;
static int fd;
static int len  = 0;
static long total = 0;
static FILE* fp = NULL ;
/**
 * @brief start_loop 
 * @param para
 */
void start_loop(void* para)
{
	DBG("%s\n",__FUNCTION__);
    g_return_if_fail(NULL != loop);
	g_main_loop_run(loop);
	DBG("exit looping\n");
	g_main_loop_unref(loop);
	gst_object_unref(GST_OBJECT(pipeline));
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
	g_main_loop_quit(loop);
    //gst_object_unref(GST_OBJECT(pipeline));
    //gst_object_unref(GST_OBJECT(pipeline));
}


/**
 * @brief push_in 
 * @param data
 * @param length
 * @return 
 */
int push_in(char* data ,int length){
	//DUMP_L(data,50);
	GstFlowReturn signal_status = GST_FLOW_ERROR;
	GstBuffer* buffer;
#if 0
    guint8 *ptr = (guint8 *)g_malloc(length);
    //guint8 ptr[500 * 1024]; //can' t work 
	//so weird the ptr memory allocation must use g_malloc and must not g_free after
	//push-buffer ! This might cause memory leaking.
    g_assert(ptr);
    memcpy(ptr,data,length);
    buffer = gst_buffer_new_and_alloc(length);
    GST_BUFFER_MALLOCDATA(buffer) = ptr;
    GST_BUFFER_SIZE(buffer) = length;
    GST_BUFFER_DATA(buffer) = GST_BUFFER_MALLOCDATA(buffer);
	GST_BUFFER_FREE_FUNC(buffer) = g_free;
    //gst_buffer_set_data(buffer,(guint8*)data,length);  
	g_signal_emit_by_name(src, "push-buffer", buffer, &signal_status);
	//signal_status = gst_app_src_push_buffer(src,buffer);//can`t work
#else
    buffer = gst_buffer_new_and_alloc(length);
	memcpy(GST_BUFFER_DATA(buffer), data, length);
    GST_BUFFER_SIZE(buffer) = length;
	signal_status = gst_app_src_push_buffer(src,buffer);
#endif
	if(GST_FLOW_OK == signal_status) {
		//DBG("push buffer ok new\n");
		//gst_buffer_unref(buffer);
        //g_free(ptr);// must not 
		return true;
	} else {
		DBG("push buffer returned %d for %d bytes \n", signal_status, length);
        //g_free(ptr);
		gst_buffer_unref(buffer);
		return false;
	}
}
/**
 * @brief get_steam_data_cb
 * @param data
 * @param length
 * @return
 */
#if 0
static char stream_buf[ 500 * 1024];
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
#endif

int get_steam_data_cb(char* data, int length)
{
	if(state  == PLAYING){
        pthread_mutex_lock(&lock);
        push_in(data,length);
        pthread_mutex_unlock(&lock);
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
        DBG (" Element %s changed state from %s to %s\n",
             GST_OBJECT_NAME (msg->src),
             gst_element_state_get_name (old_state),
             gst_element_state_get_name (new_state));
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
        DBG("Message StreamStatus Element %s (%d)\n",  GST_OBJECT_NAME (msg->src), (gint32)type);
        break;
    }
    default:
        DBG("default,type = %x\n",GST_MESSAGE_TYPE(msg));
        break;
    }

    return true;
}


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
#if 0
        gst_buffer_append_memory(buffer,
                                 gst_memory_new_wrapped(GST_MEMORY_FLAG_READONLY, ptr, len, 0, len, ptr, g_free));
        g_signal_emit_by_name(src, "push-buffer", buffer, &signal_status);
#else
#endif
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
}
/**
 * @brief stop_feed
 * @param pipeline
 * @param data
 */
static void stop_feed(GstElement * pipeline, gpointer data)
{
    DBG("stop_feed\n");
}



/**
 * @brief gstreamer_init 
 * @param mode 0-JPEG 1-H264
 */
void gstreamer_init(int mode)
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
    src = (GstAppSrc *)gst_element_factory_make("appsrc","src");
    if(!src) {
        DBG("create src fail\n");
    }
    g_object_set(G_OBJECT(src), "is-live", true, NULL);
    //g_object_set(G_OBJECT(src), "blocksize", 1024 * 100, NULL);
    g_object_set(G_OBJECT(src), "block", true, NULL);
    //g_object_set(G_OBJECT(src), "do-timestamp", TRUE, NULL);
    gst_app_src_set_stream_type(src, GST_APP_STREAM_TYPE_STREAM);
	gst_app_src_set_max_bytes(src,0);
    /*byte stream*/
    //g_object_set(G_OBJECT(src), "stream-type", 0, NULL);


	if(mode)
		parser = gst_element_factory_make("h264parse","parser");
	else
		parser = gst_element_factory_make("jpegparse","parser");
	if(!parser) {
		DBG("create parser fail\n");
	}

	if(mode){
		decoder = gst_element_factory_make("vpudec","decoder");
		g_object_set(G_OBJECT(decoder), "low-latency", TRUE, NULL);
		g_object_set(G_OBJECT(decoder), "framerate-nu", 25, NULL);
		//g_object_set(G_OBJECT(decoder), "framedrop", TRUE, NULL);
		g_object_set(G_OBJECT(decoder), "dis-reorder", TRUE, NULL);
		//g_object_set(G_OBJECT(decoder), "framedrop-level-mask", 0x0ff, NULL); 
	}
	else
		decoder = gst_element_factory_make("ffdec_mjpeg","decoder");

    if(!decoder) {
        DBG("create decoder fail\n");
    }

   converter = gst_element_factory_make("ffmpegcolorspace","converter");
    if(!converter) {
        DBG("create converter fail\n");
    }

    sink = gst_element_factory_make("mfw_v4lsink","sink");
    //sink = gst_element_factory_make("mfw_isink","sink");
    if(!sink) {
        DBG("create sink fail\n");
    }

    /*sink sync false*/
    g_object_set(sink, "sync", false, "async",false,NULL);
    /*register bus*/
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_callback, loop);
    gst_object_unref(bus);
    /*add element to pipeline*/
	gst_bin_add_many(GST_BIN(pipeline),(GstElement* )src,parser,decoder,sink,NULL);
	/*link all*/
	if(!gst_element_link_many((GstElement* )src,parser,decoder,sink,NULL))
		DBG("pipeline link fail\n");
	else
		DBG("pipeline link ok\n");
    /*queue need data!*/
    g_signal_connect(src, "need-data", G_CALLBACK(start_feed), (gpointer)NULL);
    /*queue full*/
    g_signal_connect(src, "enough-data", G_CALLBACK(stop_feed), (gpointer)NULL);/**/

    gst_element_set_state(pipeline,GST_STATE_NULL);

	gstreamer_start_loop();
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

