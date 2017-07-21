#include "scanner.h"

Scanner::Scanner() :
        scannerLib(NULL)
{
    memset(functionList, 0, sizeof(functionList));
    QSettings setting(QString("./conf/config.ini"), QSettings::IniFormat);
    setting.beginGroup("Driver");
    QString soFileName("");
    soFileName = setting.value(QString("scanner")).toString();
    setting.endGroup();
    scannerLoadLib(soFileName);
}

Scanner::~Scanner()
{
    if (scannerLib)
    {
        delete scannerLib;
        scannerLib = NULL;
    }
}

//加载读票机驱动
bool Scanner::scannerLoadLib(const QString &soFileName)
{
    bool isLibOK = true;
    if(NULL != scannerLib)
    {
        isLibOK = false;
    }

    if (isLibOK)
    {
        scannerLib = new QLibrary(soFileName);
        bool ret = scannerLib->load();
        if (!ret)
        {
            qDebug() << "Load Scanner Library Failed!";
        }

        const char nameList[functionCount][64] =
        {
            "SInit",  //打开读票机函数, 完成扫描识别的初始化
            "SQueryCapability", //读票机能力查询，是否标记打印功能
            "SGetScanDpi",  //获取扫描图像的分辨率
            "SGetBrandDpi",  //获取标记打印图像的分辨率
            "SGetHWInformation",  //取得打印机的硬件信息
            "SGetSWVersion",  //获取软件版本号的接口
            "SGetLastErrorCode",  //获取最近一次的错误码
            "SGetLastErrorStr",  //获取最近一次的错误描述
            "SStart", //开始扫描识别
            "SStop",  //结束扫描识别
            "ScanIsComplete",  //是否扫描完成
            "ScannerIsReady",  //读票机状态检测
            "SGetOriginImageSize", //获得原始图像大小，如果没有图像，则两项均为0
            "SGetOriginImage", //获得原始图像,其图像应该是经过污点处理和方位校正后的单色图像
            "SGetTicketInfo",  //读取扫描的内容
            "SPrintBrandImage", //标记打印
            "SRollBack",  //退纸
            "SRecognizeItem",  //识别指定区域的内容，可以是字符，或条码
            "SAdjustSensibility"  //读票机精度校准，调用此接口后，读票机能够自动卷入校正纸，进行精度校准，并卷出彩票纸
        };
        for (int i = 0; i < functionCount; ++i)
        {
            functionList[i] = scannerLib->resolve(nameList[i]);// dlsym(libHandle,namelist[i]);
            if (functionList[i] == NULL)
            {
                isLibOK = false;
                //qDebug() << nameList[i] << " is NULL";
            }
            else
            {
                //qDebug() << nameList[i] << " Resolve Success!";
            }
        }
    }
    if (isLibOK)
    {
        qDebug() << "Scanner Load Library & Resolve Success!";
    }
    return isLibOK;
}


//读票机初始化
int Scanner::SInit(SCANNERCALLBACK scannerCallBack(int,void*), const char* input_dir, const char* output_dir)
{

    int ret = -1;
    if (functionList[0] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(SCANNERCALLBACK(int, void*), const char*, const char*))functionList[0])(scannerCallBack, input_dir, output_dir);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute ScannerInit"));
        //return -1;
    }
    return ret;

}

//读票机能力查询函数
int Scanner::SQueryCapability()
{
    int ret = -1;
    if (functionList[1] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[1])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        /*write error info to log*/
        //LOG4CXX_DEBUG(logger_, ("fail to excute ScannerQueryCapability"));
        //return -1;
    }
    return ret;
}

//获取扫描图像的分辨率
bool Scanner::SGetScanDpi(int* const widthDpi, int* const heightDpi)
{
    bool ok = false;
    if (NULL != functionList[2])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int *, int *)) functionList[2])(widthDpi, heightDpi);
        apiTime = temTimer.elapsed();
        qDebug() << "after call GetScanDpi...";
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute SGetScanDpi"));
        //ok = false;
    }
    return ok;
}

//获取标记打印图像的分辨率
bool Scanner::SGetBrandDpi(int* const widthDpi,int* const heightDpi)
{
    bool ok = false;
    if (NULL != functionList[3])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int *, int *)) functionList[3])(widthDpi, heightDpi);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute SGetBrandDpi"));
        //ok = false;
    }
    return ok;
}

//取得打印机的硬件信息
bool Scanner::SGetHWInformation(char* const hwInfo,unsigned int const length)
{
    bool ok = false;
    if (NULL != functionList[4])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, unsigned int)) functionList[4])(hwInfo, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute SGetHWInformation"));
        //ok = false;
    }
    return ok;
}

//获取软件版本号的接口
bool Scanner::SGetSWVersion(char* const swVersion , unsigned int const length)
{
    bool ok = false;
    if (NULL != functionList[5])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, unsigned int)) functionList[5])(swVersion, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute SGetSWVersion"));
        //ok = false;
    }
    return ok;
}

//获取最近一次的错误代码
int Scanner::SGetLastErrorCode()
{
    int ret = -1;
    if (NULL != functionList[6])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[6])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SGetLastErrorCode");
        //return -1;
    }
    return ret;
}

//获取最近一次的错误描述信息
void Scanner::SGetLastErrorStr(char* const errStr, unsigned int const length)
{
    if (NULL != functionList[7])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(char*, unsigned int)) functionList[7])(errStr, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute SGetLastErrorStr"));
    }
}

//启动读票机函数,开始扫描识别
bool Scanner::SStart()
{
#ifdef TEST_SCANNER
    return true;
#endif
    bool ok = false;
    if (NULL != functionList[8])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[8])();
        apiTime = temTimer.elapsed();
        //isOpened = true;
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SStart");
        //return false;
    }
    return ok;
}

bool Scanner::SStop()
{
#ifdef TEST_SCANNER
    return true;
#endif
    bool ok = false;
    if (NULL != functionList[9])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[9])();
        apiTime = temTimer.elapsed();
        //isOpened = false;
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SStop");
        //return false;
    }
    return ok;
}

//判断扫描是否完成函数
bool Scanner::ScanIsComplete()
{
#ifdef TEST_SCANNER
    return true;
#endif
    bool ok = false;
    if (NULL != functionList[10])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[10])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute ScanIsComplete");
        //return false;
    }
    return ok;
}

//检测读票机状态函数
bool Scanner::ScannerIsReady()
{
#ifdef TEST_SCANNER
    return true;
#else
    bool ok = false;
    if (NULL != functionList[11])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[11])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute ScannerIsReady");
        //return false;
    }
    return ok;
#endif
}

//获得原始图像大小，如果没有图像，则两项均为0
bool Scanner::SGetOriginImageSize(int* const width,int* const height, int* const bufsize)
{
    bool ok = false;
    if (NULL != functionList[12])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int* const, int* const, int* const)) functionList[12])(width, height, bufsize);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SGetOriginImageSize");
        //return false;
    }
    return ok;
}

//获得原始图像,其图像应该是经过污点处理和方位校正后的单色图像
int Scanner::SGetOriginImage(char* const image,int const bufferLen)
{
    int ret = -1;
    if (NULL != functionList[13])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(char *, int)) functionList[13])(image, bufferLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SGetOriginImage");
        //return -1;
    }
    return ret;
}

//读取扫描的内容
int Scanner::SGetTicketInfo(unsigned char* const ticketInfo,int const bufferLen)
{
    int ret = -1;
    if (NULL != functionList[14])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(unsigned char *, int)) functionList[14])(ticketInfo, bufferLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SGetTicketInfo");
        //return -1;
    }
    return ret;
}

//标记打印
int Scanner::SPrintBrandImage(const char* image, int index, int xpos, int ypos)
{
    int ret = -1;
    if (NULL != functionList[15])
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(const char *, int, int, int)) functionList[15])(image, index, xpos, ypos);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SPrintBrandImage");
        //return -1;
    }
    return ret;
}

//退纸
bool Scanner::SRollBack()
{
    bool ok = false;
    if (NULL != functionList[16])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[16])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SRollBack");
        //return false;
    }
    return ok;
}

//识别指定区域的内容，可以是字符，或条码
bool Scanner::SRecognizeItem(int posX, int posY, int width, int height, const char* const image, char* result)
{
    bool ok = false;
    if (NULL != functionList[17])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int, int, int, int, const char*, char*)) functionList[17])(posX, posY, width, height, image, result);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SRecognizeItem");
        //return false;
    }
    return ok;
}

//读票机精度校准，调用此接口后，读票机能够自动卷入校正纸，进行精度校准，并卷出彩票纸
bool Scanner::SAdjustSensibility(int* currentSens, int* adjustSens)
{
    bool ok = false;
    if (NULL != functionList[18])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int*, int*)) functionList[18])(currentSens, adjustSens);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SAdjustSensibility");
        //return false;
    }
    return ok;
}
