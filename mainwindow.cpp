#include "mainwindow.h"
//#include"virtual_control.h"
#include <QTextEdit>
#include "mythread.h"
#include <QTimer>
int cnt;
float y_centre=.6;
float x_centre=0.5;
bool write_on=1;


MyThread* thread_main;
QTextEdit* error_TE;
QTimer* timer_emul;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    REC=new Receiver();
    start_b=new QPushButton("capture keyboard");
    write_b=new QPushButton("write on");

    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    sr_y_c=new QSlider();
    //    sr_x_c->setGeometry(QRect(0,0,200,50));
    sr_y_c->setOrientation(Qt::Vertical);
    //    sr_y_c->set
    sr_y_c->setRange(0,255);
    sr_y_c->setValue(60);

    error_TE=new QTextEdit;

    GL->addWidget(sr_y_c,0,0,3,1);
    GL->addWidget(start_b,0,1);
    GL->addWidget(write_b,1,1);
    GL->addWidget(error_TE,2,1,1,1);
    error_TE->setMinimumHeight(200);
    this->setMinimumWidth(350);
//    core_func();
    timer_emul=new QTimer;
    timer_emul->setInterval(4);
    timer_emul->start();

    setCentralWidget(centralWidget1);
    connect(start_b,SIGNAL(released()),this,SLOT(btn_released()));
    connect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    connect(write_b,SIGNAL(released()),this,SLOT(writeChange()));
    connect(REC,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));
//    connect(timer_emul,SIGNAL(timeout()),this,SLOT(emulation()));
}

void MainWindow::btn_released()
{
    if(cnt==0)
    {
        start_b->setText(QString("emulation works"));
        //    start_b->setEnabled(0);
        thread_main=new MyThread();
        thread_main->start();
    }
    if(cnt==1)
    {
//        core_close();
        close();
    }
    cnt++;
}

void MainWindow::refr()
{
    y_centre=(255-sr_y_c->value())/255.;
    qDebug()<<y_centre;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //    qDebug()<<"hi";
}

void MainWindow::writeChange()
{
    write_on=!write_on;
    if(write_on)
        write_b->setText(QString("write on"));
    else
        write_b->setText(QString("write off"));
}

void MainWindow::emulation()
{
//    control();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit close_sgn();
}

MainWindow::~MainWindow()
{

}

