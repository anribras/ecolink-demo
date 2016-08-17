#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "debug.h"
#include "stream.h"
/**
 * @brief thread_getimage
 * @param pdata
 */

#include <QImage>

#include <unistd.h>

/**
 * @brief get_mirror_status 
 * @return 
 */
int MainWindow::get_mirror_status()
{
	return mirror_is_running;
}

/**
 * @brief set_mirror_status 
 * @param flag
 */
void MainWindow::set_mirror_status(int flag)
{
	mirror_is_running = flag;
}


/** @brief gl_draw_image  */
void MainWindow::gl_draw_image()
{
    char path[128] = {'\0'};
    //sprintf(path,"/usr/app/res/images/koko/koko%d.jpg",i++ % 10 );
    int num = (count++ % 9) + 1;
    sprintf(path,"/usr/app/res/images/koko/koko%d.jpg",num);
    QImage image;
    image.load(path);
    //image = QGLWidget::convertToGLFormat(image);
}


/** @brief qt_draw_image  */
void MainWindow::qt_draw_image()
{
    char path[128] = {'\0'};
    //sprintf(path,"/usr/app/res/images/koko/koko%d.jpg",i++ % 10 );
    int num = (count++ % 9) + 1;
    sprintf(path,"/usr/app/res/images/koko/koko%d.jpg",num);
    //DBG("%s: %d pic...\n",path,num);
#if 1
    QPixmap pixmap;
    pixmap.load(path);
    pixmap = pixmap.scaled(1280,800);
    this->ui->label->setPixmap(pixmap);
#else

    this->ui->label->setPixmap(QPixmap(path));
#endif

#if 0
    if(!(count % 50)) {
        this->hide();
    }
    if(!(count % 100)) {
        this->show();
    }
#endif
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    count = 1;
    timer = new QTimer(this);
    cur_pos = new QPoint();
    mirror_is_running = FALSE;
    ui->setupUi(this);
    /*no titile frame*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    /*black ground*/
    setAutoFillBackground(true);
    QPalette palette;
    /*black  background*/
    //palette.setColor(QPalette::Background, QColor(0,0,0));
    /*transparent background*/
    //palette.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    //setPalette(palette);
	this->paint_image(FULL_PATH(init.jpg));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    connect(timer, SIGNAL(timeout()), this, SLOT(qt_draw_image()));
//    timer->start(30);//flush every 20  ms
//}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
    //sprintf(path,"/usr/app/res/images/koko/koko%d.jpg",i++ % 10);
    //DBG("%s: %d pic...\n",path,i++ % 10);
    //painter.drawPixmap(0,0, QPixmap(path),0,0,1280,800);
}
/**
 * @brief mousePressEvent
 * @param event
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    *cur_pos = event->pos();
    //DBG("press(x,y) = (%d,%d)\n",cur_pos->x(),cur_pos->y());
    int x = cur_pos->x();
    int y = cur_pos->y();
}
/**
 * @brief mousePressEvent
 * @param event
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    *cur_pos = event->pos();
    //DBG("move(x,y) = (%d,%d)\n",cur_pos->x(),cur_pos->y());
    int x = cur_pos->x();
    int y = cur_pos->y();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    *cur_pos = event->pos();
    //DBG("released(x,y) = (%d,%d)\n",cur_pos->x(),cur_pos->y());
    int x = cur_pos->x();
    int y = cur_pos->y();
}

void MainWindow::start_get_image()
{

}

void MainWindow::stop_get_image()
{
}

void MainWindow::show_ecolink()
{
    this->show();
}
void MainWindow::hide_ecolink()
{
    this->hide();
}

void MainWindow::paint_image(const char* file)
{
#if 1
    ui->label->setPixmap(QPixmap(file));
#endif
}

void MainWindow::paint_image(int mode ,unsigned char* mem, unsigned long size)
{
    static int i =0;
    int fd;
    int format;
    int ret;
    mode = 0;
    i++;
    switch(mode) {
    case 0:
        if( i & 1) {
            m_pix.loadFromData(mem,size,"JPG");
            //ui->label->setPixmap(m_pix);
            //DBG("w = %d h = %d \n",m_pix.height(),m_pix.width());
            if(m_pix.width() == 1280) {
                ui->label->setPixmap(m_pix);
                ui->label->setGeometry(0,0,1280,800);
                //usleep(10000);
            } else if(m_pix.width() == 400) {
                ui->label->setPixmap(m_pix);
                ui->label->setGeometry((1280-400)/2,0,(1280-400)/2,800);
                usleep(20000);
            }
        } else {
            m_pix1.loadFromData(mem,size,"JPG");
            //ui->label->setPixmap(m_pix);
            //DBG("w = %d h = %d \n",m_pix1.height(),m_pix1.width());
            if(m_pix1.width() == 1280) {
                ui->label->setPixmap(m_pix1);
                ui->label->setGeometry(0,0,1280,800);
                //usleep(10000);
            } else if(m_pix1.width() == 400) {
                ui->label->setPixmap(m_pix1);
                ui->label->setGeometry((1280-400)/2,0,(1280-400)/2,800);
                usleep(20000);
            }
        }
        break;
	case 2:
        format = QImage::Format_RGBA8888;
        ret = m_rbuf.m_decoder.decode_from_mem(mem,size);
        DBG("w = %d h = %d \n",m_pix.width(),m_pix.height());
        if(m_pix.width() == 1280) {
            m_im = QImage(m_rbuf.m_decoder.m_rgb_ptr,
                          1280,
                          720,
                          1280 * 4,
                          (QImage::Format)format);
            m_pix = QPixmap::fromImage(m_im);
            ui->label->setPixmap(m_pix);
            ui->label->setGeometry(0,0,1280,800);
            usleep(10000);
        } else if(m_pix.width() == 400) {
            m_im = QImage(m_rbuf.m_decoder.m_rgb_ptr,
                          400,
                          720,
                          400 * 4,
                          (QImage::Format)format);
            m_pix = QPixmap::fromImage(m_im);
            ui->label->setPixmap(m_pix);
            ui->label->setGeometry((1280-400)/2,0,(1280-400)/2,800);
            usleep(20000);
        }
        m_rbuf.m_decoder.release_rgb_buffer();
        break;
    default:
        break;
    }
}
