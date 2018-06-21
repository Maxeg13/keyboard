#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
#include <QDialog>
#include "Arrows.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Arrows ars;
    MainWindow w;
    ars.showFullScreen();
    w.show();


    return a.exec();
}
