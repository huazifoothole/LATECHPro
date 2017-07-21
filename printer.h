#ifndef PRINTER_H
#define PRINTER_H

#include <QByteArray>
#include <QString>
#include <QTextCodec>
#include <QLibrary>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QTime>
#include <QDebug>
#include <QDialog>

#define U2G(str) (QTextCodec::codecForName("GBK")->fromUnicode(str))
#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )

class Printer : public QDialog
{
    Q_OBJECT

public:
    Printer();
    ~Printer();

    /*初始化打印机*/
    int PInit(const char* input_dir, const char* output_dir);

    /*设置打印模式*/
    bool PSetCutterMode(int const mode);

    /*获取当前打印模式*/
    bool PGetCutterMode(int* const mode);

    /*设置打印机的字体,大小和对齐方式*/
    bool PSetFont(unsigned char const index, unsigned char const size, unsigned char const alignment);

    /*设置打印模式为页模式，并设置页模式相应信息*/
    bool PSetPageMode(int width, int height, int leftTop_x, int leftTop_y);

    /*设置打印模式为行模式*/
    bool PSetLineMode();

    /**结束页模式输入并打印该页内容*/
    int PPrintPage();

    /*设置字体加粗*/
    bool PSetFontEmpha(unsigned char const n);

    /*设置行间距*/
    bool PSetLineSpace(unsigned char const size);

    /*设置字符间距*/
    bool PSetCharSpace(unsigned char const size);

    /*设置左边界*/
    bool PSetLeftMargin(int const size);

    /*设置有效打印区域宽度*/
    bool PSetAreaWidth(int const width);

    /*打印机能力查询*/
    int PQueryCapability();

    /*检测打印机状态*/
    bool PPrinterIsReady();

    /*获取打印机的dpi值*/
    bool PGetDpi(int* const widthDpi,int* const heightDpi);

    /*获取打印机的硬件信息*/
    bool PGetHWInformation(char* const hwInfo , unsigned int const length);

    /*获取软件版本号*/
    bool PGetSWVersion(char* const swVersion , unsigned int const length);

    /*获取最近一次的错误码*/
    int PGetLastErrorCode();

    /*获取最近一次的错误描述*/
    void PGetLastErrorStr(char* const errStr, unsigned int const length);

    /*打印字符串，字符集为GB2312*/
    int PPrintString(const char*  const str);

    /*空行*/
    void PFeedLine(unsigned char const lineCount);

    /*切纸*/
    bool PCutPaper();

    /*打印二维条码，标准为PDF417*/
    int PrintPDF417(int module_width, int module_height, int data_rows, int data_columns, int err_correct_level,const char* databuf, int length, int mode);

    /*用户自定义字符*/
    bool PSetUserChar(unsigned char c1,unsigned char c2,unsigned char m, const unsigned char* const data,int length);

    /*取消用户自定义字符*/
    bool PUnsetUserChar(unsigned char c1, unsigned char c2);

    /*打印用户自定义字符*/
    bool PPrintUserChar(unsigned char c1, unsigned char c2);

    /*打印内存中用户定义的图形点阵*/
    bool PPrintUserBitmap(int xPos,int yPos, const char* const bitmap);

    /*打印用户磁盘上的图像文件*/
    int PPrintDiskImage(int xPos,int yPos,const char* const imagePath);

    /*打印黑标*/
    bool PPrintBlackMark(const char* const barCode ,unsigned int const length);

    /*获取打印机切刀与打印头间距*/
    bool PGetTopMargin(int* const topMargin);

    /*确定之前发送的打印指令是否完成*/
    int PPrintIsComplete(int const time_out);

    /*获取打印机走纸长度*/
    long PGetPrintLength();

    /*打印机下载自定义LOGO图像到打印机*/
    bool PLoadLogoImage(int imgCount, char *imgeList[]);

    /*打印机下载自定义LOGO图像到打印机*/
    bool PLoadDiskLogo(int imgCount, char *imgFileList[]);

    /*打印LOGO图标*/
    bool PPrintLogo(unsigned int const xpos, unsigned int const ypos, int const index);

    /*设置打印的旋转角度*/
    bool PSetAngle(unsigned int const angle);

    /*执行ESC/POS指令*/
    bool PExec_ESC_POS(char* const command, int const strLen);
//37

    //非体彩接口
    /*打印大黑标*/
//    bool PrintBMark(char *barCode, int length);

    /*打印小黑标*/
//    bool PrintSMark(char *barCode, int length);

    /*切刀模式*/
//    void CutPaperByType(int type);  //type: 0--全切 1--半切

    /*打印一维条码*/
//    bool Print1DBar(unsigned char width, unsigned char height, const char* str,char codeType,char setType);

    /*热敏头及切刀寿命查询*/
//    bool GetMaintenanceCounter(int counter, unsigned long* count);


    /*加载打印机驱动*/
    bool printerLoadLib(const QString &soFileName);

    /*生成黑标*/
    char* CreateBlackMark(char* buf, int len);

    /*打印黑标样票*/
    bool PrintMarkTicket(int markType); //markType: 0--大黑标  1--小黑标

    /*打印条码样票*/
    bool PrintCodeTicket(int codeType);

    /*打印其它样票*/
    bool PrintOtherTicket();

    bool PrintSampleTicket(int flag);

    bool PrintHalfCut();
    bool PrintFullCut();


private :
    QString soFileName;
    QLibrary *printerLib;
    static const int functionCount = 37;
    void* functionList[functionCount]; ////打印机接口函数名称数组
    int apiTime;//计算上一个执行接口的运行时间

signals:
    void showText(char *);
    void showText_(char *);
};

#endif // HWILATECHPRINTER_H
