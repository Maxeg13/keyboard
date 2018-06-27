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
bool emulate_from_tracker_on=1;
bool lr_checked=1;
bool downup_checked=1;

//extern QTextEdit* error_TE;

QLabel* UDP_label;
MyThread* thread_main;
QGroupBox* tracker_box;
QGroupBox* UDP_box;
QCheckBox *downup_check, *lr_check;
QGridLayout lyt1;
QGridLayout lyt2;
QTimer* timer_emul;
QLineEdit* LE_addr;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    thread_main=new MyThread();
    UDP_label=new QLabel;
    UDP_label->setText("host address: 50123\nclient address:");

    QFile inputFile("clientAddr.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       in.readLine();
       LE_addr=new QLineEdit(in.readLine());
    }
    else
    LE_addr=new QLineEdit("50123");
//    LE_addr->setT
//    UDP_box=new QGroupBox(tr(""))
    tracker_box=new QGroupBox(tr("tracker->keyboard"));
    UDP_box=new QGroupBox(tr("UDP->keyboard"));

    start_b=new QPushButton(tr("&capture eyetracker"));
    emulate_b1=new QPushButton(tr("emu&late on"));

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
    GL->addWidget(UDP_box,0,1);
    GL->addWidget(start_b,1,0);
    GL->addWidget(emulate_b1,1,1);

    downup_check=new QCheckBox("upward-back");
    downup_check->setChecked(true);
    lr_check=new QCheckBox("left-right");
    lr_check->setChecked(true);

    tracker_box->setLayout(&lyt1);
    lyt1.addWidget(sr_y_c,0,0,3,1);
    lyt1.addWidget(downup_check,0,1);
    lyt1.addWidget(lr_check,1,1);
//    lyt1.addWidget(emulate_b1,2,1);

    UDP_box->setLayout((&lyt2));
    lyt2.addWidget(UDP_label,0,0);
    lyt2.addWidget(LE_addr,1,0);
//    lyt2.addWidget(emulate_b2,2,1);
//    GL->addWidget(thread_main->error_TE,2,1,1,1);

    this->setMinimumWidth(350);
//    core_func();
//    timer_emul=new QTimer;
//    timer_emul->setInterval(4);
//    timer_emul->start();
    REC=new Receiver();

    REC->setAddr(LE_addr->text());

    setCentralWidget(centralWidget1);
    connect(lr_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(downup_check,SIGNAL(stateChanged(int)),this,SLOT(checkChanged()));
    connect(start_b,SIGNAL(released()),this,SLOT(start_released()));
    connect(sr_y_c,SIGNAL(valueChanged(int)),this,SLOT(refr()));
    connect(emulate_b1,SIGNAL(released()),this,SLOT(emulateChange1()));
//    connect(emulate_b2,SIGNAL(released()),this,SLOT(emulateChange1()));
    connect(LE_addr,SIGNAL(returnPressed()),this,SLOT(emitAddr()));
    connect(this,SIGNAL(addr_sgn(QString)),REC,SLOT(setAddr(QString)));
//    connect(REC,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));
//    connect(timer_emul,SIGNAL(timeout()),this,SLOT(emulation()));
//    emit addr_sgn(LE_addr->text());
}

void MainWindow::checkChanged()
{
    lr_checked=lr_check->isChecked();
    downup_checked=downup_check->isChecked();
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::emulateChange1()
{
    emulate_from_tracker_on=!emulate_from_tracker_on;
    if(emulate_from_tracker_on)
        emulate_b1->setText(QString("emulate on"));
    else
        emulate_b1->setText(QString("emulate off"));
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

