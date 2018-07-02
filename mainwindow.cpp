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
#include <QFile>
int cnt;
float y_centre=.6;
float x_centre=0.5;
bool emulate_on;
bool lr_checked=1;
bool kb_layout_checked=1;
bool downup_checked=1;

//extern QTextEdit* error_TE;

QLabel* UDP_label;
MyThread* thread_main;
QGroupBox* tracker_box;
QGroupBox* UDP_box;
QCheckBox *downup_check, *lr_check, *kb_layout_check, *proxy_check;
QGridLayout lyt1;
QGridLayout lyt2;
QTimer* timer_emul;
QLineEdit* LE_addr;
QString sendHost;
bool proxy_checked=1;
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
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    proxy_check=new QCheckBox("is proxy:");
    proxy_check->setChecked(true);
    proxy_check->setDisabled(true);
    kb_layout_check=new QCheckBox("wasd keyboard layout");
    kb_layout_check->setChecked(1);
    downup_check=new QCheckBox("upward-back");
    downup_check->setChecked(true);
    lr_check=new QCheckBox("left-right");
    lr_check->setChecked(true);

    sr_y_c=new QSlider();
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
    GL->addWidget(proxy_check,2,1);

    tracker_box->setLayout(&lyt1);
    lyt1.addWidget(sr_y_c,0,0,3,1);
    lyt1.addWidget(downup_check,0,1);
    lyt1.addWidget(lr_check,1,1);
    //    lyt1.addWidget(emulate_b1,2,1);

    UDP_box->setLayout((&lyt2));
    lyt2.addWidget(UDP_label,0,0);


    this->setMinimumWidth(350);
    udp=new myUDP(this);

    UDP_label->setText(QString("remote address: ")+udp->remoteAddr+QString("\nreceive port: ")+udp->readPort+QString("\nlocal client port: ")+udp->srdClientPort+
            QString("\nremote client port: ")+udp->remoteClientPort);
//    udp->setAddr(LE_addr->text());

    setCentralWidget(centralWidget1);
    connect(lr_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(downup_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(start_b,SIGNAL(released()),this,SLOT(start_released()));
    connect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    connect(emulate_b1,SIGNAL(released()),this,SLOT(emulateChange1()));
//    connect(proxy_check,SIGNAL(stateChanged(int)),)
    //    connect(emulate_b2,SIGNAL(released()),this,SLOT(emulateChange1()));
//    connect(LE_addr,SIGNAL(returnPressed()),this,SLOT(emitAddr()));
    connect(this,SIGNAL(addr_sgn(QString)),udp,SLOT(setAddr(QString)));
    connect(kb_layout_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    //    connect(udp,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));
    //    connect(timer_emul,SIGNAL(timeout()),this,SLOT(emulation()));
    //    emit addr_sgn(LE_addr->text());
}

void MainWindow::checkChanged()
{
    lr_checked=lr_check->isChecked();
    downup_checked=downup_check->isChecked();
    proxy_checked=proxy_check->isChecked();
    kb_layout_checked=kb_layout_check->isChecked();
    qDebug()<<kb_layout_checked;
}

void MainWindow::emitAddr()
{
    emit addr_sgn(LE_addr->text());
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
    emit close_sgn();
}

//MainWindow::~MainWindow()
//{

//}

