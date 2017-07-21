#ifndef ICCARDLIB_H
#define ICCARDLIB_H

#include <QLibrary>
#include <QSettings>
#include <QTime>
#include <QDebug>
#include <QThread>

class ICcardLib
{
public:
    ICcardLib();
    ~ICcardLib();

    int IC_InitPort_ex(int comType, char* szPathName,long baud);
   // int  IC_InitPort(int Port,unsigned long baud);
    int  IC_Write (int ICDev, int Offset,int len, unsigned char* DataBuffer);
    int  IC_Read (int ICDev,int Offset,int len, unsigned char * DataBuffer);
    int  IC_ReadVer(int icdev,unsigned char *data_buff);				//读取硬件版本号
    int	 IC_Choose_24C64(int ICDev);
    int  IC_DevBeep(int icdev,int time);
    int asc2hex(unsigned char *strasc, unsigned char *strhex, int length);
    int  IC_Choose_24Cxx(int ICDev);
    int  IC_InitPort(int Port,unsigned long baud);

    /*加载读票机驱动*/
    bool IC_LoadLib(const QString &soFileName);


private:
    QString soFileName;
    QLibrary *IClib;
    static const int functionCount = 9;
    void *functionList[functionCount];  //读票机接口函数名称数组
    int apiTime;   //计算上一个执行接口的运行时间

};

#endif // ICCARDLIB_H

