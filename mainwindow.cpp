//any, sleep 3,
#include "mainwindow.h"
//#include"virtual_control.h"
#include <QTextEdit>
#include <QLineEdit>
#include "mythread.h"
#include <QTimer>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QUdpSocket>
#include <QFile>
int cnt;
float y_centre=.6;
float x_centre=0.5;
bool emulate_on;
bool lr_checked=1;
bool kb_layout_checked=1;
bool downup_checked=1;
int PWM_bound=7;//will be changed below
int PWM_accum=0;
float PWM_accum_fl=0;

//extern QTextEdit* error_TE;

QLabel* UDP_label;
MyThread* thread_main;
QGroupBox* tracker_box;
QGroupBox* UDP_box;
QCheckBox *downup_check, *lr_check, *kb_layout_check, *arrows_check;
QCheckBox *local_h_check, *remote_h_check;
QTimer* timer_emul;
QLineEdit* LE_addr;
QGridLayout lyt1;
QGridLayout lyt2;

QString sendHost;
bool arrows_checked=1;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    thread_main=new MyThread();
    UDP_label=new QLabel;

    tracker_box=new QGroupBox(tr("tracker->keyboard"));
    UDP_box=new QGroupBox(tr("UDP->keyboard"));

    start_b=new QPushButton(tr("&capture eyetracker"));
    emulate_b1=new QPushButton(tr("emu&late off"));

    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget(this);
    centralWidget1->setLayout(GL);

    local_h_check=new QCheckBox("read from local host");
    local_h_check->setChecked(false);

    remote_h_check=new QCheckBox("read from remote host");
    remote_h_check->setChecked(false);

    arrows_check=new QCheckBox("draw arrows");
    arrows_check->setChecked(true);
    //    arrows_check->setDisabled(true);
    kb_layout_check=new QCheckBox("wasd keyboard layout");
    kb_layout_check->setChecked(1);
    downup_check=new QCheckBox("upward-back");
    downup_check->setChecked(true);
    lr_check=new QCheckBox("left-right");
    lr_check->setChecked(true);

    sr_PWM=new QSlider(this);
    sr_PWM->setRange(0,10);
    sr_PWM->setValue(8);
    sr_PWM->setOrientation(Qt::Horizontal);


    sr_y_c=new QSlider(this);
    //    sr_x_c->setGeometry(Qudpt(0,0,200,50));
    sr_y_c->setOrientation(Qt::Vertical);
    //    sr_y_c->set
    sr_y_c->setRange(0,255);
    sr_y_c->setValue(60);

    GL->addWidget(tracker_box,0,0);
    GL->addWidget(UDP_box,0,1);
    GL->addWidget(start_b,1,0);
    GL->addWidget(emulate_b1,1,1);
    GL->addWidget(kb_layout_check,2,0);
    GL->addWidget(arrows_check,2,1);

    tracker_box->setLayout(&lyt1);
    lyt1.addWidget(sr_y_c,0,0,3,1);
    lyt1.addWidget(downup_check,0,1);
    lyt1.addWidget(lr_check,1,1);
    //    lyt1.addWidget(emulate_b1,2,1);

    UDP_box->setLayout((&lyt2));
    lyt2.addWidget(UDP_label,0,0);    
    lyt2.addWidget(sr_PWM,1,0);


    this->setMinimumWidth(350);
    udp=new myUDP(this);

    UDP_label->setText(QString("remote address: ")+udp->remoteAddr+QString("\nreceive port: ")+udp->readPort+QString("\nlocal client port: ")+udp->srdClientPort+
                       QString("\nremote client port: ")+udp->remoteClientPort+QString("\n\n     PWM value:"));
    //    udp->setAddr(LE_addr->text());

    setCentralWidget(centralWidget1);
    connect(lr_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(downup_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(start_b,SIGNAL(released()),this,SLOT(start_released()));
    connect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    connect(emulate_b1,SIGNAL(released()),this,SLOT(emulateChange1()));
    connect(arrows_check,SIGNAL(stateChanged(int)),this, SLOT(checkChanged()));
    connect(arrows_check,SIGNAL(stateChanged(int)),this, SLOT(checkChanged()));
    connect(this,SIGNAL(addr_sgn(QString)),udp,SLOT(setAddr(QString)));
    connect(kb_layout_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(sr_PWM,SIGNAL(valueChanged(int)),this,SLOT(PWM_changed()));
}

void MainWindow::checkChanged()
{
    lr_checked=lr_check->isChecked();
    downup_checked=downup_check->isChecked();
    arrows_checked=arrows_check->isChecked();
    kb_layout_checked=kb_layout_check->isChecked();
    //    udp->localOrHost(local_h_check->isChecked());



    //    qDebug()<<kb_layout_checked;
}

void MainWindow::emitAddr()
{
    emit addr_sgn(LE_addr->text());
}

void MainWindow::PWM_changed()
{
    PWM_bound=sr_PWM->value();
}

void MainWindow::start_released()
{
    if(cnt==0)
    {
        start_b->setText(QString("emulation works"));
        //    start_b->setEnabled(0);
        start_b->setDisabled(true);
        thread_main->start();
    }
    if(cnt==1)
    {
        //        core_close();
        //        close();
    }
    cnt++;
}

void MainWindow::refr()
{
    y_centre=(255-sr_y_c->value())/255.;
    qDebug()<<y_centre;
}

//void MainWindow::keyPressEvent(QKeyEvent *event)
//{

//}

void MainWindow::emulateChange1()
{
    emulate_on=!emulate_on;
    if(emulate_on)
        emulate_b1->setText(QString("emulate on"));
    else
        emulate_b1->setText(QString("emulate off"));
}

//void MainWindow::emulation()
//{
//    //    control();
//}

void MainWindow::closeEvent(QCloseEvent *e)
{
    qDebug()<<"bye!";
    disconnect(lr_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    disconnect(downup_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    disconnect(start_b,SIGNAL(released()),this,SLOT(start_released()));
    disconnect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    disconnect(emulate_b1,SIGNAL(released()),this,SLOT(emulateChange1()));
    disconnect(arrows_check,SIGNAL(stateChanged(int)),this, SLOT(checkChanged()));
    disconnect(arrows_check,SIGNAL(stateChanged(int)),this, SLOT(checkChanged()));
    disconnect(this,SIGNAL(addr_sgn(QString)),0,0);
    disconnect(kb_layout_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    delete thread_main;
    emit close_sgn();
}

//MainWindow::~MainWindow()
//{

//}

