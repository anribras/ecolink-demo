#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

//#include "ringbuffer.h"

#define FULL_PATH(filename) "/usr/app/res/le/images/"#filename""

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
    QTimer *timer;
    QPoint *cur_pos;
    QPixmap m_pix;
    QPixmap m_pix1;
    QImage m_im;
    /**
     * method
     * */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paint_image(const char* file);
    void start_get_image();
    void stop_get_image();
    void stop_decode_paint();
    void show_ecolink();
    void hide_ecolink();
	int get_mirror_status();
	void set_mirror_status(int flag);
private slots:
    //void on_pushButton_clicked();
    void qt_draw_image();
    void gl_draw_image();

private:
    Ui::MainWindow *ui;
    void thread_consume(void* pdata);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
