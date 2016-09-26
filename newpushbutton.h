#ifndef NEWPUSHBUTTON_H
#define NEWPUSHBUTTON_H
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QPixmap>
#include <QPoint>


class newPushButton : public QPushButton
{
public:
    newPushButton(QWidget*);
    void setIcon(int x ,int y, float ratio, QPixmap* pix, int x1, int y1, char* text);
private:
    QPixmap* m_pixmap;
    char* m_text;
    int PicX,PicY,TextX,TextY;
    float Ratio;
    void paintEvent(QPaintEvent *event);
};

#endif // NEWPUSHBUTTON_H
