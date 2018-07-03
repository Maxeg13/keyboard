#include "udp.h"
#include <QTimer>
#include <QtWidgets>
#include <QtNetwork>
#include "arrows.h"
#include <QFile>
//#include "eyetracker.h"
extern INPUT ip;
extern bool lr_checked;
extern bool downup_checked;
extern bool proxy_checked;
extern Arrows* ars;
//INPUT ip2;
char* byteptr;
uint8_t readVar;
QHostAddress* addr_var;
myUDP::myUDP(QWidget *parent)
    : QWidget(parent)
{
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    byteptr=new char;
    //    statusLabel = new QLabel(tr("Listening for broadcasted messages"));
    //    statusLabel->setWordWrap(true);
    QFile inputFile("UDP_settings.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.readLine();
        addr_var=new QHostAddress(remoteAddr=in.readLine()); /*remoteAddr=addr_var->toString();*/
        //        remoteAddr=in.readLine();
        in.readLine();
        readPort=in.readLine();
        in.readLine();
        srdClientPort=in.readLine();
        in.readLine();
        remoteClientPort=in.readLine();
    }
    else
    {
        addr_var=new QHostAddress("127.0.0.2");
        srdClientPort=QString(2222);
        readPort=QString(3333);
        remoteClientPort=QString(4444);
    }
    socketForGettingAny = new QUdpSocket(this);
    socketForGetting = new QUdpSocket(this);



    //    quitButton = new QPushButton(tr("&Quit"));
    srdSocket= new QUdpSocket(this);
    //    simSocket= new QUdpSocket(this);
    //    srdSocket->connectToHost(*hostAddr,49123,QIODevice::WriteOnly);


    timer=new QTimer();
    timer->setInterval(14);
    timer->start();

    connect(timer,SIGNAL(timeout()),this,SLOT(control()));
    //! [1]

    //    socketForGetting->bind(*addr_var,(quint16)(readPort.toInt()));
    socketForGetting->bind(QHostAddress::LocalHost,(quint16)(readPort.toInt()));
    socketForGettingAny->bind(QHostAddress::AnyIPv4,(quint16)(readPort.toInt()));
    connect(socketForGetting, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    connect(socketForGettingAny, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams2()));
}

void myUDP::setAddr(QString s)
{

}

void myUDP::control()
{
    //     controlFromUDP(INPUT& ip,int b1,int sended)
    //    ip.ki.dwFlags =0;
    //    key_map(ip,0);
    //    SendInput(1, &ip, sizeof(INPUT));

    //    if(proxy_checked)
    //            for (int b1=1;b1<5;b1++)
    //            {
    //                controlFromUDP( ip,b1,0);
    //            }
    //    *byteptr=2;
    //    QByteArray ar;
    //    ar.push_back(2);
    //            if(srdSocket!=NULL)
    //                srdSocket->write(byteptr);
    //        srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
    uint8_t b1=0;
    int noth=0;
    static int cnt1;

    for(int b=0;b<4;b++)
    {
        if(ars->checked[b])
        {
            switch(b)
            {
            case 2:b1=1;break;
            case 1:b1=4;break;
            case 0:b1=3;break;
            case 3:b1=2;break;
            }
        }
    }


//    for(int b=0;b<4;b++)
    {/////
        //        if (b1==0)
        //            b1=5;
        //        qDebug()<<b1;
        if(b1==0)
        {
            QByteArray ar;
            ar.push_back(5);
            //    qDebug()<<b1;
            srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
            //            srdSocket->writeDatagram(ar,QHostAddress("192.168.1.3"),49123);
            srdSocket->writeDatagram(ar,*addr_var,remoteClientPort.toInt());

        }
        else
        {
            //    b1=3;
            QByteArray ar;
            ar.push_back(b1);
            //    qDebug()<<b1;
            srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
            //            srdSocket->writeDatagram(ar,QHostAddress("192.168.0.104"),remoteClientPort.toInt());
            srdSocket->writeDatagram(ar,*addr_var,remoteClientPort.toInt());

        }

    }

}

void myUDP::localOrHost(bool b)
{
    //    disconnect(socketForGetting, SIGNAL(readyRead()),
    //            0, 0);
    //    delete socketForGetting;
    //    socketForGetting=new QUdpSocket(this);
    //    if(b)
    //        socketForGetting->bind(QHostAddress::LocalHost,(quint16)(readPort.toInt()));
    //    else
    //        socketForGetting->bind(QHostAddress::Broadcast,(quint16)(readPort.toInt()));
    //    connect(socketForGetting, SIGNAL(readyRead()),
    //            this, SLOT(processPendingDatagrams()));
}

void myUDP::processPendingDatagrams()
{
    //    qDebug()<<"yes";ddd
    static int ptr=0;
    static uint8_t key=255;
    static int S=4;
    static vector<byte> data;
    while (socketForGetting->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketForGetting->pendingDatagramSize());
        socketForGetting->readDatagram(datagram.data(), datagram.size());
        //        statusLabel->setText(tr("Received datagram: \"%1\"")
        //                             .arg(datagram.data()));


        for(int i=0;i<datagram.size();i++)
        {
            readVar=datagram[i];
            controlFromUDP( ip,readVar,1);
            //            *byteptr=readVar;
            QByteArray ar;
            ar.push_back(readVar);

        }
    }



}

void myUDP::processPendingDatagrams2()
{
    //    qDebug()<<"yes";ddd
    static int ptr=0;
    static uint8_t key=255;
    static int S=4;
    static vector<byte> data;


    while (socketForGettingAny->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketForGettingAny->pendingDatagramSize());
        socketForGettingAny->readDatagram(datagram.data(), datagram.size());
        //        statusLabel->setText(tr("Received datagram: \"%1\"")
        //                             .arg(datagram.data()));


        for(int i=0;i<datagram.size();i++)
        {
            readVar=datagram[i];
            controlFromUDP( ip,readVar,1);
            //            *byteptr=readVar;
            QByteArray ar;
            ar.push_back(readVar);

        }
    }

}
