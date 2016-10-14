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
#include "sdk.h"



#define FLOAT_BTN_EN
#define CLIENT_TEST_EN



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
    //timer = new QTimer(this);
    cur_pos = new QPoint();
    ui->setupUi(this);
    /*no titile frame*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    /*tranparent bgd*/
    setAttribute(Qt::WA_TranslucentBackground, true);

    this->paint_image(FULL_PATH(init.jpg));

#ifdef FLOAT_BTN_EN

    ui->floatButton->installEventFilter(this);
    ui->floatButton->setIcon(QIcon(FULL_PATH(float-button.png)));
    ui->floatButton->hide();

    ui->label->setPixmap(QPixmap(FULL_PATH(group.png)));

    ui->btnGroup->setStyleSheet("border: none");
    ui->btnGroup->hide();

    ui->appBtn->setIcon(83,15,1.5,new QPixmap(FULL_PATH(le.png)),65,83,"乐车联");
    ui->returnBtn->setIcon(83,15,1.5,new QPixmap(FULL_PATH(back.png)),80,83,"返回");
    ui->menuBtn->setIcon(83,15,1.5,new QPixmap(FULL_PATH(menu.png)),83,83,"菜单");
    ui->homeBtn->setIcon(83,15,1.5,new QPixmap(FULL_PATH(home.png)),82,83,"桌面");

    ui->centralWidget->setStyleSheet("background-color:transparent;");
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
#ifdef FLOAT_BTN_EN
    static QPoint startPnt;
    static QPoint pressPnt;
    static QPoint lastPnt;
    QMouseEvent* e = static_cast<QMouseEvent*>(evt);
    startPnt = ui->floatButton->pos();
    //qDebug("start pos(%d %d)\n",startPnt.x(),startPnt.y());
    if(evt->type() == QEvent::MouseButtonPress)
    {
        qDebug("oMouseButtonPress (%d %d)\n",e->pos().x(),e->pos().y());
        if(ui->floatButton->rect().contains(e->pos()) )
        {
            //qDebug("inPosition\n");
            pressPnt = e->pos();
            //qDebug("press pos(%d %d)\n",pressPnt.x(),pressPnt.y());
            disable_touchevent();
        }
    }
    else if(evt->type() == QEvent::MouseMove)
    {
        movedFolatBtn = true;
        qDebug("MouseMove (%d %d)\n",e->pos().x(),e->pos().y());
        int dx = e->pos().x() - pressPnt.x();
        int dy = e->pos().y()-pressPnt.y();
        //qDebug("dx dy (%d %d)\n",dx,dy);
        ui->floatButton->move(ui->floatButton->pos().x()+dx,ui->floatButton->pos().y()+dy);

    }else if(evt->type() == QEvent::MouseButtonRelease)
    {
        if(!movedFolatBtn)
        {
            //show btngroup
            ui->btnGroup->show();
            //ui->label->show();
            ui->floatButton->hide();

        }
        movedFolatBtn = false;
        qDebug("MouseMouseButtonRelease (%d %d)\n",e->pos().x(),e->pos().y());
        //qDebug("start pos(%d %d)\n",startPnt.x(),startPnt.y());
        int x = startPnt.x();
        int y = startPnt.y();
        if(x < 0)
        {
            x = 0;
        }
        if( (x  + ui->floatButton->width()) > this->width())
        {
            x = startPnt.x() + this->width() - (x + ui->floatButton->width());
        }
        if(y < 0)
        {
            y = 0;
        }
        if((y  + ui->floatButton->height()) > this->height())
        {
            y = startPnt.y() + this->height() - (y + ui->floatButton->height());
        }
        ui->floatButton->move(x,y);

        enable_touchevent();
    }

    lastPnt = e->pos();
#endif

    return false;
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
	/*
	 * depends on!
	 * 1. only qt UI
	 * 2. stream is foreground
	 * 3. UI and stream all foreground
	 * */
    this->show();
    ui->picLabel->show();
	DBG("show_ecolink\n");
}
void MainWindow::hide_ecolink()
{
	DBG("hide_ecolink\n");
    //m_fbc.Alpha("/dev/fb0",1,255);//only qt display = close stream
    this->hide();//hide qt display
    ui->picLabel->hide();
}

void MainWindow::paint_image(const char* file)
{
#ifdef FLOAT_BTN_EN
    //ui->picLabel->setPixmap(QPixmap(file));
    ui->picLabel->setPixmap(QPixmap(file));
#else
    ui->label->setPixmap(QPixmap(file));
#endif
}

#ifdef FLOAT_BTN_EN
void MainWindow::on_floatButton_clicked()
{
    qDebug("on_floatButton_clicked\n");
    //ui->btnGroup->hide();
    //ui->floatButton->show();
}

void MainWindow::on_appBtn_clicked()
{
    qDebug("on_appBtn_clicked\n");
    ui->btnGroup->hide();
    //ui->label->hide();
    ui->floatButton->show();
    extra_event(EcolinkMainpage);
    disable_transparentBgd();//restore again;
}

void MainWindow::on_returnBtn_clicked()
{
    qDebug("on_returnBtn_clicked\n");
    ui->btnGroup->hide();
    //ui->label->hide();
    ui->floatButton->show();
    extra_event(PhoneReturnBtn);

}

void MainWindow::on_homeBtn_clicked()
{
    qDebug("on_homeBtn_clicked\n");
    ui->btnGroup->hide();
    //ui->label->hide();
    ui->floatButton->show();
    extra_event(PhoneDesktopBtn);
}

void MainWindow::on_menuBtn_clicked()
{
    qDebug("on_menuBtn_clicked\n");
    ui->btnGroup->hide();
    //ui->label->hide();
    ui->floatButton->show();
    extra_event(PhoneMenuBtn);

}
#endif

void MainWindow::enable_transparentBgd()
{
    m_fbc.Alpha("/dev/fb0",1,128);//display button
    ui->picLabel->hide();
	ui->floatButton->show();
}

void MainWindow::disable_transparentBgd()
{
    m_fbc.Alpha("/dev/fb0",1,0);//display stream fully
    ui->picLabel->hide();
	ui->floatButton->hide();
}


