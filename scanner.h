#ifndef SCANNER_H
#define SCANNER_H

#include <QLibrary>
#include <QSettings>
#include <QTime>
#include <QDebug>
#include <QThread>

/**
 * @brief 读票机回调函数
 * @param eventId    事件ID，当有票时需要发送该事件
 * @param data   数据指针，用来区分是投注单还是彩票，1为投注单，2为彩票
 * @return 无
 **/
typedef void(*SCANNERCALLBACK)(int eventId, void *data);
extern "C"
{
extern SCANNERCALLBACK scannerCallBack(int eventID, void *data);
}


class Scanner
{
public:
    Scanner();
    ~Scanner();

    /*读票机初始化函数*/
    int SInit(SCANNERCALLBACK scannerCallBack(int,void*), const char* input_dir, const char* output_dir);

    /*读票机能力查询，是否标记打印功能*/
    int SQueryCapability();

    /*获取扫描图像的分辨率*/
    bool SGetScanDpi(int* const widthDpi,int* const heightDpi);

    /*获取标记打印图像的分辨率*/
    bool SGetBrandDpi(int* const widthDpi,int* const heightDpi);

    /*取得打印机的硬件信息*/
    bool SGetHWInformation(char* const hwInfo,unsigned int const length);

    /*获取软件版本号的接口*/
    bool SGetSWVersion(char* const swVersion , unsigned int const length);

    /*获取最近一次的错误码*/
    int SGetLastErrorCode();

    /*获取最近一次的错误描述*/
    void SGetLastErrorStr(char* const errStr, unsigned int const length);

    /*开始扫描识别*/
    bool SStart();

    /*结束扫描识别*/
    bool SStop();

    /*是否扫描完成*/
    bool ScanIsComplete();

    /*读票机状态检测*/
    bool ScannerIsReady();

    /*获得原始图像大小，如果没有图像，则两项均为0*/
    bool SGetOriginImageSize(int* const width,int* const height, int* const bufsize);

    /*获得原始图像,其图像应该是经过污点处理和方位校正后的单色图像*/
    int SGetOriginImage(char* const image,int const bufferLen);

    /*读取扫描的内容*/
    int SGetTicketInfo(unsigned char* const ticketInfo,int const bufferLen);

    /*标记打印*/
    int SPrintBrandImage(const char* image, int index, int xpos, int ypos);

    /*退纸*/
    bool SRollBack();

    /*识别指定区域的内容，可以是字符，或条码*/
    bool SRecognizeItem(int posX, int posY, int width, int height, const char* const image, char* result);

    /*读票机精度校准，调用此接口后，读票机能够自动卷入校正纸，进行精度校准，并卷出彩票纸*/
    bool SAdjustSensibility(int *currentSens, int *adjustSens);


    /*加载读票机驱动*/
    bool scannerLoadLib(const QString &soFileName);

private:
    QString soFileName;
    QLibrary *scannerLib;
    static const int functionCount = 19;
    void *functionList[functionCount];  //读票机接口函数名称数组
    int apiTime;   //计算上一个执行接口的运行时间

};

#endif // SCANNER_H
