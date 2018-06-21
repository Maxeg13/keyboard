#include "arrows.h"
#include <QTimer>
#include <QPainter>
#include <QPolygon>
QTimer* timer;
int cc;
//QTimer
QPolygon pgn_o, pgn_ov;
QPolygon  pgn[4];
Arrows::Arrows(QWidget *parent) : QWidget(parent)
{
    int width=100;
    int thick=width*0.4;
    int height=150;
    pgn_o.push_back(QPoint(-width/2,-height/2));
    pgn_o.push_back(QPoint(-width/2+thick,-height/2));
    //    pgn_o.push_back(QPoint(0,-height/2));
    pgn_o.push_back(QPoint(width/2,0));
    pgn_o.push_back(QPoint(-width/2+thick,height/2));
    pgn_o.push_back(QPoint(-width/2,height/2));
    pgn_o.push_back(QPoint(width/2-thick,0));

    float scr=1.5;
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

    int indent=100;
    for(int i=0;i<6;i++)
    {
        pgn[0][i]=pgn_o[i]+QPoint(indent,height/2);

        pgn[1][i]=-pgn_o[i]+QPoint(width-indent,height/2);
        pgn[2][i]=pgn_ov[i]+QPoint(width/2,indent);
        pgn[3][i]=-pgn_ov[i]+QPoint(width/2,height-indent);
    }



    QPainter* painter=new QPainter(this);




    painter->setBrush(Qt::black);
    painter->drawRect(QRect(this->geometry()));

    for(int i=0;i<4;i++)
    {
        QPainterPath path;
        path.addPolygon(pgn[i]);

        painter->fillPath(path,QBrush(Qt::white));
    }
    //    painter->drawPolygon(pgn[0]);
    painter->setPen(QPen(QColor(0,0,255)));
    //    painter->drawLine(QPoint(0,0),QPoint(20,cc));

    delete painter;
}
