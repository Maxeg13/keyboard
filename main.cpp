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
    QObject::connect(&w,SIGNAL(close_sgn()),&a,SLOT(closeAllWindows()));

    return a.exec();
}
