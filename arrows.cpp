#include "arrows.h"
#include <QTimer>
#include <QPainter>
#include <QPolygon>
QTimer* timer;
int cc;
bool checked[4]={0,0,0,0};
QPoint shift[4];
float shift_c[4];
int cnt_ar[4];
//QTimer
QPolygon pgn_o, pgn_ov;
QPolygon  pgn[4];
Arrows::Arrows(QWidget *parent) : QWidget(parent)
{
    int width=100;
    int thick=width*0.4;
    int height=190;
    pgn_o.push_back(QPoint(-width/2,-height/2));
    pgn_o.push_back(QPoint(-width/2+thick,-height/2));
    //    pgn_o.push_back(QPoint(0,-height/2));
    pgn_o.push_back(QPoint(width/2,0));
    pgn_o.push_back(QPoint(-width/2+thick,height/2));
    pgn_o.push_back(QPoint(-width/2,height/2));
    pgn_o.push_back(QPoint(width/2-thick,0));

    thick=width*0.4;
    float scr=1.2;
    pgn_ov.push_back(QPoint(-height/2*scr,-width/2));
    pgn_ov.push_back(QPoint(-height/2*scr,-width/2+thick));
    //    pgn_o.push_back(QPoint(0,-height/2));
    pgn_ov.push_back(QPoint(0,width/2));
    pgn_ov.push_back(QPoint(height/2*scr,-width/2+thick));
    pgn_ov.push_back(QPoint(height/2*scr,-width/2));
    pgn_ov.push_back(QPoint(0,width/2-thick));

    for(int i=0;i<4;i++)
        pgn[i].resize(6);


    timer = new QTimer(this);
    timer->setInterval(42);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));


}


void Arrows::paintEvent(QPaintEvent *e)
{
    cc++;
    int width=this->geometry().width();
    int height=this->geometry().height();

    for(int i=0;i<4;i++)
    {
        checked[0]=1;
        if(checked[i])
            cnt_ar[i]++;
        else cnt_ar[i]=0;

        shift_c[i]=1-exp(-cnt_ar[i]*.4);
    }



    int indent=100;
    for(int i=0;i<6;i++)
    {
        pgn[0][i]=pgn_o[i]+QPoint(indent+100*shift_c[0],height/2);//left
        pgn[1][i]=-pgn_o[i]+QPoint(width-indent,height/2)+shift[1];//right
        pgn[2][i]=pgn_ov[i]+QPoint(width/2,indent)+shift[2];//up
        pgn[3][i]=-pgn_ov[i]+QPoint(width/2,height-indent);//down
    }



    QPainter* painter=new QPainter(this);




    painter->setBrush(Qt::black);
    painter->drawRect(QRect(this->geometry()));

    for(int i=0;i<4;i++)
    {
        QPainterPath path;
        path.addPolygon(pgn[i]);
        int h=55*shift_c[i]+200;
        painter->fillPath(path,QBrush(QColor(h,h,h)));
    }
    //    painter->drawPolygon(pgn[0]);
    painter->setPen(QPen(QColor(0,0,255)));
    //    painter->drawLine(QPoint(0,0),QPoint(20,cc));

    delete painter;
}
