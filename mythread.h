#ifndef THREAD_H
#define THREAD_H
#include "virtual_control.h"
#include <QThread>
class MyThread : public QThread
{
    Q_OBJECT
public:
//    explicit MyThread();
protected:

    void run()
    {core_func();}

//public Q_SLOTS:
//    void start();
} ;


#endif // THREAD_H
