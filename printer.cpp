#include "printer.h"

Printer::Printer() :
        printerLib(NULL)
{
    memset(functionList, 0, sizeof(functionList));
    QSettings setting(QString("./conf/config.ini"), QSettings::IniFormat);
    setting.beginGroup("Driver");
    QString soFileName("");
    soFileName = setting.value(QString("printer")).toString();
    setting.endGroup();
    printerLoadLib(soFileName);
}

Printer::~Printer()
{
    if (printerLib)
    {
        delete printerLib;
        printerLib = NULL;
    }
}

//加载打印机驱动
bool Printer::printerLoadLib(const QString &soFileName)
{
    bool isLibOK = true;
    if(NULL != printerLib)
    {
          isLibOK = false;
    }

    if (isLibOK)
    {
        printerLib = new QLibrary(soFileName);
        bool ret = printerLib->load();
        if (!ret)
        {
            qDebug() << "Load Printer Library Failed!";
        }

        const char nameList[functionCount][64] =
        {
            "PInit",            //打印机初始化
            "PSetCutterMode",   //设置打印模式
            "PGetCutterMode",   //获取当前打印模式
            "PSetFont",   //设置字体
            "PSetPageMode", //页模式信息设置
            "PSetLineMode", //行模式设置
            "PPrintPage", //结束页模式并打印该页
            "PSetFontEmpha",  //设置黑体
            "PSetLineSpace",  //设置行间距
            "PSetCharSpace",   //设置字符间距
            "PSetLeftMargin",  //设置左边界
            "PSetAreaWidth",  //设置有效打印区域宽度
            "PQueryCapability",   //打印机能力查询
            "PPrinterIsReady",  //检测打印机状态
            "PGetDpi",  //获取打印机dpi值
            "PGetHWInformation",  //取得打印机的硬件信息
            "PGetSWVersion",  //获取软件版本号
            "PGetLastErrorCode",  //获取最近一次的错误码
            "PGetLastErrorStr",  //获取最近一次的错误描述
            "PPrintString",  //打印字符串
            "PFeedLine",  //空行
            "PCutPaper", //切纸
            "PrintPDF417", //打印标准二维条码
            "PSetUserChar",  //用户自定义字符
            "PUnsetUserChar", //取消用户自定义字符
            "PPrintUserChar", //打印用户自定义字符
            "PPrintUserBitmap", //打印内存中用户定义的图形点阵
            "PPrintDiskImage",  //打印用户磁盘上的图像文件
            "PPrintBlackMark",  //打印黑标
            "PGetTopMargin",  //获取打印机切刀与打印头间距
            "PPrintIsComplete",  //确定之前发送的打印指令是否完成
            "PGetPrintLength" ,  //获取打印机走纸长度
            "PLoadLogoImage" ,  //获取自定义LOGO到打印机
            "PLoadDiskLogo" ,  //下载自定义LOGO到打印机
            "PPrintLogo" ,  //打印LOGO图标
            "PSetAngle" ,  //设置打印的旋转角度
            "PExec_ESC_POS",   //执行ESC/POS命令
        #if 0
            "PrintBMark", //打印大黑标
            "PrintSMark", //打印小黑标
            "CutPaperByType", //切刀模式 0--全切 1--半切
            "Print1DBar", //打印一维条码
            "GetMaintenanceCounter"  //热敏头及切刀寿命查询
        #endif
        };
        for (int i = 0; i < functionCount; ++i)
        {
            functionList[i] = printerLib->resolve(nameList[i]);// dlsym(libHandle,namelist[i]);
            if (functionList[i] == NULL)
            {
                isLibOK = false;
                qDebug() << nameList[i] << "fun is NULL";
            }
            else
            {
                //qDebug() << nameList[i] << " Resolve Success!";
            }
        }
    }
    if (isLibOK)
    {
        qDebug() << "Printer Load Library & Resolve Success!";
    }
    return isLibOK;
}

//打印机初始化
int Printer::PInit(const char* input_dir, const char* output_dir)
{
        int ret = -1;
        if (functionList[0] != NULL)
        {
            QTime temTimer;
            temTimer.start();
            ret = ((int(*)(const char*, const char*)) functionList[0])(input_dir, output_dir);
            apiTime = temTimer.elapsed();
        }
        else
        {
                //LOG4CXX_DEBUG(logger_, _T("error when calling PrinterInit."));
                ret = -1;
        }
        return ret ;
}

//设置打印模式
bool Printer::PSetCutterMode(int const mode)
{
    bool ok = false;
    if (functionList[1] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int)) functionList[1])(mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrinterSetMode."));
        //ok = false;
    }

    return ok;
}

//获取打印模式
bool Printer::PGetCutterMode(int* const mode)
{
    int ok = false;
    if (functionList[2] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int* const)) functionList[2])(mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PGetCutterMode."));
        //ok = false;
    }

    return ok;
}

//设置字体
bool Printer::PSetFont(unsigned char const index, unsigned char const size,
    unsigned char const alignment)
{
    bool ok = false;
    if (functionList[3] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char, unsigned char, unsigned char)) functionList[3])(
            index, size, alignment);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetFont."));
        //ok = false;
    }
    return ok;
}

//页模式信息设置接口
bool Printer::PSetPageMode(int width, int height, int leftTop_x, int leftTop_y)
{
    bool ok = false;
    if (functionList[4] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int, int, int, int)) functionList[4])(width, height, leftTop_x, leftTop_y);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetFont."));
        //ok = false;
    }
    return ok;
}

//行模式设置
bool Printer::PSetLineMode()
{
    bool ok = false;
    if (functionList[5] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)()) functionList[5])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetFont."));
        //ok = false;
    }
    return ok;
}

//结束页模式输入并打印该页内容
int Printer::PPrintPage()
{
    int ret = -1;
    if (functionList[6] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[6])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling QueryCapability."));
        ret = -1;
    }
    return ret;
}

//设置字体加粗
bool Printer::PSetFontEmpha(unsigned char const n)
{
    bool ok = false;
    if (functionList[7] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char)) functionList[7])(n);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetFontEmpha."));
        //ok = false;
    }
    return ok;
}

//设置行间距
bool Printer::PSetLineSpace(unsigned char const size)
{
    bool ok = false;
    if (functionList[8] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char)) functionList[8])(size);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetLineSpace."));
        //ok = false;
    }
    return ok;
}

//设置字符间距
bool Printer::PSetCharSpace(unsigned char const size)
{
    bool ok = false;
    if (functionList[9] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char)) functionList[9])(size);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetCharSpace."));
        //ok = false;
    }
    return ok;
}

//设置左边界
bool Printer::PSetLeftMargin(int const size)
{
    bool ok = false;
    if (functionList[10] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int)) functionList[10])(size);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetLeftMargin."));
        //ok = false;
    }
    return ok;

}

//设置有效打印区域宽度
bool Printer::PSetAreaWidth(int const width)
{
    bool ok = false;
    if (functionList[11] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int)) functionList[11])(width);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetAreaWidth."));
        //return false;
    }
    return ok;
}

//打印机能力查询(暂时需要支持查询是否支持字符打印模式及图像打印模式)
int Printer::PQueryCapability()
{
    int ret = -1;
    if (functionList[12] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ret = ((int(*)(void)) functionList[12])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling QueryCapability."));
        ret = -1;
    }
    return ret;
}

//检测打印机状态
bool Printer::PPrinterIsReady()
{
    bool ok = false;
    if (functionList[13] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[13])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrinterIsReady."));
        //return false;
    }
    return ok;
}

//获取打印机的dpi值
bool Printer::PGetDpi(int* const widthDpi,int* const heightDpi)
{
    bool ok = false;
    if (functionList[14] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int *, int *)) functionList[14])(widthDpi, heightDpi);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetDpi."));
        //return false;
    }
    return ok;
}

//取得打印机的硬件信息
bool Printer::PGetHWInformation(char* const hwInfo , unsigned int const length)
{
    bool ok = false;
    if (functionList[15] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, int)) functionList[15])(hwInfo, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetHWInformation."));
        // return false;
    }
    return ok;
}

//获取软件版本号
bool Printer::PGetSWVersion(char* const swVersion , unsigned int const length)
{
    bool ok = false;
    if (functionList[16] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, unsigned int)) functionList[16])(swVersion, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetSWVersion."));
        // return false;
    }
    return ok;
}

//获取最近一次的错误码
int Printer::PGetLastErrorCode()
{
    int code = -1;
    if (functionList[17] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        code = ((int(*)(void)) functionList[17])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetLastErrorCode."));
        code = -1;
    }
    return code;
}

//获取最近一次的错误描述
void Printer::PGetLastErrorStr(char* const errStr, unsigned int const length)
{
    if (functionList[18] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(char *, unsigned int)) functionList[18])(errStr, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetLastErrorStr."));
    }
}

//打印字符串，字符集为GB2312
int Printer::PPrintString(const char* const str)
{
    int code = -1;
    if (functionList[19] != NULL)
    {
        qDebug() << str;
        QTime temTimer;
        temTimer.start();
        code = ((int(*)(const char *)) functionList[19])(U2G(str).data());
        //code = ((int(*)(const char *)) functionList[19])(str);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetLastErrorCode."));
        code = -1;
    }
    return code;
}

//空行
void Printer::PFeedLine(unsigned char const lineCount)
{
    if (functionList[20] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(unsigned char)) functionList[20])(lineCount);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling FeedLine."));
    }
}

//切纸，字符打印模式与图像打印模式公用
bool Printer::PCutPaper()
{
    bool ok = false;
    if (functionList[21] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(void)) functionList[21])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling CutPaper."));
        //return false;
    }
    return ok;
}

//打印二维条码，标准为PDF417
int Printer::PrintPDF417(int module_width, int module_height, int data_rows, int data_columns, int err_correct_level,const char* databuf, int length, int mode)
{
    int code = -1;
    if (functionList[22] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        code = ((int(*)(int, int, int, int, int, const char*, int, int))functionList[22])(module_width, module_height, data_rows, data_columns, err_correct_level, databuf, length, mode);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Print2DBar."));
        //return false;
    }
    return code;
}

//用户自定义字符
bool Printer::PSetUserChar(unsigned char c1,unsigned char c2,unsigned char m, const unsigned char* const data,int length)
{
    bool ok = false;
    if (functionList[23] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char, unsigned char, unsigned char, const unsigned char*, int)) functionList[23])(c1, c2, m, data, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling SetUserChar."));
        //return false;
    }
    return ok;
}

//取消用户自定义字符
bool Printer::PUnsetUserChar(unsigned char c1, unsigned char c2)
{
    bool ok = false;
    if (functionList[24] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char, unsigned char)) functionList[24])(c1, c2);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling UnsetUserChar."));
        //return false;
    }
    return ok;
}

//打印用户自定义字符
bool Printer::PPrintUserChar(unsigned char c1, unsigned char c2)
{
    bool ok = false;
    if (functionList[25] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char, unsigned char)) functionList[25])(c1, c2);
        apiTime = temTimer.elapsed();
    }
    else
    {
        // LOG4CXX_DEBUG(logger_, _T("error when calling PPrintUserChar."));
    // return false;
    }
    return ok;
}

//打印内存中用户定义的图形点阵
bool Printer::PPrintUserBitmap(int xPos,int yPos, const char* const bitmap)
{
    bool ok = false;
    if (functionList[26] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int, int, const char* const)) functionList[26])(xPos, yPos, bitmap);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrintUserBitmap."));
    // return false;
    }
    return ok;
}

//打印用户磁盘上的图像文件
int Printer::PPrintDiskImage(int xPos,int yPos, const char* const imagePath)
{
    int code = -1;
    if (functionList[27] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        code = ((int(*)(int, int, const char*))functionList[27])(xPos, yPos, imagePath);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrintDiskImage."));
        //return false;
    }
    return code;
}

//打印黑标
bool Printer::PPrintBlackMark(const char* const barCode ,unsigned int const length)
{
    bool ok = false;
    if (functionList[28] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(const char*, unsigned int)) functionList[28])(barCode, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrintBlackMark."));
        // return false;
    }
    return ok;
}

//获取打印机切刀与打印头间距
bool Printer::PGetTopMargin(int* const topMargin)
{
    bool ok = false;
    if (functionList[29] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int *)) functionList[29])(topMargin);
        apiTime = temTimer.elapsed();
    }
    else
    {
        // LOG4CXX_DEBUG(logger_, _T("error when calling PGetTopMargin."));
    // return false;
    }
    return ok;
}

//确定之前发送的打印指令是否完成
int Printer::PPrintIsComplete(int const time_out)
{
    int code = -1;
    if (functionList[30] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        code = ((int(*)(int))functionList[30])(time_out);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PrintIsComplete."));
        //return false;
    }
    return code;
}

//获取打印机走纸长度
long Printer::PGetPrintLength()
{
    long ver = -1;
    if (functionList[31] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ver = ((long(*)(void))functionList[31])();
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling GetPrintLength."));
        //return false;
    }
    return ver;
}

bool Printer::PLoadLogoImage(int imgCount, char *imgeList[])
{
    bool ok = false;
    if (functionList[32] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        qDebug() << "before PLoadLogoImage.";
        ok = ((bool(*)(int , char *[])) functionList[32])(imgCount, imgeList);
        qDebug() << "after PLoadLogoImage.";
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling LoadLogoImage."));
        //return false;
    }
    return ok;
}

bool Printer::PLoadDiskLogo(int imgCount, char *imgFileList[])
{
    bool ok = false;
    if (functionList[33] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        qDebug() << "before PLoadDiskLogo.";
        ok = ((bool(*)(int, char *[])) functionList[33])(imgCount, imgFileList);
        qDebug() << "after PLoadDiskLogo.";
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PLoadDiskLogo."));
        //return false;
    }
    return ok;
}

bool Printer::PPrintLogo(unsigned int const xpos, unsigned int const ypos, int const index)
{
    bool ok = false;
    if (functionList[34] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int const, unsigned int const, int const)) functionList[34])(xpos, ypos, index);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PPrintLogo."));
        //return false;
    }
    return ok;
}

bool Printer::PSetAngle(unsigned int const angle)
{
    bool ok = false;
    if (functionList[35] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned int const)) functionList[35])(angle);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling PSetAngle."));
        //return false;
    }
    return ok;
}

bool Printer::PExec_ESC_POS(char* const command, int const strLen)
{
    bool ok = false;
    if (functionList[36] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char* const, int const)) functionList[36])(command, strLen);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Exec_ESC_POS."));
        //return false;
    }
    return ok;
}

#if 0
//打印大黑标
bool Printer::PrintBMark(char *barCode, int length)
{
    bool ok = false;
    if (functionList[37] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, int)) functionList[37])(barCode, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Print1DBar."));
        //return -1;
    }
    return ok;
}

//打印小黑标
bool Printer::PrintSMark(char *barCode, int length)
{
    bool ok = false;
    if (functionList[38] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(char *, int)) functionList[38])(barCode, length);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Print1DBar."));
        //return -1;
    }
    return ok;
}

//切刀模式
void Printer::CutPaperByType(int type)  //type: 0--全切 1--半切
{
    if (functionList[39] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ((void(*)(int)) functionList[39])(type);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling FeedLine."));
    }
}

//打印一维条码
bool Printer::Print1DBar(unsigned char width, unsigned char height, const char* str, char codeType, char setType)
{
    bool ok = false;
    if (functionList[40] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(unsigned char, unsigned char, const char*, char, char)) functionList[40])(
                width, height, str, codeType, setType);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Print1DBar."));
        //return -1;
    }
    return ok;
}

//热敏头及切刀寿命查询
bool Printer::GetMaintenanceCounter(int counter, unsigned long* count)
{
    bool ok = false;
    if (functionList[41] != NULL)
    {
        QTime temTimer;
        temTimer.start();
        ok = ((bool(*)(int, unsigned long*)) functionList[41])(counter, count);
        apiTime = temTimer.elapsed();
    }
    else
    {
        //LOG4CXX_DEBUG(logger_, _T("error when calling Print1DBar."));
        //return -1;
    }
    return ok;
}
#endif

/*生成黑标*/
char* Printer::CreateBlackMark(char* buf, int len)
{
    int i;

    memset(buf, 0, len);
    srand((int)time(NULL));

    for (i=0; i<len; i++)
    {
        if(0 == (rand()%2))
        {
            strcat(buf, "0");
        }
        else
        {
            strcat(buf, "1");
        }
    }
    buf[len] = '\0';

    return buf;
}


/*打印黑标样票*/
bool Printer::PrintMarkTicket(int markType)
{
    if(true != PPrinterIsReady())
    {
        qDebug("PrinterIsReady Error\n");
        return false;
    }

    char markBuf[1024] = {0};

    CreateBlackMark(markBuf,96);
    PSetCutterMode(0);

    /********head********/
    PSetLeftMargin(0x0);
    PSetAreaWidth(0x0300);
    PSetLineSpace(10);
    PSetCharSpace(0);

    PSetFont(0x00,0x10,0x01);
    PPrintString("样票<超级大乐透>");

    PSetLineSpace(0);
    PFeedLine(1);
    PSetFont(0x00,0x00,0x01);
    PPrintString("08021309112912260220   y0WIni   05151206");
    PFeedLine(1);

    PPrintString("销售点:05126 共1期 13015期  13/02/04开奖");
    PFeedLine(1);

    PPrintString("倍:1  合计:10元     2013/02/04  12:06:20");
    PFeedLine(1);
    PSetLineSpace(10);
    PSetFont(0x00,0x10,0x01);
    PPrintString("直 选 票");
    PFeedLine(1);
    PSetFont(0x00,0x00,0x01);
    PSetLineSpace(0);
    PPrintString("             前区              后区    ");

    PSetFont(0x01,0x10,0x01);
    PSetLineSpace(0);
    qsrand((uint)time(NULL));

    char dataBuf[256] = {0};
    snprintf(dataBuf, sizeof(dataBuf), "\n\n①%02d %02d %02d %02d %02d+%02d %02d",
            qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%12+1, qrand()%12+1);
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n②%02d %02d %02d %02d %02d+%02d %02d",
            qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%12+1, qrand()%12+1);
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n③%02d %02d %02d %02d %02d+%02d %02d",
            qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%12+1, qrand()%12+1);
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n④%02d %02d %02d %02d %02d+%02d %02d",
            qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%12+1, qrand()%12+1);
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n⑤%02d %02d %02d %02d %02d+%02d %02d",
            qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%35+1, qrand()%12+1, qrand()%12+1);
    PPrintString(dataBuf);
    PFeedLine(1);   //add

    //打印广告
    PSetLineSpace(15);
    PSetFont(0x00,0x00,0x01);
    PPrintString("欢迎使用广州市乐得瑞科技有限公司体彩终端！");
    PFeedLine(1);
    PPrintString("祝彩民 行大运 中大奖！");
    /********head********/

    PFeedLine(1);
    if (0 == markType)
    {
//        PrintBMark(markBuf, 96);
       PPrintBlackMark(markBuf, 96);
        emit(showText_("－大黑标样票"));
    }
    if (1 == markType)
    {
//        PrintSMark(markBuf, 96);
        PPrintBlackMark(markBuf, 96);
        emit(showText_("－小黑标样票"));
    }
   PCutPaper();
    return true;
}

/*打印条码样票*/
bool Printer::PrintCodeTicket(int codeType)
{
    if(true != PPrinterIsReady())
    {
        qDebug("PrinterIsReady Error\n");
        return false;
    }

    QString pStr;
    pStr = "08021309112912260220   y0WIni   05151206\n" +
            QString("05126 13015 2013/02/04\n");

    PSetCutterMode(0);

    /********head********/
    PSetLeftMargin(0x0);
    PSetAreaWidth(0x0300);
    PSetLineSpace(10);
    PSetCharSpace(0);

    PSetFont(0x00,0x10,0x01);
    PPrintString("样票<全国联网排列3>");
    PFeedLine(1);

    PSetLineSpace(0);
    PFeedLine(1);
    PSetFont(0x00,0x00,0x01);
    PPrintString("08021309112912260220   y0WIni   05151206");
    PFeedLine(1);

    PPrintString("销售点:05126 共1期 13015期  13/02/04开奖");


    PPrintString("倍:1  合计:10元     2013/02/04  12:06:20");
    PFeedLine(1);

    PSetLineSpace(10);
    PSetFont(0x00,0x10,0x01);
    PPrintString("直 选 票");
    PFeedLine(1);

    PSetFont(0x01,0x10,0x01);
    PSetLineSpace(0);
    qsrand((uint)time(NULL));

    char dataBuf[256] = {0};
    snprintf(dataBuf, sizeof(dataBuf), "\n\n①%2d %2d %2d", qrand()%10, qrand()%10, qrand()%10);
    pStr += QString(dataBuf+3) + "\n";
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n②%2d %2d %2d", qrand()%10, qrand()%10, qrand()%10);
    pStr += QString(dataBuf+3) + "\n";
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n③%2d %2d %2d", qrand()%10, qrand()%10, qrand()%10);
    pStr += QString(dataBuf+3) + "\n";
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n④%2d %2d %2d", qrand()%10, qrand()%10, qrand()%10);
    pStr += QString(dataBuf+3) + "\n";
    PPrintString(dataBuf);
//    PFeedLine(1);
    snprintf(dataBuf, sizeof(dataBuf), "\n\n⑤%2d %2d %2d", qrand()%10, qrand()%10, qrand()%10);
    pStr += QString(dataBuf+3) + "\n";
    PPrintString(dataBuf);
    PFeedLine(1);

    //打印广告
    PSetLineSpace(15);
    PSetFont(0x00,0x00,0x01);
    PPrintString("欢迎使用广州市乐得瑞科技有限公司体彩终端！");
    PFeedLine(1);
    PPrintString("祝彩民 行大运 中大奖！");
    PFeedLine(1);

    if (1 == codeType)
    {
        char code[] = "05151206";
//        Print1DBar(5, 150, code, 70, sizeof(code)-1);
        emit(showText_("－一维码样票"));
    }

    if (2 == codeType)
    {
        int w = 0, h = 0;
        if (PGetDpi(&w, &h))
        {
            int tmpW =(int)((3*1000.0/w) + 0.5);  //单位换算，四舍五入，3,6是经验值
            int tmpH = (int)((6*1000.0/h) + 0.5);

            PrintPDF417(tmpW, tmpH, 20 , 6, 3, U2G(pStr).data(), U2G(pStr).size()+1, 1);
            qDebug() << "PDF417-1";
        }
        else
        {
            PrintPDF417(15, 30,  20 , 6, 3, U2G(pStr).data(), U2G(pStr).size()+1, 1);
            qDebug() << "PDF417-2";
        }
        emit(showText_("－二维码样票"));
    }

   PCutPaper();

    return true;
}

/*打印其它样票*/
bool Printer::PrintOtherTicket()
{
    //打印定长票
    /* PrintSampleTicket(2);
    emit(showText_("－定长票"));
   PCutPaper();*/

    //打印非定长长票
    PSetCutterMode(1);
  PrintSampleTicket(1);
    emit(showText_("－非定长票"));
   PCutPaper();

    //打印非定长短票
  //  PSetCutterMode(1);
 PrintSampleTicket(3);
      emit(showText_("－非定长票"));
   //  PCutPaper();
      PCutPaper();

    return true;
}

bool Printer::PrintHalfCut()
{

    PFeedLine(6);
    PSetFont(0,0x22,1);
    PPrintString("半切");
    PFeedLine(10);
    PCutPaper();
}

bool Printer::PrintFullCut()
{
    PFeedLine(6);
    PSetFont(0,0x22,1);
    PPrintString("全切");
    PFeedLine(10);
    PCutPaper();
}

bool Printer::PrintSampleTicket(int flag)
{
    QTime temTimer;
    temTimer.start();

     if (PPrinterIsReady())
      {
     //设置票面格式
         if (!PSetLeftMargin(0x0010))
             qDebug() << "PSetLeftMargin(0x0010) return false!";
         if(!PSetAreaWidth(0x0300)) //0x02D0--720  0x0300--768 (GOT不支持768)
             qDebug() << "PSetAreaWidth(0x0300) return false!";
         if (!PSetAreaWidth(0x02C0))
             qDebug() << "PSetAreaWidth(0x02C0) return false!";
         if (!PSetLineSpace(27))
             qDebug() << "PSetLineSpace(27) return false!";
        // if (!PSetCutterMode(1))  //非定长切纸模式
         //    qDebug() << "PSetCutterMode(1) return false!";
      }

    if(flag==1)//长票
    {
           if (PPrinterIsReady())
           {
                                 PSetLeftMargin(0x0000);
                                 PSetAreaWidth(768);
                                 PSetLeftMargin(0x0000);
                                 PSetLineSpace(38);


                             //打印投注内容
                             PSetFont(0x10, 0x10, 0x01);
                             PPrintString("中国体育彩票\n");

                             PSetFont(0x00, 0x00, 0x01);
                             PPrintString("演示票竞彩篮球大小分");

                             PSetFont(0x00, 0x00, 0x00);
                             PPrintString("   8x28\n");
                             PPrintString("20024394571218992267   EFB9DE99   00546858\n");
                             PPrintString("─────────────────────\n");

                             //打印字符串
                             PSetFont(0x00, 0x00,0x00);
                             for (int i = 0; i < 7; i++)
                             {
                                 PPrintString("第1关 周日301\n浮动奖预设总分:298.5 固定奖预设总分:198.5\n客队:达拉斯小牛 Vs 主队:犹他爵士\n大@12.0元+小@12.0元\n");
                             }

                             //打印本张票可能的最大兑奖金额
                             PPrintString("(选项固定奖金额为每1元投注对应的奖金额)\n");
                             PPrintString("本票最高可能固定奖金:268.00元\n");

                             //打印填充符
                              // for (int k = 0; k < 3; k++)
                            // {
                            //     PPrintString(" *  *  *\n");
                           //    }

                             PPrintString("─────────────────────\n");
                             PPrintString("倍数:1 合计:3584元 2008-07-31 23:49:00\n");
                             PPrintString("311234599\n");//终端编号

                             //打印广告
                             PSetFont(0x00, 0x00, 0x01);
                             PPrintString("中国竞彩网 http://www.sporttery.cn\n");

                             PFeedLine(1);

                             //打印黑标
                             //char barCode[]="010101110000101000011111100001000100110111011011011101010111011001010100101000000010";
                             char barCode[]="010101110000101000011111100001000100110111011011011101010111011001010100101000000010010101110000";
                             PPrintBlackMark(barCode, 96); //长度不超过96,并且要求是12的倍数
                             PFeedLine(0);
           }

       }
    else if(flag==2)//定长票
    {
            if (PPrinterIsReady())
            {
               // PSetCutterMode(0);
             //   int wdpi = 203;
                   int hdpi = 203;
    //            PGetDpi(&wdpi, &hdpi);
                PSetLeftMargin(0);
                PSetAreaWidth(0x0300);
                PSetFont(0x00,0x10,0x01);
                PSetLineSpace(27 * 254 / hdpi);
                PPrintString("演示票<超级大乐透>\n");
                PSetFont(0x00,0x00,0x01);
                PPrintString("00677464905588424417   VghMgb   00031801\n");
                PPrintString("销售点:12345 05005期 共 1期 05/06/30开奖\n");
                PPrintString("倍:1  合计:682元    2012/12/10  10:15:06\n");

                PSetLineSpace(27 * 254 / hdpi);	//28刚好够打印满的胆拖票
                PSetFont(0x01,0x00,0x01);
                PPrintString("胆拖票\n");
                PPrintString("前区胆");
                PSetFont(0x01,0x10,0x01);
                PPrintString(" 01 02 03 04  .  .  .  .\n");
                PSetFont(0x01,0x00,0x01);
                PPrintString("前区拖");
                PSetFont(0x01,0x10,0x01);
                PPrintString(" 05 06 07 08 09 10 11 12\n");
                PPrintString(" 13 14 15 16 17 18 19 20 21\n");
                PPrintString(" 22 23 24 25 26 27 28 29 30\n");
                PPrintString(" 31 32 33 34 35  .  .  .  .\n");
                PSetFont(0x01,0x00,0x01);
                PPrintString("后区胆");
                PSetFont(0x01,0x10,0x01);
                PPrintString(" 01  .  .  .  .  .  .  .\n");
                PSetFont(0x01,0x00,0x01);
                PPrintString("后区拖");
                PSetFont(0x01,0x10,0x01);
                PPrintString(" 02 03 04 05 06 07 08 09\n");
                PPrintString(" 10 11 12  .  .  .  .  .  .\n");

                PSetFontEmpha(0);
                PSetFont(0x01,0x00,0x01);

                //下面的string,若是小黑标打印5行，大黑标打3行
                QSettings setting(QString("./scanner/terminal.ini"),QSettings::IniFormat);
                setting.beginGroup("printer");
                QString bmark("");
                bmark=setting.value(QString("black_mark")).toString();
                setting.endGroup();
                qDebug() << "bmark = " << bmark;
                int printNum=0;
                if (0 == bmark.compare(QString("small"), Qt::CaseInsensitive))
                {
                    qDebug() << "small:printNum = 5";
                    printNum = 5;
                } else if (0 == bmark.compare(QString("big"), Qt::CaseInsensitive))
                {
                    qDebug() << "big:printNum = 3";
                    printNum = 3;
                }


                for (int i = 0; i < printNum; i++)
                {
                    PPrintString("演示票，请勿用于销售！！！\n");
                }
    //            PPrintString("演示票，请勿用于销售！！！\n");
    //            PPrintString("演示票，请勿用于销售！！！\n");
    //            PPrintString("演示票，请勿用于销售！！！\n");
                PFeedLine(1);

                char blackMark[128] = {0};
                strcpy(blackMark, "100111100110110010101110000001110010001110111111001011000100111010101110111100010000");
                PPrintBlackMark(blackMark, 84); //长度不超过96,并且要求是12的倍数
              //  PCutPaper();
            }
    }
       else if(flag==3)//非定长短票
            {
                    if (PPrinterIsReady())
                    {
                       // PSetCutterMode(0);
                     //   int wdpi = 203;
                           int hdpi = 203;
            //            PGetDpi(&wdpi, &hdpi);
                        PSetLeftMargin(0);
                        PSetAreaWidth(0x0300);
                        PSetFont(0x00,0x10,0x01);
                        PSetLineSpace(21 * 255 / hdpi);
                        PPrintString("演示票<超级大乐透>\n");
                        PSetFont(0x00,0x00,0x01);
                        PPrintString("00677464905588424417   VghMgb   00031801\n");
                        PPrintString("销售点:12345 05005期 共 1期 05/06/30开奖\n");
                        PPrintString("倍:1  合计:682元    2012/12/10  10:15:06\n");

                        PSetLineSpace(26 * 254 / hdpi);	//28刚好够打印满的胆拖票
                        PSetFont(0x01,0x00,0x01);
                        PPrintString("胆拖票\n");
                        PPrintString("前区胆");
                        PSetFont(0x01,0x10,0x01);
                        PPrintString(" 01 02 03 04  .  .  .  .\n");
                        PSetFont(0x01,0x00,0x01);
                        PPrintString("前区拖");
                        PSetFont(0x01,0x10,0x01);
                        PPrintString(" 05 06 07 08 09 10 11 12\n");
                        PPrintString(" 13 14 15 16 17 18 19 20 21\n");
                      //  PPrintString(" 22 23 24 25 26 27 28 29 30\n");
                       // PPrintString(" 31 32 33 34 35  .  .  .  .\n");
                        PSetFont(0x01,0x00,0x01);
                        PPrintString("后区胆");
                        PSetFont(0x01,0x10,0x01);
                        PPrintString(" 01  .  .  .  .  .  .  .\n");
                        PSetFont(0x01,0x00,0x01);
                        PPrintString("后区拖");
                        PSetFont(0x01,0x10,0x01);
                        PPrintString(" 02 03 04 05 06 07 08 09\n");
                        PPrintString(" 10 11 12  .  .  .  .  .  .\n");

                        PSetFontEmpha(0);
                        PSetFont(0x01,0x00,0x01);

                        //下面的string,若是小黑标打印5行，大黑标打3行
                        QSettings setting(QString("./scanner/terminal.ini"),QSettings::IniFormat);
                        setting.beginGroup("printer");
                        QString bmark("");
                        bmark=setting.value(QString("black_mark")).toString();
                        setting.endGroup();
                        qDebug() << "bmark = " << bmark;
                        int printNum=0;
                        if (0 == bmark.compare(QString("small"), Qt::CaseInsensitive))
                        {
                            qDebug() << "small:printNum = 5";
                            printNum = 5;
                        } else if (0 == bmark.compare(QString("big"), Qt::CaseInsensitive))
                        {
                            qDebug() << "big:printNum = 3";
                            printNum = 3;
                        }

                        for (int i = 0; i < printNum; i++)
                        {
                            PPrintString("演示票，请勿用于销售！！！\n");
                        }
            //            PPrintString("演示票，请勿用于销售！！！\n");
            //            PPrintString("演示票，请勿用于销售！！！\n");
            //            PPrintString("演示票，请勿用于销售！！！\n");
                        PFeedLine(2);

                        char blackMark[128] = {0};
                        strcpy(blackMark, "100111100110110010101110000001110010001110111111001011000100111010101110111100010000");
                        PPrintBlackMark(blackMark, 84); //长度不超过96,并且要求是12的倍数
                      //  PCutPaper();
                    }
            }


        apiTime = temTimer.elapsed();

        return true;

}
