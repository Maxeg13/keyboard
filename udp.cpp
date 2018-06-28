#include "udp.h"
#include <QTimer>
#include <QtWidgets>
#include <QtNetwork>
#include "arrows.h"
#include <QFile>

extern Arrows* ars;
INPUT ip2;
char* byteptr;
uint8_t readVar;
QHostAddress* hostAddr;
myUDP::myUDP(QWidget *parent)
    : QWidget(parent)
{
    byteptr=new char;
    //    statusLabel = new QLabel(tr("Listening for broadcasted messages"));
    //    statusLabel->setWordWrap(true);
    QFile inputFile("UDP_settings.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
//               in.readLine();
        hostAddr=new QHostAddress(in.readLine());
        srdClientPort=in.readLine();
        readPort=in.readLine();
    }
    else
    {
        hostAddr=new QHostAddress("127.0.0.1");
        srdClientPort=QString(2222);
        readPort=QString(3333);
    }

    socketForGetting = new QUdpSocket(this);

    //    quitButton = new QPushButton(tr("&Quit"));
    srdSocket= new QUdpSocket(this);

//    srdSocket->connectToHost(*hostAddr,49123,QIODevice::WriteOnly);


    timer=new QTimer();
    timer->setInterval(10);
    timer->start();

    connect(timer,SIGNAL(timeout()),this,SLOT(control()));
    //! [1]


    socketForGetting->bind(readPort.toInt(), QUdpSocket::ShareAddress);
    connect(socketForGetting, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));


}

void myUDP::setAddr(QString s)
{

}

void myUDP::control()
{
    for (int b1=1;b1<6;b1++)
    {
        controlFromUDP( ip2,b1,0);

    }

    for(int b=0;b<4;b++)
    {
        if(ars->checked[b])
        {       char b1;
            switch(b)
            {
            case 0:b1=2;break;
            case 1:b1=3;break;
            case 2:b1=4;break;
            case 3: b1=5;break;
            }
            *byteptr=b1;
            QByteArray ar;
            ar.push_back(b1);
//            if(srdSocket!=NULL)
//                srdSocket->write(byteptr);
                srdSocket->writeDatagram(ar,QHostAddress("127.0.0.1"),srdClientPort.toInt());
        }
    }

}

void myUDP::processPendingDatagrams()
{
    //    qDebug()<<"yes";
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
            controlFromUDP( ip2,readVar,1);
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
