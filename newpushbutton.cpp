#include "newpushbutton.h"

newPushButton::newPushButton(QWidget* w):QPushButton(w)
{

}



void newPushButton::setIcon(int x, int y, float ratio, QPixmap* pix, int x1 ,int y1, char* text)
{
    m_pixmap = pix;
    m_text = text;
    PicX = x;
    PicY = y;
    TextX = x1;
    TextY = y1;
    Ratio = ratio;
}


void newPushButton::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap;
    QPushButton::paintEvent(event) ;
    QPainter painter(this) ;
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //qDebug("ratio = %d ,(%d %d)\n",Ratio,m_pixmap->width(),m_pixmap->height(),Qt::KeepAspectRatio);
    //qDebug("(%d %d)\n",m_pixmap->width()/Ratio,m_pixmap->height()/Ratio);
    pixmap = m_pixmap->scaled(m_pixmap->width()/Ratio,
                              m_pixmap->height()/Ratio);
    //83 10
    painter.drawPixmap(PicX,PicY,
                       pixmap.width(),
                       pixmap.height(),pixmap);
    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(0,0,0));
    this->setPalette(pal);
    //65 83
    painter.drawText(TextX,TextY,m_text);
}







