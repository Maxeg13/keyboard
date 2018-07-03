#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QPushButton>
#include <QMainWindow>
#include <QGridLayout>
#include <QSlider>
#include "udp.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    myUDP* udp;
//    QPushButton* proxy_b;
    QPushButton* start_b;
    QPushButton *emulate_b1, *emulate_b2;
//    void keyPressEvent(QKeyEvent *event);
    explicit MainWindow(QWidget *parent = 0);
//    ~MainWindow();
    QSlider* sr_y_c;

public slots:
    void checkChanged();
    void start_released();
    void refr();
    void emitAddr();

    void closeEvent(QCloseEvent *);
    void emulateChange1();
//    void emulation();
private:
signals:
    void addr_sgn(QString );
    void close_sgn();


};

#endif // MAINWINDOW_H
