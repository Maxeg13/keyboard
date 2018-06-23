#include "mainwindow.h"
//#include"virtual_control.h"
#include <QTextEdit>
#include "mythread.h"
#include <QTimer>
#include <QGroupBox>
#include <QCheckBox>
int cnt;
float y_centre=.6;
float x_centre=0.5;
bool emulate_on=1;
bool lr_checked=1;
bool updown_checked=1;

//extern QTextEdit* error_TE;


MyThread* thread_main;
QGroupBox* tracker_box;
QGroupBox* UDP_box;
QCheckBox *updown_check, *lr_check;
QGridLayout lyt1;
QGridLayout lyt2;
QTimer* timer_emul;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    thread_main=new MyThread();

//    UDP_box=new QGroupBox(tr(""))
    tracker_box=new QGroupBox(tr("tracker"));
    UDP_box=new QGroupBox(tr("UDP"));
    REC=new Receiver();
    start_b=new QPushButton(tr("&capture keyboard"));
    emulate_b=new QPushButton(tr("emu&late on"));

    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    sr_y_c=new QSlider();
    //    sr_x_c->setGeometry(QRect(0,0,200,50));
    sr_y_c->setOrientation(Qt::Vertical);
    //    sr_y_c->set
    sr_y_c->setRange(0,255);
    sr_y_c->setValue(60);

//    error_TE=new QTextEdit;

//    GL->addWidget(sr_y_c,0,0,3,1);

    GL->addWidget(tracker_box,0,0);
    GL->addWidget(start_b,1,0);
    GL->addWidget(emulate_b,1,1);

    updown_check=new QCheckBox("upward-back");
    updown_check->setChecked(true);
    lr_check=new QCheckBox("left-right");
    lr_check->setChecked(true);

    tracker_box->setLayout(&lyt1);
    lyt1.addWidget(sr_y_c,0,0,3,1);
    lyt1.addWidget(updown_check,0,1);
    lyt1.addWidget(lr_check,1,1);

    UDP_box->setLayout((&lyt2));
//    lyt2.addWidget();
//    GL->addWidget(thread_main->error_TE,2,1,1,1);

    this->setMinimumWidth(350);
//    core_func();
//    timer_emul=new QTimer;
//    timer_emul->setInterval(4);
//    timer_emul->start();

    setCentralWidget(centralWidget1);
    connect(lr_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(start_b,SIGNAL(released()),this,SLOT(btn_released()));
    connect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    connect(emulate_b,SIGNAL(released()),this,SLOT(emulateChange()));
    connect(REC,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));
//    connect(timer_emul,SIGNAL(timeout()),this,SLOT(emulation()));
}

void MainWindow::checkChanged()
{
    lr_checked=lr_check->isChecked();
    updown_checked=updown_check->isChecked();
}

void MainWindow::btn_released()
{
    if(cnt==0)
    {
        start_b->setText(QString("emulation works"));
        //    start_b->setEnabled(0);

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

}

void MainWindow::emulateChange()
{
    emulate_on=!emulate_on;
    if(emulate_on)
        emulate_b->setText(QString("emulate on"));
    else
        emulate_b->setText(QString("emulate off"));
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

