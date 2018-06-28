#ifndef myUDP_H
#define myUDP_H



#include <QWidget>
#include "control_rules.h"
using namespace std;
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
class QAction;
QT_END_NAMESPACE

class myUDP : public QWidget
{
    Q_OBJECT

public:
    QString readPort;
    QString srdClientPort;
    myUDP(QWidget *parent = 0);
    QTimer* timer;
   public  slots:
    void processPendingDatagrams();
    void setAddr(QString);
    void control();

private:

    QLabel *statusLabel;
    QPushButton *quitButton;
    QUdpSocket *socketForGetting, *srdSocket;
    signals:
    void sig_out(vector<uint8_t>);

};



#endif // myUDP_H
