#ifndef HR200_H
#define HR200_H

#include <QLibrary>
#include <QSettings>
#include <QTime>
#include <QDebug>


/**
 * @brief 扫描枪回调函数
 * @brief 设备初始化后需要判断传入的函数指针是否为空，如果为空，则不需要回调方式支持，非空时需要回调方式
 * @brief 当有票进入，并成功得到该票信息后，调用此回调函数，之后应用程序会调GetTicketInfo函数获取该票内容。
 * @param eventId    事件ID，当有票时需要发送该事件
 * @param data
 * @return 无
 **/
typedef void(*SCANNERCALLBACK)(int eventId, void *data);



class HR200
{
public:
    HR200(QString);
    ~HR200();

    /*打开扫描枪函数, 完成扫描识别的初始化*/
    int BCRInit(SCANNERCALLBACK scannerCallBack(int, void*), const char* input_dir, const char* output_dir);

    /*关闭设备*/
    void BCRClose();

    /*获取最近一次的错误码*/
    int BCRGetLastErrorCode();

    /*扫描枪能力查询*/
    int BCRQueryCapability();

    /*获取最近一次的错误描述（信息长度在100字节以内，包含结束符）*/
    void BCRGetLastErrorStr (char* const errStr , int const errStrBufLen);

    /*打开瞄准灯*/
    bool BCRAimOn();

    /*关闭瞄准灯*/
    bool BCRAimOff();

    /*启用扫描枪设备*/
    bool BCREnable();

    /*禁用扫描枪设备*/
    bool BCRDisable();

    /*启用某种条码类型的支持*/
    bool BCREnableCode(unsigned int const codeType);

    /*关闭某种条码类型的支持*/
    bool BCRDisableCode(unsigned int const codeType);

    /*进入休眠模式，进入休眠后若有条码进入扫描区域或用户按下扫描开关，可以离开休眠模式，或者通过Wakeup接口离开休眠模式*/
    bool BCRSleep(unsigned int const time);

    /*唤醒扫描枪*/
    bool BCRWakeup();

    /*重置通讯*/
    bool BCRResetComm();

    /*开始扫描识别，使用此接口后需要通过Stop接口结束扫描。此接口应能自动完成LEDOn及AimOn的功能，因此调用时只需调用此接口即可*/
    bool BCRStartScan();

    /*停止扫描识别，可在扫描的过程中调用这个接口停止扫描*/
    bool BCRStopScan();

    /*是否扫描完成。应用轮询此接口，当接口返回完成时，应用调用数据读取接口读取数据*/
    bool BCRScanIsComplete();

    /*扫描枪状态检测，检测扫描枪是否就绪*/
    bool BCRIsReady();

    /*设置扫描枪的模式*/
    bool BCRSetScanMode(unsigned int const mode);

    /*获取扫描枪的模式*/
    bool BCRGetScanMode(unsigned int* const mode);

    /*获取扫描枪扳机状态*/
    void BCRGetTriggerStatus(int* const status);

    /*获取扫描图像的分辨率*/
    void BCRGetScanDpi(int* const widthDpi,int* const heightDpi);

    /*获得原始图像大小，如果没有图像，则两项均为0*/
    bool BCRGetImageSize(int* const width,int* const height, int *const buffsize);

    /*获得原始图像,其图像应该是经过可污点处理和方位校正后的单色图像*/
    int BCRGetImage(char* const image,int const bufferLen);

    /*获取扫描数据的长度，供应用分配数据区大小*/
    bool BCRGetDataLength(unsigned int* const length);

    /*读取扫描的内容*/
    int BCRGetTicketInfo(unsigned char* const ticketInfo, unsigned int const bufferLen);

    /*播放扫描枪提示音*/
    bool BCRBeep(int const tone);

    /*允许扫描枪播放提示音，提示音只允许通过调用Beep产生*/
    void BCREnableBeep();

    /*禁止扫描枪播放提示音，Disable后Beep将无效*/
    void BCRDisableBeep();

    /*清空扫描枪缓存中的数据*/
    void BCRClearBuffer();

    /*取得扫描仪的硬件信息（信息长度在4096字节以内，包含结束符）*/
    bool BCRGetHWInformation(char* const hwInfo , int const length);

    /*获取软件版本号的接口*/
    void BCRGetSWVersion(char* const swVersion , unsigned int const length);

    /*打开并设置用户反馈灯的颜色*/
    bool BCRUserLEDOn(unsigned int* const mode);

    /*关闭用户反馈灯*/
    void BCRUserLEDOff();


    /*加载扫描枪驱动*/
    bool hscannerLoadLib(const QString &soFileName);

private:
    QString soFileName;
    QLibrary *hscannerLib;
    static const int functionCount = 34;
    void *functionList[functionCount];  //扫描机接口函数名称数组
    int apiTime;   //计算上一个执行接口的运行时间

};

#endif // HSCANNER_H
