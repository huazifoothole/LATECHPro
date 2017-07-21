#include "iccardlib.h"

ICcardLib::ICcardLib():
    IClib(NULL)
{
    memset(functionList, 0, sizeof(functionList));
    QSettings setting(QString("./conf/config.ini"), QSettings::IniFormat);
    setting.beginGroup("Driver");
    QString soFileName("");
    soFileName = setting.value(QString("ic")).toString();
    qDebug()<<"IC soFileName=="<<soFileName;
    setting.endGroup();
    IC_LoadLib(soFileName);
}

ICcardLib::~ICcardLib()
{
    if (IClib)
    {
        delete IClib;
        IClib = NULL;
    }
}


bool  ICcardLib::IC_LoadLib(const QString &soFileName)
{
    bool isLibOK = true;
    if(NULL != IClib)
    {
        isLibOK = false;
    }

    if (isLibOK)
    {
         IClib = new QLibrary(soFileName);
        bool ret = IClib->load();
        if (!ret)
        {
            qDebug() << "Load IC Library Failed!";
        }

        const char nameList[functionCount][30] =
        {
          "IC_InitPort_ex",
          "IC_Write" ,
          "IC_Read",
          "IC_ReadVer",
           "IC_Choose_24C64",
           "IC_DevBeep",
           "asc2hex",
            "IC_Choose_24Cxx",
            "IC_InitPort"
        };
        for (int i = 0; i < functionCount; ++i)
        {
            functionList[i] = IClib->resolve(nameList[i]);// dlsym(libHandle,namelist[i]);
            if (functionList[i] == NULL)
            {
                isLibOK = false;
             //   qDebug() << nameList[i] << " is NULL";
            }
            else
            {
                //qDebug() << nameList[i] << " Resolve Success!";
            }
        }
    }
    if (isLibOK)
    {
        qDebug() << "IC Load Library & Resolve Success!";
    }
    else
    {
         qDebug() << "IC Load Library & Resolve failed!";
    }

    return isLibOK;
}

int ICcardLib::IC_InitPort_ex(int comType, char *szPathName, long baud)
{
    int ret = -1;
    if (NULL != functionList[0])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,char*,long)) functionList[0])(comType, szPathName, baud);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::IC_Write(int ICDev, int Offset,int len, unsigned char* DataBuffer)
{
    int ret = -1;
    if (NULL != functionList[1])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,int,int,unsigned char*)) functionList[1])(ICDev,  Offset,len, DataBuffer);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::IC_Read(int ICDev, int Offset,int len, unsigned char* DataBuffer)
{
    int ret = -1;
    if (NULL != functionList[2])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,int,int,unsigned char*)) functionList[2])(ICDev,  Offset,len, DataBuffer);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::IC_ReadVer(int ICDev, unsigned char* DataBuffer)
{
    int ret = -1;
    if (NULL != functionList[3])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,unsigned char*)) functionList[3])(ICDev, DataBuffer);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int	ICcardLib::IC_Choose_24C64(int ICDev)
{
    int ret = -1;
    if (NULL != functionList[4])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int)) functionList[4])(ICDev);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int	ICcardLib::IC_DevBeep(int ICDev,int time)
{
    int ret = -1;
    if (NULL != functionList[5])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,int)) functionList[5])(ICDev,time);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::asc2hex(unsigned char *strasc, unsigned char *strhex, int length)
{
    int ret = -1;
    if (NULL != functionList[6])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(unsigned char*,unsigned char*,int)) functionList[6])(strasc,strhex,length);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::IC_Choose_24Cxx(int ICDev)
{
    int ret = -1;
    if (NULL != functionList[7])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int)) functionList[7])(ICDev);
        apiTime = temTimer.elapsed();
    }
    return ret;
}

int ICcardLib::IC_InitPort(int Port, unsigned long baud)
{
    int ret = -1;
    if (NULL != functionList[8])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(int,unsigned long)) functionList[8])(Port,baud);
        apiTime = temTimer.elapsed();
    }
    return ret;
}



