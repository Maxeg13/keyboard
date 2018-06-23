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
    Receiver* REC;
    QPushButton* start_b;
    QPushButton* emulate_b;
    void keyPressEvent(QKeyEvent *event);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSlider* sr_y_c;

public slots:
    void checkChanged();
    void btn_released();
    void refr();

    void closeEvent(QCloseEvent *);
    void emulateChange();
    void emulation();
private:
signals:
    void close_sgn();


};

#endif // MAINWINDOW_H
