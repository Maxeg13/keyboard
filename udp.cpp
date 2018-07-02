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
        addr_var=new QHostAddress(in.readLine()); /*remoteAddr=addr_var->toString();*/
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

    socketForGetting = new QUdpSocket(this);

    //    quitButton = new QPushButton(tr("&Quit"));
    srdSocket= new QUdpSocket(this);
    simSocket= new QUdpSocket(this);
    //    srdSocket->connectToHost(*hostAddr,49123,QIODevice::WriteOnly);


    timer=new QTimer();
    timer->setInterval(20);
    timer->start();

    connect(timer,SIGNAL(timeout()),this,SLOT(control()));
    //! [1]

    //    socketForGetting->bind(*addr_var,(quint16)(readPort.toInt()));
    //    socketForGetting->bind(QHostAddress::LocalHost,(quint16)(readPort.toInt()));
    socketForGetting->bind(QHostAddress::Any,(quint16)(readPort.toInt()));
    connect(socketForGetting, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
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
    //        for (int b1=1;b1<6;b1++)
    //        {
    //            controlFromUDP( ip2,b1,0);
    //        }
    //    *byteptr=2;
    //    QByteArray ar;
    //    ar.push_back(2);
    //            if(srdSocket!=NULL)
    //                srdSocket->write(byteptr);
    //        srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
    uint8_t b1=0;
    int noth=0;
    for(int b=0;b<4;b++)
    {
        if(ars->checked[b])
        {
            switch(b)
            {
            case 2:b1=1;
                //                controlFromUDP(ip, b1, 0);
                break;
            case 1:b1=4;break;
            case 0:b1=3;break;
            case 3:b1=2;break;
            }

        }

        /////
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
            //            if(srdSocket!=NULL)
            //                srdSocket->write(byteptr);
            //            if(proxy_checked)
            {
                //                srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
                //                simSocket->writeDatagram(ar,*addr_var,remoteClientPort.toInt());
                //                if(readVar==5)
                //                {
                //                    srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
                //                    srdSocket->writeDatagram(ar,QHostAddress("192.168.1.3"),49123);
                //                }
            }
            //            qDebug()<<datagram.size();
        }



        //        for(int i=0;i<datagram.size();i++)
        //        {
        //            readVar=datagram[i];
        //            qDebug()<<datagram.size();


        //            if(ptr==(S-1))
        //            {
        //                if(readVar==key)
        //                {
        //                    //                                qDebug()<<(uint8_t)readVar<<" ptr="<<(int8_t)ptr<<"\n";
        ////                    qDebug()<<data.size();
        //                    emit sig_out(data);
        //                    data.resize(0);
        //                    ptr=0;
        //                }
        //            }
        //            else
        //            {
        //                //                            qDebug()<<(uint8_t)readVar<<" ptr="<<(int8_t)ptr<<"\n";
        //                data.push_back(readVar);
        //                ptr++;
        //            }
        //        }

    }

}
