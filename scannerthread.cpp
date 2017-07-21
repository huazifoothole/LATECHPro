#include "scannerthread.h"
//#include <QMatrix>


#define SO_PATH		"/home/oneu/ccpos/dist/driver/la"

ScannerThread::ScannerThread() :
        QThread(),
        scanner(NULL)
{
    isWork = false;
    ydjFlag = false;
    yqxFlag = false;
    zdyFlag = false;
    zdySetPos="bottom";
    zdyBrand = new zdyBrandDig();
   // connect(zdyBrand,SIGNAL(posChoice_(QString)),this,SLOT(posChoice_(QString)));
}

ScannerThread::~ScannerThread()
{
    ydjFlag = false;
    yqxFlag = false;
    zdyFlag = false;

    if (isWork == true)
    {
        StopRun();
        isWork = false;
    }

    if (scanner)
    {
        delete scanner;
        scanner = NULL;
    }

}

bool ScannerThread::InitDevice()
{
    scanner = new Scanner();

    int ret = scanner->SInit(NULL, SO_PATH, SO_PATH);

    if (0 == ret)
    {
        qDebug() << "Scanner Init Success! " << ret;
        return true;
    }
    else
    {
        qDebug() << "Scanner Init Failed! " << ret;
        return false;
    }
}

bool ScannerThread::StartRun()
{
    qDebug() << "ScannerThread StartRun!";
    isWork = true;
    //start()内部调用run()启动线程执行
    start();
    return true;
}

bool ScannerThread::StopRun()
{
    isWork = false;
    ydjFlag = false;
    yqxFlag = false;
    zdyFlag = false;

    if (scanner->SStop())
    {
        qDebug() << "Scanner Stop Success!";
    }
    else
    {
        qDebug() << "Scanner Stop Failed!";
    }

    qDebug() << "ScannerThread StopRun!";
    //中止线程
    terminate();
    wait();

    return true;
}

void ScannerThread::run()
{
    int errCode = 0;
    char errStr[256] = {0};
    unsigned char ticketInfo[1024] = {0};

    while (isWork)  //start只需要调用一次
    {
        if (scanner->SStart())
        {
            qDebug() << "Scanner Start Scan OK...";
            break;
        }
        else
        {
            qDebug() << "Scanner Start Scan Error...";
        }
    }

    while (isWork)
    {
        if (scanner->ScannerIsReady())
            break;
        errCode = scanner->SGetLastErrorCode();
        qDebug() << "Scanner is not ready !";
        qDebug() << "errCode = " << errCode;
        scanner->SGetLastErrorStr(errStr, 256);
        qDebug() << "errStr = " << errStr;
        usleep(500000);
    }
    qDebug() << "Scanner is ready!!!";

    while (isWork)  //每次循环从ScanIsComplete开始
    {
        while (isWork)
        {
            if (scanner->ScanIsComplete())
            {
                break;  //跳出该循环
            }
            usleep(500000);  //500ms
        }
        qDebug() << "ScannerIsComplete is finished...";

        //字符识别
        memset(ticketInfo, 0, sizeof(ticketInfo)); //每次在读票之前需先清空缓存数据
        usleep(50000);  //50ms

        int size = scanner->SGetTicketInfo(ticketInfo, sizeof(ticketInfo));
        if (0 != size)
        {
              int type = (int)ticketInfo[0];
            qDebug() << "SGetTicketInfo return size value=" << size;
            emit showTicketInfo((char*)ticketInfo, type);
        }
        else
        {
            qDebug() << "SGetTicketInfo failed!";
        }

        if (true == ydjFlag)
        {
            emit showText_("－－［已兑奖］标记打印");
            brandPrnData(1,NULL);
        }
        if (true == yqxFlag)
        {
            emit showText_("－－［已取消］标记打印");
            brandPrnData(3,NULL);
        }
        if (true == zdyFlag)
        {
            emit showText_("－－［自定义］标记打印");
            brandPrnData(0,zdySetPos);
        }

        if (scanner->SRollBack())
        {
            qDebug() << "SRollBack success!";
        }
        else
        {
            qDebug() << "SRollBack failed!";
        }
        usleep(500000);  //每500ms轮询一次
    }

    //若线程中没有自己定义的槽函数，可以不调用exec(),即不启动事件循环
    exec();
}


void ScannerThread::brandPrnData(int index,QString zdyPos)
{
    int xPos = 340;
    int yPos =  68;

   if("center"==zdyPos) //if("Vertical"==zdyPos)
  {
     xPos = 320;
     yPos = 300;
  }
   if("Vertical"==zdyPos)
   {
       xPos = 320;
       yPos = 550;
   }
    int nIndex = index;

    if (0 != nIndex)
    {
        xPos=440;
        yPos=670;
        int ret = scanner->SPrintBrandImage(NULL, nIndex, xPos, yPos);
        if (0==ret || 1513==ret || 1514==ret)
        {
            qDebug() << "SPrintBrandImage success!";
            emit showText_("－－成功");
        }
        else
        {
            qDebug() << "SPrintBrandImage failed!";
            emit showText_("－－失败");
        }
    }
    else
    {
        //自定义BMP
        char bmpPath[128];
        memset(bmpPath, 0, sizeof(bmpPath));
        //QString zdyPos;
      //  zdyBrand->checkBoxChoice(&zdyPos);
        createBmp(bmpPath,zdySetPos);

        char bmpData[1024*64];
        memset(bmpData, 0, sizeof(bmpData));

        if (getBmpData((char *)bmpPath, (unsigned char *)bmpData))
        {
            int ret = scanner->SPrintBrandImage(bmpData, nIndex, xPos, yPos);
            if (0==ret || 1513==ret || 1514==ret)
            {
                qDebug() << "SPrintBrandImage success!";
                emit showText_("－－成功");
            }
            else
            {
                qDebug() << "SPrintBrandImage failed!";
                emit showText_("－－失败");
            }
        }
        else
        {
            qDebug() << "SPrintBrandImage get BMP failed!";
            emit showText_("－－失败");
        }
    }
}

void ScannerThread::createBmp(char *bmpPath,QString zdyPos)
{
            /*char *s=zdyText.toLatin1().data() ;
            char varc1[10],varc2[10],varc3[10];
              sscanf(s,"%s %s %s",varc1,varc2,varc3);
            qDebug("1:%s 2:%s 3:%s",varc1,varc2,varc3);
            qDebug("1=%d \n2=%d\n 3= %d\n ",strlen(varc1),strlen(varc2),strlen(varc3));*/


        int width = 256;
        int heigth = 104;
    //自定义BMP 256 30
        if("bottom"==zdyPos)
    {
       qDebug("it is bottom center");
      width = 256;
      heigth = 96;
    }
   else if("center"==zdyPos)
    {

        qDebug("it is bottom center");
        width = 224;
        heigth = 96;
    }
   else if("Vertical"==zdyPos)
    {
        qDebug("it is Ver center");//240 ,96   128, 240
        width=240;
        heigth=96;
    }
   else qDebug("no accepted zdypos");

   QImage  bmp(width, heigth, QImage::Format_Mono);
   QImage bmp2;
    bmp.fill(1);
    QPainter painter(&bmp);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawLine(0, 0, 0, heigth);
    painter.drawLine(width, 0, width, heigth);

    painter.drawLine(0, 0, width, 0);
    painter.drawLine(0, heigth, width, heigth);
    QFont font;
    font.setFamily("URW Chancery L");
    font.setBold(true);
    font.setPixelSize(38);
    painter.setFont(font);
   // painter.drawText(0, 0, width, heigth, Qt::AlignLeft, zdyText+tr("元"));
     if("center"==zdyPos)
     {
         painter.drawText(0, 0, width, heigth, Qt::AlignHCenter | Qt::TextWordWrap, zdyText);
     }

    else if("Vertical"==zdyPos)
       {
            painter.drawText(0, 0, width, heigth, Qt::AlignLeft | Qt::TextWordWrap, zdyText);
            QMatrix  matrix;
            matrix.rotate(90);
            bmp2=bmp.transformed(matrix,Qt::FastTransformation);
            qDebug("creat Vertical bmp");
            if(bmp2.save("./BrandPrint1.bmp"));
              sprintf(bmpPath, "./BrandPrint1.bmp");
             return;
                /*  painter.drawLine(0, 0, width, 0);
                  painter.drawLine(0, heigth,width, heigth);
                   font.setPixelSize(28);
                    font.setBold(true);
                  painter.setFont(font);
                   QTransform transform;
                    painter.translate(128,128);
                    transform.rotate(+90);
                   painter.setWorldTransform(transform);
                    painter.drawText(50,-180, width,heigth , Qt::AlignCenter, zdyText);*/
       }
    else
         painter.drawText(0, 0, width, heigth, Qt::AlignLeft, zdyText);

       if(bmp.save("./BrandPrint.bmp"));
             sprintf(bmpPath, "./BrandPrint.bmp");

}

bool ScannerThread::getBmpData(char *fileName, unsigned char *bmpData)
{
    int i = 0;
    FILE *fp;

    fp = fopen(fileName, "r+");
    if (NULL == fp)
    {
        return false;
    }
    while (!feof(fp))
    {
        fread(bmpData+i, sizeof(char), 1, fp);
        i++;
    }
    fclose(fp);

    return true;
}
