#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMainWindow>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QDropEvent>
#include <QEvent>
#include <QIcon>

#include "fbcontrol.h"
#include "hs_socket_app.h"


#define FLOAT_BTN_EN

#define FULL_PATH(filename) "/usr/app/phonelink/ecolink/res/le/images/"#filename""
#define gWidth (1280)
#define gHeight (720)

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //CRingBuf m_rbuf;
    unsigned int count;
    //QTimer *timer;
    QPoint *cur_pos;
    //QPixmap m_pix;
    //QPixmap m_pix1;
    //QImage m_im;
    Fbcontrol m_fbc;
    /**
     * method
     * */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paint_image(const char* file);
    void start_get_image();
    void stop_get_image();
    void stop_decode_paint();
    void show_ecolink(bool needRp);
    void hide_ecolink(bool needRp);
    bool get_wnd_display_status();
	int get_mirror_status();
	void set_mirror_status(int flag);
    void enable_transparentBgd();
    void disable_transparentBgd();
private slots:

    void qt_draw_image();
    void gl_draw_image();
#ifdef FLOAT_BTN_EN
    void on_floatButton_clicked();
    void on_appBtn_clicked();
    void on_returnBtn_clicked();
    void on_homeBtn_clicked();
    void on_menuBtn_clicked();
#endif

private:
    Ui::MainWindow *ui;
    void thread_consume(void* pdata);
protected:
    bool wnd_is_showing;
    bool pressedFolatBtn;
    bool movedFolatBtn;
    bool releasedFolatBtn;
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *evt);
};

#endif // MAINWINDOW_H
