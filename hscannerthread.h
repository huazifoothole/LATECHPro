#ifndef HSCANNERTHREAD_H
#define HSCANNERTHREAD_H

#include <QDebug>
#include <QThread>
#include "hscanner.h"

class HScannerThread : public QThread
{
    Q_OBJECT

public:
    HScannerThread(QString);
    ~HScannerThread();

private:

public:
    bool InitDevice(QString);
    virtual void run();
    bool isWork;
    QString BCRtype;
    HScanner *hscanner;


    bool StartRun();
    bool StopRun();


signals:
    void showTicketInfo(char *,int);
    void showText(char *);
    void showText_(char *);
};

#endif // HSCANNERTHREAD_H
