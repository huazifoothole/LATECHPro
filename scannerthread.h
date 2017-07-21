#ifndef SCANNERTHREAD_H
#define SCANNERTHREAD_H

#include <QDebug>
#include <QThread>
#include <QPainter>
#include <QImage>
#include "scanner.h"
#include"zdybranddig.h"

class ScannerThread : public QThread
{
    Q_OBJECT

public:
    ScannerThread();
    ~ScannerThread();

public:
    bool InitDevice();
    virtual void run();
    bool isWork;
    Scanner *scanner;
    zdyBrandDig *zdyBrand;

    bool ydjFlag;
    bool yqxFlag;
    bool zdyFlag;
    QString zdyText;
    QString zdySetPos;

    void brandPrnData(int,QString);
    void createBmp(char *,QString);
    bool getBmpData(char *, unsigned char *);

//public slots:
    bool StartRun();
    bool StopRun();

    public slots:
// void   posChoice_(QString);

signals:
    void showTicketInfo(  char *,int);
    void showText(char *);
    void showText_(char *);
};

#endif // SCANNERTHREAD_H
