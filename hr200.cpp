#include "hr200.h"

HR200::HR200(QString s) :
        hscannerLib(NULL)
{
    memset(functionList, 0, sizeof(functionList));
    QSettings setting(QString("./conf/config.ini"), QSettings::IniFormat);
    setting.beginGroup("Driver");
    QString soFileName("");
    soFileName = setting.value(QString("hr200")).toString();
    hscannerLoadLib(soFileName);
    setting.endGroup();
}

HR200::~HR200()
{
    if (hscannerLib)
    {
        delete hscannerLib;
        hscannerLib = NULL;
    }
}

//加载扫描枪驱动
bool HR200::hscannerLoadLib(const QString &soFileName)
{
    bool isLibOK = true;
    if (NULL != hscannerLib)
    {
        isLibOK = false;
    }

    if (isLibOK)
    {
        hscannerLib = new QLibrary(soFileName);
        bool ret = hscannerLib->load();
        if (!ret)
        {
            qDebug() << "Load HR200 Library Failed!";
        }

        const char nameList[functionCount][64] =
        {
            "BCRInit",  //打开扫描枪函数, 完成扫描识别的初始化
            "BCRClose",  //关闭设备
            "BCRGetLastErrorCode", //获取最近一次的错误码
            "BCRQueryCapability",  //扫描枪能力查询
            "BCRGetLastErrorStr",  //获取最近一次的错误描述
            "BCRAimOn",  //打开瞄准灯
            "BCRAimOff",  //关闭瞄准灯
            "BCREnable",  //启用扫描枪设备
            "BCRDisable",  //禁用扫描枪设备
            "BCREnableCode", //启用某种条码类型的支持
            "BCRDisableCode",  //关闭某种条码类型的支持
            "BCRSleep", //进入休眠模式
            "BCRWakeup",  //唤醒扫描枪
            "BCRResetComm",  //重置通讯
            "BCRStartScan",  //开始扫描识别
            "BCRStopScan",  //停止扫描识别
            "BCRScanIsComplete", //是否扫描完成
            "BCRIsReady", //检测扫描枪是否就绪
            "BCRSetScanMode", //设置扫描枪的模式
            "BCRGetScanMode",  //获取扫描枪的模式
            "BCRGetTriggerStatus", //获取扫描枪扳机状态
            "BCRGetScanDpi", //获取扫描图像的分辨率
            "BCRGetImageSize", //获得原始图像大小
            "BCRGetImage",  //获得原始图像
            "BCRGetDataLength", //获取扫描数据的长度
            "BCRGetTicketInfo", //读取扫描的内容
            "BCRBeep", //播放扫描枪提示音
            "BCREnableBeep", //允许扫描枪播放提示音
            "BCRDisableBeep", //禁止扫描枪播放提示音
            "BCRClearBuffer", //清空扫描枪缓存中的数据
            "BCRGetHWInformation", //取得扫描仪的硬件信息
            "BCRGetSWVersion",//获取软件版本号
            "BCRUserLEDOn",  //打开并设置反馈灯的颜色
            "BCRUserLEDOff"  //关闭用户反馈灯
        };
        for (int i = 0; i < functionCount; ++i)
        {
            functionList[i] = hscannerLib ->resolve(nameList[i]);// dlsym(libHandle,namelist[i]);
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
        qDebug() << "HR200 Load Library & Resolve Success!";
    }
    return isLibOK;
}

//打开扫描枪函数, 完成扫描识别的初始化
int HR200::BCRInit(SCANNERCALLBACK scannerCallBack(int, void*), const char* input_dir, const char* output_dir)
{
    int ret = -1;
    if (functionList[0] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret =  ((int(*)(SCANNERCALLBACK(int, void*), const char*, const char*))functionList[0])(scannerCallBack, input_dir, output_dir);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //     LOG4CXX_DEBUG(logger_, _T("error when calling HR200 Init."));
        ret = -1;
    }
    return ret ;
}

//关闭设备
void HR200::BCRClose()
{
    if (NULL != functionList[1])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(void)) functionList[1])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute Close"));
    }
}

//获取最近一次的错误码
int HR200::BCRGetLastErrorCode()
{
    int ret = -1;
    if (functionList[2] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[2])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //     LOG4CXX_DEBUG(logger_, _T("error when calling HR200 GetLastErrorCode."));
        ret = -1;
    }
    return ret ;
}

//扫描枪能力查询
int HR200::BCRQueryCapability()
{
    int ret = -1;
    if (functionList[3] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[3])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling HR200 QueryCapability."));
        ret = -1;
    }
    return ret ;
}

//获取最近一次的错误描述（信息长度在100字节以内，包含结束符）
void HR200::BCRGetLastErrorStr (char* const errStr , int const errStrBufLen)
{
    if (NULL != functionList[4])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(char*, int)) functionList[4])(errStr, errStrBufLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute GetLastErrorStr"));
    }
}

//打开瞄准灯
bool HR200::BCRAimOn()
{
    bool ok = false;
    if (NULL != functionList[5])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[5])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute AimOn");
        //return false;
    }
    return ok;
}

//关闭瞄准灯
bool HR200::BCRAimOff()
{
    bool ok = false;
    if (NULL != functionList[6])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[6])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute AimOff");
        //return false;
    }
    return ok;
}

//启用扫描枪设备
bool HR200::BCREnable()
{
    bool ok = false;
    if (NULL != functionList[7])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[7])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute Enable");
        //return false;
    }
    return ok;
}

//禁用扫描枪设备
bool HR200::BCRDisable()
{
    bool ok = false;
    if (NULL != functionList[8])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[8])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute Disable");
        //return false;
    }
    return ok;
}

//启用某种条码类型的支持
bool HR200::BCREnableCode(unsigned int const codeType)
{
    bool ok = false;
    if (NULL != functionList[9])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int)) functionList[9])(codeType);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute EnableCode");
        //return false;
    }
    return ok;
}

//关闭某种条码类型的支持
bool HR200::BCRDisableCode(unsigned int const codeType)
{
    bool ok = false;
    if (NULL != functionList[10])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int)) functionList[10])(codeType);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute DisableCode");
        //return false;
    }
    return ok;
}

//进入休眠模式，进入休眠后若有条码进入扫描区域或用户按下扫描开关，可以离开休眠模式，或者通过Wakeup接口离开休眠模式
bool HR200::BCRSleep(unsigned int const time)
{
    bool ok = false;
    if (NULL != functionList[11])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int)) functionList[11])(time);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute HR200Sleep");
        //return false;
    }
    return ok;
}

//唤醒扫描枪
bool HR200::BCRWakeup()
{
    bool ok = false;
    if (NULL != functionList[12])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[12])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute Wakeup");
        //return false;
    }
    return ok;
}

//重置通讯
bool HR200::BCRResetComm()
{
    bool ok = false;
    if (NULL != functionList[13])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[13])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute ResetComm");
        //return false;
    }
    return ok;
}

//开始扫描识别，使用此接口后需要通过Stop接口结束扫描。此接口应能自动完成LEDOn及AimOn的功能，因此调用时只需调用此接口即可
bool HR200::BCRStartScan()
{
    bool ok = false;
    if (NULL != functionList[14])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[14])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute StartScan");
        //return false;
    }
    return ok;
}

//停止扫描识别，可在扫描的过程中调用这个接口停止扫描
bool HR200::BCRStopScan()
{
    bool ok = false;
    if (NULL != functionList[15])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[15])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute StopScan");
        //return false;
    }
    return ok;
}

//是否扫描完成。应用轮询此接口，当接口返回完成时，应用调用数据读取接口读取数据
bool HR200::BCRScanIsComplete()
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
        //LOG4CXX_DEBUG(logger_, "fail to excute ScanIsComplete");
        //return false;
    }
    return ok;
}

//扫描枪状态检测，检测扫描枪是否就绪
bool HR200::BCRIsReady()
{
    bool ok = false;
    if (NULL != functionList[17])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[17])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute HR200IsReady");
        //return false;
    }
    return ok;
}

//设置扫描枪的模式
bool HR200::BCRSetScanMode(unsigned int const mode)
{
    bool ok = false;
    if (NULL != functionList[18])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int)) functionList[18])(mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute SetScanMode");
        //return false;
    }
    return ok;
}

//获取扫描枪的模式
bool HR200::BCRGetScanMode(unsigned int* const mode)
{
    bool ok = false;
    if (NULL != functionList[19])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int *)) functionList[19])(mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute GetScanMode");
        //return false;
    }
    return ok;
}

//获取扫描枪扳机状态
void HR200::BCRGetTriggerStatus(int* const status)
{
    if (NULL != functionList[20])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(int *)) functionList[20])(status);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute GetTriggerStatus"));
    }
}

//获取扫描图像的分辨率
void HR200::BCRGetScanDpi(int* const widthDpi,int* const heightDpi)
{
    if (NULL != functionList[21])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(int*, int *)) functionList[21])(widthDpi, heightDpi);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute GetScanDpi"));
    }
}

//获得原始图像大小，如果没有图像，则两项均为0
bool HR200::BCRGetImageSize(int* const width,int* const height, int *const buffsize)
{
    bool ok = false;
    if (NULL != functionList[22])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int *, int *,int *)) functionList[22])(width, height, buffsize);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute GetImageSize");
        //return false;
    }
    return ok;
}

//获得原始图像,其图像应该是经过可污点处理和方位校正后的单色图像
int HR200::BCRGetImage(char* const image,int const bufferLen)
{
    int ret = -1;
    if (functionList[23] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(char *, int)) functionList[23])(image, bufferLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling HR200 GetImage."));
        ret = -1;
    }
    return ret ;
}

//获取扫描数据的长度，供应用分配数据区大小
bool HR200::BCRGetDataLength(unsigned int* const length)
{
    bool ok = false;
    if (NULL != functionList[24])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int *)) functionList[24])(length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute GetDataLength");
        //return false;
    }
    return ok;
}

//读取扫描的内容
int HR200::BCRGetTicketInfo(unsigned char* const ticketInfo, unsigned int const bufferLen)
{
    int ret = -1;
    if (functionList[25] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(unsigned char*, unsigned int)) functionList[25])(ticketInfo, bufferLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling HR200 GetTicketInfo."));
        ret = -1;
    }
    return ret ;
}

//播放扫描枪提示音
bool HR200::BCRBeep(int const tone)
{
    bool ok = false;
    if (NULL != functionList[26])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int)) functionList[26])(tone);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute Beep");
        //return false;
    }
    return ok;
}

//允许扫描枪播放提示音，提示音只允许通过调用Beep产生
void HR200::BCREnableBeep()
{
    if (NULL != functionList[27])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(void)) functionList[27])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute EnableBeep"));
    }
}

//禁止扫描枪播放提示音，Disable后Beep将无效
void HR200::BCRDisableBeep()
{
    if (NULL != functionList[28])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(void)) functionList[28])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute DisableBeep"));
    }
}

//清空扫描枪缓存中的数据
void HR200::BCRClearBuffer()
{
    if (NULL != functionList[29])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(void)) functionList[29])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute ClearBuffer"));
    }
}

//取得扫描仪的硬件信息
bool HR200::BCRGetHWInformation(char* const hwInfo , int const length)
{
    bool ok = false;
    if (NULL != functionList[30])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, int)) functionList[30])(hwInfo, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute GetHWInfomation");
        //return false;
    }
    return ok;
}

//获取软件版本号的接口
void HR200::BCRGetSWVersion(char* const swVersion , unsigned int const length)
{
    if (NULL != functionList[31])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(char *, unsigned int)) functionList[31])(swVersion, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute GetSWVersion"));
    }
}

//打开并设置用户反馈灯的颜色
bool HR200::BCRUserLEDOn(unsigned int* const mode)
{
    bool ok = false;
    if (NULL != functionList[32])
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int* const)) functionList[32])(mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, "fail to excute UserLEDOn");
        //return false;
    }
    return ok;
}

//关闭用户反馈灯
void HR200::BCRUserLEDOff()
{
    if (NULL != functionList[33])
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(void)) functionList[33])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, ("fail to excute UserLEDOff"));
    }
}

