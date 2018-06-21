#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QPushButton>
#include <QMainWindow>
#include <QGridLayout>
#include <QSlider>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton* start_b;
    QPushButton* write_b;
    void keyPressEvent(QKeyEvent *event);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSlider* sr_y_c;

public slots:
    void btn_released();
    void refr();


    void writeChange();
private:



};

#endif // MAINWINDOW_H
