#ifndef RECEIVER_H
#define RECEIVER_H



#include <QWidget>
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

private slots:
    void processPendingDatagrams();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QUdpSocket *udpSocket;
    signals:
    void sig_out(vector<uint8_t>);

};



#endif // RECEIVER_H
