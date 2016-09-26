/** * @file mainapp_client.c
 * @brief
 * @author yangding@le.com
 * @version v0.1
 * @date 2016-05-10
 */
#include "ecolink.h"
#include <sys/un.h>

TIME_IMPORT();

#define UINX_SOCKET_ADDR "unix_socket.txt"
#define HS_SOCKET_ADDR "/var/tmp/mirrorlink.txt"

char SOCKET_ADDR[] = "unix_socket.txt";
char DF_SOCKET_ADDR[] = "/var/tmp/mirrorlink.txt";

static int main_app_client;
static NSL_MsgInfo_t msg_t;
extern int rereive_anytouch_when_srceen_is_off ;
static int last_msg = 0;

/** @brief voice_ctl  */
void launch_voice_identify(void)
{
	PRIVATE_PROT msg;
	char msg_data[4] = {0};
	char * msg_ptr = 0;
	msg.header = 0xEEFF;
	msg.app_id = 2;
	msg.total_num = 1;
	msg.index_num = 1;
	msg.type = 1; //string
	msg.extend = 0;
	msg.content_len =  4;
	*((int*)msg_data) = 0x1;
	msg_ptr = (char *)malloc(sizeof(PRIVATE_PROT) + msg.content_len );
	memcpy(msg_ptr,&msg,sizeof(PRIVATE_PROT));
	memcpy(msg_ptr + sizeof(PRIVATE_PROT), msg_data,msg.content_len);
	Ecolink_postPrivateData(1,msg_ptr,msg.content_len + sizeof(PRIVATE_PROT));
	free(msg_ptr);
}
/**
 * @brief send_to_main_app
 * @param type
 * @return
 */
int send_msg_to_main_app(NSL_MsgType_t type)
{
    int ret = 0;
    msg_t.MsgType = (NSL_MsgType_t)type;
	DBG("%s : %d\n",__FUNCTION__,type);
    ret = write(main_app_client,&msg_t,sizeof(NSL_MsgInfo_t));
    return ret;
}
/**
 * @brief thread_app_client
 * @param para
 * @return
 */
void* thread_app_client(void* para)
{
    struct sockaddr_un addr;
    int length = 0;
    int ret = 0;
client_start:
	DBG("main_app_client connecting ...\n");
	usleep(100000);
	length = 0;
	ret = 0;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    //DBG("para = %s\n",(char*)para);
    strcpy(addr.sun_path, (char*)para);
    main_app_client = socket(AF_UNIX, SOCK_STREAM, 0);
    ret = connect(main_app_client, (struct sockaddr*)&addr, sizeof(addr));
    DBG("ret = %d\n",ret);
    if(ret == -1) {
        DBG("main_app_client connect error\n");
        goto client_start;
    } else {
        memset(&msg_t , 0 , sizeof(msg_t));
        length = send_msg_to_main_app(WM_ECOLINK_START);
        if(length == -1) {
            DBG("main_app_client send start error\n");
            return NULL;
        } else {
            DBG("main_app_client send start %d bytes\n",length);
        }
        while(1) {
            memset(&msg_t , 0 , sizeof(msg_t));
            length = read(main_app_client,&msg_t,sizeof(msg_t));
			/*in case mainapp breaks connection*/
			if(length < 0){
                DBG("main_app_client recv error\n");
				return ;
			}
            if(length == 0) {
                DBG("main_app_client connection broken\n");
				goto client_start;
            } else {
                DBG("get client data from main_app, msg_type = %d\n",msg_t.MsgType);
                switch(msg_t.MsgType) {
                case WM_ECOLINK_EXIT:
                    DBG("exit ecolink\n");
                    system("killall server.bin");
                    shutdown(main_app_client,SHUT_RDWR);
                    exit(0);
                    break;
                case WM_ECOLINK_WND_SHOW:
						start_paint_and_show_window();
                    break;
                case WM_ECOLINK_WND_HIDE:
                    stop_paint_and_hidden_window();
                    break;
				case WM_ECOLINK_RETURN: //14
					Ecolink_updateRemoteKey(eSend2PhoneEvent_Return);
					break;
				case WM_ECOLINK_PREV_MUSIC://15
					DBG("todo ,prev music\n");
					break;
				case WM_ECOLINK_NEXT_MUSIC://16
					DBG("todo ,next music\n");
					break;
				case WM_ECOLINK_MUTE://18
					DBG("todo ,mute\n");
					break;
				case WM_ECOLINK_DISP_OFF://25
					/*
					 * screen control authority gives back to main app
					 * */
					rereive_anytouch_when_srceen_is_off  = TRUE; 
					break;
				case WM_ECOLINK_DISP_ON://26
					DBG("todo ,display screen on\n");
					break;
                case WM_CLEAR_TIME_DEBUG:
                    DBG("clear time\n");
                    TIME_INIT();
                    break;
				case WM_ECOLINK_LAUNCH_VOCIE:
                    DBG("lunch voice indentify\n");
					launch_voice_identify();
					break;
                default:
                    break;
                }
				last_msg = msg_t.MsgType;
            }

        }

    }
}


				

