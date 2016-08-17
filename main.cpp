#include <QApplication>

#include "mainwindow.h"
#include "debug.h"
//#include "ecolink.h"
#include "stream.h"
#include "sdk.h"
#include "demo.h"


MainWindow * window;


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
	/*
	 * ringbuf 
	 * */
    window->m_rbuf.set_mainwindow_ptr(&w);
    window->m_rbuf.init();
    window->show();
	/*
	 * sdk init
	 * */
	ECOLINK_CB cb;
	cb.link_connected_cb = connect;
	cb.link_disconnected_cb = disconnect;
	cb.link_status_changed_cb= st_changed;
	cb.link_phone_screen_data_cb= data_gotten;

	if(init_link(&cb))
		exit(-1);
	/*
	 *touchscreen event init
	 */
	if(init_touchevent("/dev/input/touchscreen0"))
		exit(-1);

    DBG("qt5 ecolink start\n");

    return app.exec();
}
