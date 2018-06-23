#ifndef THREAD_H
#define THREAD_H
#include "eyetracker.h"
#include <QThread>
#include <QTextEdit>

class MyThread : public QThread
{
    Q_OBJECT
public:
    QTextEdit* error_TE;
    MyThread();
protected:

    void run();


    //public Q_SLOTS:
    //    void start();
} ;


#endif // THREAD_H
