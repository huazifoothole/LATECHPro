#include "hscannerthread.h"


#define SO_PATH		"/home/oneu/ccpos/dist/driver/la"

HScannerThread::HScannerThread(QString type) :
        QThread(),
hscanner(NULL)
{
    BCRtype=type;
    isWork = false;


}

HScannerThread::~HScannerThread()
{

    if (isWork == true)
    {
        StopRun();
        isWork = false;
    }

    if (hscanner)
    {
        delete hscanner;
        hscanner = NULL;
    }
}

bool HScannerThread::InitDevice(QString dev)
{

    hscanner = new HScanner(dev);
   // bool initflag=false;
    int count =0;
    while(1)
    {
         if(count > 1)
         break;
         int ret = 0;
         {
          ret = hscanner->BCRInit(NULL,  SO_PATH,SO_PATH);
         }
         if (0 == ret)
         {
             qDebug() << "HScanner Init Success! " << ret;
             hscanner->BCREnableCode(56);//开启DataMatrix
             hscanner->BCREnableCode(57);//开启QR码
             return true;
         }
         else
         {
             qDebug() << "HScanner Init Failed! " << ret;
             count++;
//             sleep(1);
         }
    }

    qDebug() << "HScanner Init Failed! ";
    return false;
}


bool HScannerThread::StartRun()
{
    qDebug() << "HScannerThread StartRun!";
    isWork = true;
    //start()内部调用run()启动线程执行
    start();
    return true;
}


bool HScannerThread::StopRun()
{
    isWork = false;
    if (hscanner->BCRStopScan())
    {
        qDebug() << "HScanner Stop Success!";
    }
    else
    {
        qDebug() << "HScanner Stop Failed!";
    }

    qDebug() << "HScannerThread StopRun!";
    //中止线程
   // terminate();
   // wait();

    return true;
}

void HScannerThread::run()
{
    char errStr[256] = {0};
    unsigned char ticketInfo[4096] = {0};

    if(pthread_mutex_trylock(&HscannerMutex))
    {
            usleep(200000);
    }

//    if(BCRtype.compare("EM2028") == 0)

        while (isWork)
        {
            //条码枪是否准备好
            if (hscanner->BCRIsReady())
                break;
            qDebug() << "HScanner is not ready !";
             hscanner->BCRGetLastErrorStr(errStr, 256);
            qDebug() << "errStr = " << errStr;
             usleep(300000);//500ms
            int count =0;
            while(count<2)
        {
            int ret = hscanner->BCRInit(NULL, "./conf/hscanner", "./conf/hscanner");
            if (0 == ret)
            {
                qDebug() << "HScanner Init Success! in run" << ret;
                isWork = true;
                break;
            }
            else
            {
                qDebug() << "HScanner Init Failed!  in run" << ret;
                isWork = false;
                count++;
            }
        }
            break;
        }
    qDebug() << "HScanner is ready!!!";

   if(isWork)  //start只需要调用一次
    {
        if (hscanner->BCRStartScan())
        {
            qDebug() << "HScanner Start Scan OK...";
            hscanner->BCREnableBeep();
            emit showText("条码枪开始扫描");
        }
        else
        {
            qDebug() << "HScanner Start Scan Error...";
        }
    }else {
        pthread_mutex_unlock(&HscannerMutex);
        return;
    }

    while (isWork)  //每次循环从ScanIsComplete开始
    {

        while (isWork)
        {

            if (hscanner->BCRScanIsComplete())
            {
               // qDebug() << "HScannerIsComplete is finished";

                if(pthread_mutex_trylock(&HscannerMutex))
                {
                        usleep(20000);
                }
                break;  //跳出该循环
            }
            usleep(50000);  //50ms
        }

        unsigned int length;
        hscanner->BCRGetDataLength(&length);
        if(length <= 0)
          {
                                    continue;
          }
        else
            qDebug()<<"EM2028 get length = "<<length;

        //字符识别
        memset(ticketInfo, 0, sizeof(ticketInfo)); //每次在读票之前需先清空缓存数据
     //   memset(ticketInfo, 0, 1024*sizeof(char));
//        usleep(500000);  //50ms
        int size = hscanner->BCRGetTicketInfo(ticketInfo, sizeof(ticketInfo));
        if (0 != size)
        {
            qDebug() << "BCRGetTicketInfo return size value=" << size;
            hscanner->BCRBeep(0x00);
            qDebug()<<"BCR ticketInfo[0]"<<ticketInfo[0];
           // hscanner->BCRStopScan();
             int type = (int)ticketInfo[0];
             qDebug()<<"BCR scan type="<<type;

            emit showTicketInfo((char*)ticketInfo,type);
        }
        else
        {
            qDebug() << "BCRGetTicketInfo failed!";

        }

        pthread_mutex_unlock(&HscannerMutex);
        usleep(500000);  //每500ms轮询一次

    }
    pthread_mutex_unlock(&HscannerMutex);

#if 0
    else if(BCRtype.compare("HR200") == 0)
    {
        while (isWork)
        {
            if (hscanner->BCRIsReady())
                break;
            errCode = hscanner->BCRGetLastErrorCode();
            qDebug() << "HR200  is not ready !";
            hscanner->BCRGetLastErrorStr(errStr, 256);
            qDebug() << "HR200  is not ready errStr = " << errStr;
            break;
        }

        while (isWork)  //start只需要调用一次
        {

                hscanner->BCREnableBeep();
                break;

        }

        while (isWork)  //每次循环从ScanIsComplete开始
        {
            while (isWork)
            {
                if (hscanner->BCRScanIsComplete())
                {
                    break;  //跳出该循环
                }
                usleep(500000);  //500ms
            }
            qDebug() << "HScannerIsComplete is finished...";


            //字符识别
            memset(ticketInfo, 0, sizeof(ticketInfo)); //每次在读票之前需先清空缓存数据
            usleep(50000);  //50ms
            int size = hscanner->BCRGetTicketInfo(ticketInfo, sizeof(ticketInfo));

            if (0 != size)
            {
                qDebug() << "BCRGetTicketInfo return size value=" << size;
                hscanner->BCRBeep(0x00);
                int type = (int)ticketInfo[0];
                qDebug()<<"hr200 scan type="<<type;
               // hscanner->BCRStopScan();
                emit showTicketInfo((char*)ticketInfo,type);
            }
            else
            {
                qDebug() << "HR200 BCRGetTicketInfo failed!";
            }

            usleep(1000000);  //每500ms轮询一次
            //hscanner->BCRStartScan();
        }
    }
#endif

    //若线程中没有自己定义的槽函数，可以不调用exec(),即不启动事件循环
   // exec();
}
