#include <QApplication>

#include "mainwindow.h"
#include "debug.h"
//#include "ecolink.h"
#include "stream.h"
#include "sdk.h"
#include "demo.h"
#include <signal.h>


MainWindow * window;

#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <netinet/in.h>    // for sockaddr_in
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

typedef enum {
    WM_ECOLINK_UNKNOWN	 = 0,
    WM_ECOLINK_START,
    WM_ECOLINK_EXIT,
    WM_ECOLINK_WND_SHOW ,
    WM_ECOLINK_WND_HIDE ,
    WM_ECOLINK_PARK_PLAY ,
    WM_ECOLINK_PARK_STOP ,
    WM_ECOLINK_CHINESE , //7
    WM_ECOLINK_ENGLISH ,
    WM_ECOLINK_NODEVICE,
    WM_ECOLINK_CONNECTING,
    WM_ECOLINK_CONNECTED,
    WM_ECOLINK_CONNECTERROR,
	WM_ECOLINK_RETURN = 14,
	WM_ECOLINK_NEXT_MUSIC = 15,
	WM_ECOLINK_PREV_MUSIC = 16,
	WM_ECOLINK_MUTE = 18,
	WM_ECOLINK_DISP_OFF = 25,
	WM_ECOLINK_DISP_ON = 26,
	WM_ECOLINK_LAUNCH_VOCIE =  27,
	WM_ECOLINK_OTHER_PAGE = 28,
    WM_ECOLINK_ACC_ON,
    WM_ECOLINK_ACC_OFF,
    WM_ECOLINK_POWER_ON,
    WM_ECOLINK_POWER_OFF,
    WM_ECOLINK_HOME_REQ,

    //WM_ECOLINK_MUTE, //18
    WM_ECOLINK_BT_STATE, //19
    WM_ECOLINK_TIME_MOVE, //20
    WM_ECOLINK_TEMPUTURE, //22
    WM_ECOLINK_SKIN_SET, //21
    WM_ECOLINK_WARNING_SHOW,
    WM_ECOLINK_WARNING_HIDE,
    WM_ECOLINK_SETTING,
    WM_ECOLINK_CAR_BACK,
    WM_ECOLINK_CAR_BACK_OK,

    WM_ECOLINK_DEV_CONNECT = 100,
    WM_ECOLINK_DEV_DISCONNECT ,
    WM_ECOLINK_HARDKEY_BACK,
    WM_ECOLINK_HARDKEY_PRE,
    WM_ECOLINK_HARDKEY_NEXT,
    WM_ECOLINK_WND_SHOW_RSP = 200,
    WM_ECOLINK_WND_HIDE_RSP =201,
    WM_ECOLINK_WND_WIFI_CUT =202,
    WM_ECOLINK_CLOSE=203,
    WM_ECOLINK_HIDE_OK=204,
    WM_ECOLINK_PAUSE=205,
    WM_ECOLINK_RESUME=206,
    WM_ECOLINK_MAX,
    //user define
    WM_ECOLINK_WND_LANGUAGE,
    WM_ECOLINK_WND_QUIT,
    WM_ECOLINK_SHOW_NOTIFY=300,
    WM_CLEAR_TIME_DEBUG
} NSL_MsgType_t;

typedef struct {
    NSL_MsgType_t MsgType;
    char MsgBuf[256];
} __attribute__ ((packed))NSL_MsgInfo_t;

static NSL_MsgInfo_t msg_t;
/**
 * @brief thread_client
 * @param para
 * @return
 */
void* thread_client(void* para)
{
    struct sockaddr_in addr;
    int length = 0;
    int ret = 0;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1) {
        DBG("test client connect error\n");
        //close(fd);
        return NULL;
    } else {
        //first send ok to server
        char start[] = "ecolink_start";
        strcpy(msg_t.MsgBuf,start);
        length = send(fd,&msg_t,sizeof(msg_t),0);
        if(length == -1) {
            DBG("client send start error\n");
            //close(fd);
            return NULL;
        } else {
            DBG("client send start %d bytes\n",length);
        }

        while(1) {
            //DBG("client ready recv...\n");
            memset(&msg_t,0,sizeof(msg_t));
            length = recv(fd,&msg_t, sizeof(msg_t),0);
            if(length == -1) {
                DBG("client recv error\n");
                //close(fd);
                return NULL;
            } else {
                DBG("client recv :%s\n",msg_t.MsgBuf);
                if(!strcmp(msg_t.MsgBuf,"s")) {
					DBG("client recv show\n");
					window->show_ecolink();
                } else if(!strcmp(msg_t.MsgBuf,"h")) {
					DBG("client recv hide\n");
					window->hide_ecolink();
                } else if(!strcmp(msg_t.MsgBuf,"q")) {
					DBG("client recv quit\n");
                } else {
                    system((const char*)msg_t.MsgBuf);
                    //DBG("doing nothing\n");
                }
            }
        }
    }
}

void on_signal(int sign_no)
{
  if(sign_no == SIGINT)
  {
	  DBG("ctrl + c to quit\n");
	  exit(0);
  }
}


/**
 * @brief main
 * @param argc
 * @param argv[]
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    window = &w;
	signal(SIGINT, on_signal);
	/*
	 * ringbuf 
	 * */
    //window->m_rbuf.set_mainwindow_ptr(&w);
    //window->m_rbuf.init();
    window->show();
	/*
	 * sdk init
	 * */
	ECOLINK_CFG cfg;
	cfg.link_connected_cb = connect;
	cfg.link_disconnected_cb = disconnect;
	cfg.link_status_changed_cb= st_changed;
	cfg.link_phone_screen_data_cb= data_gotten;
	/*
	 * offset 
	 * */
	cfg.view_base_x = 0;
	cfg.view_base_y = 0;
	/*
	 * view size
	 * */
	cfg.view_width= 1280;
	cfg.view_height= 720;
	/*
	 * screen size
	 * */
	cfg.sc_width = 1280;
	cfg.sc_height = 720;


	if(init_link(&cfg))
		exit(-1);
	/*
	 *touchscreen event init
	 */
	if(init_touchevent("/dev/input/event0"))
		exit(-1);

	/*
	 * test client
	 * */
	pthread_t id;
	pthread_create(&id,NULL,thread_client,NULL);


    DBG("qt5 ecolink start\n");


    return app.exec();
}
