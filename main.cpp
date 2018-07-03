#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
#include <QDialog>
#include "Arrows.h"
#include "udp.h"
Arrows* ars;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ars=new Arrows();

    MainWindow w;


    w.show();
       ars->showFullScreen();
    QObject::connect(&w,SIGNAL(close_sgn()),&a,SLOT(closeAllWindows()));

    return a.exec();
}
