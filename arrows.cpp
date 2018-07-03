#include "arrows.h"
#include <QTimer>
#include <QPainter>
#include <QPolygon>
QTimer* timer;
int cc;
//bool checked[4]={0,0,0,0};
int indent=100;
float shift_c[4];
int cnt_ar[4];
//QTimer
QPolygon pgn_o, pgn_ov;
QPolygon  pgn[4];
QColor band1_clr, band2_clr[4];
int width_ar=100;
int thick=width_ar*0.5;
int height_ar=190;
QLinearGradient gradient1;
QBrush brush;
QPainterPath path;
extern bool arrows_checked;
Arrows::Arrows(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);

    //    setWindowFlags();
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    band1_clr=QColor(100,255,100);
    //    band2_clr=QColor(255,255,255);
    pgn_o.resize(0);
    pgn_o.resize(0);

    pgn_o.push_back(QPoint(width_ar/2,-height_ar/2));
    pgn_o.push_back(QPoint(width_ar/2-thick,-height_ar/2));
    //    pgn_o.push_back(QPoint(0,-height_ar/2));
    pgn_o.push_back(QPoint(-width_ar/2,0));
    pgn_o.push_back(QPoint(width_ar/2-thick,height_ar/2));
    pgn_o.push_back(QPoint(width_ar/2,height_ar/2));
    pgn_o.push_back(QPoint(-width_ar/2+thick,0));

    thick=width_ar*0.5;
    float scr=1.2;
    pgn_ov.push_back(QPoint(-height_ar/2*scr,width_ar/2));
    pgn_ov.push_back(QPoint(-height_ar/2*scr,width_ar/2-thick));
    //    pgn_o.push_back(QPoint(0,-height_ar/2));
    pgn_ov.push_back(QPoint(0,-width_ar/2));
    pgn_ov.push_back(QPoint(height_ar/2*scr,width_ar/2-thick));
    pgn_ov.push_back(QPoint(height_ar/2*scr,width_ar/2));
    pgn_ov.push_back(QPoint(0,-width_ar/2+thick));

    for(int i=0;i<4;i++)
        pgn[i].resize(6);


    timer = new QTimer(this);
    timer->setInterval(42);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));


}


void Arrows::paintEvent(QPaintEvent *e)
{
    if(arrows_checked)
    {
        cc++;
        int width=this->geometry().width();
        int height=this->geometry().height();
        centre[0]=QPoint(indent,height/2);
        centre[1]=QPoint(width-indent,height/2);
        centre[2]=QPoint(width/2,indent);
        centre[3]=QPoint(width/2,height-indent);

        for(int i=0;i<4;i++)
        {
            //        checked[0]=1;
            if(checked[i])
                cnt_ar[i]++;
            else cnt_ar[i]=0;

            shift_c[i]=1-exp(-cnt_ar[i]*.4);
        }

        float shift_k=0.16;
        for(int i=0;i<6;i++)
        {
            pgn[0][i]=pgn_o[i]*(1+shift_k*shift_c[0])+centre[0];//left
            pgn[1][i]=-pgn_o[i]*(1+shift_k*shift_c[1])+centre[1];//right
            pgn[2][i]=pgn_ov[i]*(1+shift_k*shift_c[2])+centre[2];//up
            pgn[3][i]=-pgn_ov[i]*(1+shift_k*shift_c[3])+centre[3];//down
        }

        //    arrow_rect[i]=QRect();

        QPainter* painter=new QPainter(this);



        //    painter->setBrush(QColor(0,0,0,255));
        //    painter->drawRect(QRect(this->geometry()));

        for(int i=0;i<4;i++)
        {


            QPainterPath path;
            //        path.swap();
            //        path.swap();
            path.addPolygon(pgn[i]);
            int h=155*shift_c[i]+100;
            band2_clr[i]=QColor(h,h,h,h);

            int grad_space=350;
            gradient1=QLinearGradient();
            switch(i)
            {
            case 0:
                //             gradient1=QLinearGradient(0,height/2,grad_space,height/2);
                gradient1.setStart(0,height/2);
                gradient1.setFinalStop(grad_space,height/2);

                break;
            case 1:
                //             gradient1=QLinearGradient(width,height/2,width-grad_space,height/2);
                gradient1.setStart(width,height/2);
                gradient1.setFinalStop(width-grad_space,height/2);
                break;
            case 2:
                //            gradient1=QLinearGradient(width/2,0,width/2,grad_space);
                gradient1.setStart(width/2,0);
                gradient1.setFinalStop(width/2,grad_space);
                break;
            case 3:
                //            gradient1=QLinearGradient(width/2,height,width/2,height-grad_space);
                gradient1.setStart(width/2,height);
                gradient1.setFinalStop(width/2,height-grad_space);
            }


            int speed=7;
            int interval=100;
            gradient1.setColorAt(((cnt_ar[i]*speed-10+interval)%interval)/(float)interval, band2_clr[i]);
            gradient1.setColorAt(((cnt_ar[i]*speed)%interval)/(float)interval, band1_clr);
            gradient1.setColorAt(((cnt_ar[i]*speed+10)%interval)/(float)interval, band2_clr[i]);

            gradient1.setColorAt(((cnt_ar[i]*speed-40+interval)%interval)/(float)interval, band2_clr[i]);
            gradient1.setColorAt(((cnt_ar[i]*speed-30+interval)%interval)/(float)interval, band1_clr);
            gradient1.setColorAt(((cnt_ar[i]*speed-20+interval)%interval)/(float)interval, band2_clr[i]);


            //        brush.setColor(gradient1);
            painter->setBrush(gradient1);
            QPen pen(QColor(0,0,0,h));
            pen.setWidth(4);
            painter->setPen(pen);
            painter->drawPolygon(pgn[i]);
            //        painter->fillPath(path,QBrush(QColor(h,h,h)));
        }
        //    painter->drawPolygon(pgn[0]);
        //    painter->setPen(QPen(QColor(0,0,255)));
        //    painter->drawLine(QPoint(0,0),QPoint(20,cc));

        delete painter;
    }
}
