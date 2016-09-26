#include <QApplication>

#include "mainwindow.h"
#include "debug.h"
//#include "ecolink.h"
#include "stream.h"
#include "sdk.h"
#include "demo.h"
#include <signal.h>


MainWindow * window;

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

    DBG("qt5 ecolink start\n");


    return app.exec();
}
