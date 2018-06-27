#ifndef RECEIVER_H
#define RECEIVER_H



#include <QWidget>
#include "control_rules.h"
using namespace std;
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
class QAction;
QT_END_NAMESPACE

class Receiver : public QWidget
{
    Q_OBJECT

public:
    Receiver(QWidget *parent = 0);
    QTimer* timer;
   public  slots:
    void processPendingDatagrams();
    void setAddr(QString);
    void control();

private:

    QLabel *statusLabel;
    QPushButton *quitButton;
    QUdpSocket *udpSocket, *udpSocketSend;
    signals:
    void sig_out(vector<uint8_t>);

};



#endif // RECEIVER_H
