#include "udp.h"

#include <QtWidgets>
#include <QtNetwork>


uint8_t readVar;
Receiver::Receiver(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Listening for broadcasted messages"));
    statusLabel->setWordWrap(true);

    quitButton = new QPushButton(tr("&Quit"));

    //! [0]
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(6321, QUdpSocket::ShareAddress);
    //! [0]

    //! [1]
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    //! [1]
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Receiver"));
}

void Receiver::processPendingDatagrams()
{
    static int ptr=0;
    static uint8_t key=255;
    static int S=6;
    static vector<byte> data;
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        //        statusLabel->setText(tr("Received datagram: \"%1\"")
        //                             .arg(datagram.data()));




        for(int i=0;i<datagram.size();i++)
        {
            readVar=datagram[i];



            if(ptr==(S-1))
            {
                if(readVar==key)
                {
                    //                                qDebug()<<(uint8_t)readVar<<" ptr="<<(int8_t)ptr<<"\n";
//                    qDebug()<<data.size();
                    emit sig_out(data);
                    data.resize(0);
                    ptr=0;
                }
            }
            else
            {
                //                            qDebug()<<(uint8_t)readVar<<" ptr="<<(int8_t)ptr<<"\n";
                data.push_back(readVar);
                ptr++;
            }
        }

    }

}
