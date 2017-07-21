#include "windows.h"
#include "ui_windows.h"
#include"cthread.h"
#include <QDir>
#include <QStringList>
#include <QDesktopWidget>
#include <string.h>
#include <dirent.h>
#include <QMessageBox>

#define SO_PATH		"/home/oneu/ccpos/dist/driver/la"

const QString btnPressPic[]=
{
   "1-1-3", "1-2-3", "1-3-3",
   "2-1-3", "2-2-3", "2-3-3",
   "3-1-3", "3-2-3", "3-3-3",
   "4-1-3", "4-2-3", "4-3-3",
   "5-1-1", "picture", "usb", "stop",
   "5-2-1", "picture", "usb", "stop"
};

const QString btnReleasePic[]=
{
   "1-1-1", "1-2-1", "1-3-1",
   "2-1-1", "2-2-1", "2-3-1",
   "3-1-1", "3-2-1", "3-3-1",
   "5-1-1", "5-2-1", "5-3-1",
   "4-1-1", "4-2-1", "4-3-1", "stop",
   "4-4-1", "4-2-1", "4-3-1", "stop",
    "exit-1","reboot-1","shutdown-1"
};

const QString btnFocusPic[]=
{
   "1-1-4", "1-2-4", "1-3-4",
   "2-1-4", "2-2-4", "2-3-4",
   "3-1-4", "3-2-4", "3-3-4",
   "5-1-4", "5-2-4", "5-3-4",
   "4-1-4", "4-2-4", "4-3-4", "stop",
   "4-4-4", "4-5-4", "4-3-4", "stop",
    "exit-actived","reboot-actived","power-off-actived"
};
const QString btnObjName[]=
{
    "smpPrnBtn", "markPrnBtn", "codePrnBtn",
    "scanerRegnBtn", "hscannerRegnBtn", "exitRegnBtn",
    "ydjBrandBtn", "yqxBrandBtn", "zdyBrandBtn",
    "hwInfoBtn", "iccardBtn", "magcardBtn",
    "secondVdoBtn", "secondPicBtn", "secondUsbBtn", "secondEixtBtn",
    "thirdVdoBtn", "thirdPicBtn", "thirdUsbBtn", "thirdEixtBtn"
};




static int videoFilter(const struct dirent *dir)
{
    if (NULL != strstr(dir->d_name, ".mpg") ||
        NULL != strstr(dir->d_name, ".avi") ||
        NULL != strstr(dir->d_name, ".mp4") ||
        NULL != strstr(dir->d_name, ".mp3") ||
        NULL != strstr(dir->d_name, ".vob") ||
        NULL != strstr(dir->d_name, ".wma") ||
        NULL != strstr(dir->d_name, ".wax") ||
        NULL != strstr(dir->d_name, ".wm") ||
        NULL != strstr(dir->d_name, ".wmv") ||
        NULL != strstr(dir->d_name, ".wav") ||
        NULL != strstr(dir->d_name, ".fli") ||
        NULL != strstr(dir->d_name, ".flv") ||
        NULL != strstr(dir->d_name, ".rm")  ||
        NULL != strstr(dir->d_name, ".rmvb") ||
        NULL != strstr(dir->d_name, ".mpeg") ||
        NULL != strstr(dir->d_name, ".aac") ||
        NULL != strstr(dir->d_name, ".m4a") ||
        NULL != strstr(dir->d_name, ".m4p") ||
        NULL != strstr(dir->d_name, ".mkv"))

        return 1;
    else
        return 0;
}

static int abcSort(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

static char** readDir(const char *path)
{
    char **fileNames = NULL;
    int i, j, total;
    struct dirent **videoList;

    total = scandir(path, &videoList , videoFilter, abcSort);
    for (i=0, j=0; i<total; i++)
    {
        fileNames = (char**)realloc(fileNames,sizeof(char*)*(j+1));
        fileNames[j] =  strdup(videoList[i]->d_name);
        j++;
    }

    fileNames = (char **)realloc(fileNames,sizeof(char*)*(j+1));
    fileNames[j] = NULL;

    return fileNames;
}

//static void destroyDir(char **fileNames)
//{
//        int i;
//        for (i=0; fileNames[i] != NULL; i++)
//        {
//                free(fileNames[i]);
//        }
//        free(fileNames);
//}

char *searchFile(const char *dirPath)//, char *filePath)
{
    //int i = 0;

    QDir dir(dirPath);
    if (!dir.exists())
    {
        return NULL;
    }
    dir.setFilter(QDir::Dirs);

    QFileInfoList list = dir.entryInfoList();
    if (3 > list.size())
    {
        return NULL;
    }
    //do{
            QFileInfo fileInfo = list.at(2);

//        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
//        {
//            i++;
//            continue;
//        }

        char **fileList = readDir((char *)(fileInfo.filePath().toLatin1().data()));
        char *filePath = (fileInfo.filePath() + "/" + fileList[0]).toLatin1().data();
        return filePath;

    //}while(i < list.size());

    //return true;
}

Windows::Windows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Windows),
    printer(NULL),
    scanner(NULL),
    hscanner(NULL),
    hr200(NULL),
    scannerThread(NULL),
    hscannerThread(NULL),
    hr200Thread(NULL)
  //  seniorInfoDialog(NULL)
{
    ui->setupUi(this);
  //  loginWindow = new DialogMagcardLogin();
  //  QTimer::singleShot(10,this,SLOT(show_loginWindow()));

    setWindowFlags(Qt::FramelessWindowHint);    //标题栏隐藏
    //setAttribute(Qt::WA_TranslucentBackground);
   ui->bgallLabel->setFocus();
  // ui->samPrnlabel->show();

    for(int i=1;i<=5;i++)
         fPictureFlag[i]=0;

    move(0, 0);

   system("sh ./hr200.sh");
  // system("sh ./ic.sh");
   system("insmod ./conf/scanner/usbreader_fc14.ko");
   system("insmod ./conf/scanner/scanner_f14.ko");
   system("insmod ./conf/hscanner/cdc-acm.ko");
   system("insmod ./conf/hr200/usb_datapipe.ko");


   QFile mFile1("./HR200");
   if(mFile1.exists())
   {
       hr200 = new HScanner("HR200");
       qDebug()<<"HR200 exsit ";
   }
   else
   {
       hr200 = NULL;
       qDebug()<<"HR200 not exist  ";
   }



        printer = new Printer();
        scanner = new Scanner;
        hscanner = new HScanner("EM2028");
    scannerThread = new ScannerThread();
    hscannerThread = new HScannerThread("EM2028");
    hr200Thread = new HScannerThread("HR200");
    hwinfoquery = new HWInfoQuery();
  //  seniorInfoDialog= new seniorInfo();


   zdyBrandDialog=new zdyBrandDig();
   zdyBrandDialog->hide();

   iccard=new ICcard();
   iccard->hide();

   magcard = new Dialogmagcard();



    pSettings = new QSettings("./conf/config.ini", QSettings::IniFormat);
    pSettings->setIniCodec(QTextCodec::codecForName("UTF-8"));
  //  typeSettings = new QSettings("./conf/scanner/terminal.ini", QSettings::IniFormat);
    //typeSettings->setIniCodec(QTextCodec::codecForName("UTF-8"));
    productType=pSettings->value("Terminal/Model").toString();
    QByteArray print = pSettings->value("Driver/printer").toByteArray();
    printerPath = print.data();
    QByteArray scan = pSettings->value("Driver/scanner").toByteArray();
    scanPath = scan.data();
    QByteArray bcr = pSettings->value("Driver/hscanner").toByteArray();
    bcrPath = bcr.data();
    ui->labelType->setStyleSheet("color:white");
    if(productType.contains("900"))
        ui->labelType->setStyleSheet("background:url(:/images/LA900A.png);");
    else
        ui->labelType->setStyleSheet("background:url(:/images/LA700A.png);");


    //图片、视频存放路径
    secondVdoPath = "./videos/VGA";
    secondPicPath = "./images/VGA";
    secondUsbPath = "/media";
    thirdVdoPath  = "./videos/USB";;
    thirdPicPath  = "./images/USB";

    botton_Init();
    signed_slot_Init();
    flag_Init();
    device_Init();
    hwInfoShow();
    hide_label();
    installFilter();
 //   ui->hwInfoBtn->installEventFilter();

}



Windows::~Windows()
{
    if (printer)
    {
        delete printer;
        printer = NULL;
    }
    if (scanner)
    {
        delete scanner;
        scanner = NULL;
    }
    if (hscanner)
    {
        delete hscanner;
        hscanner = NULL;
    }
    if (scannerThread)
    {
        delete scannerThread;
        scannerThread = NULL;
    }
    if (hscannerThread)
    {
        delete hscannerThread;
        hscannerThread = NULL;
    }
    if(hwinfoquery)
    {
        delete hwinfoquery;
        hwinfoquery = NULL;
    }
    if(zdyBrandDialog)
    {
        delete zdyBrandDialog;
        zdyBrandDialog=NULL;
    }
    if(hr200Thread)
    {
        delete hr200Thread;
        hr200Thread= NULL;
    }
    if(iccard)
    {
        delete iccard;
        iccard=NULL;
    }
    if(magcard)
    {
        delete magcard;
        magcard=NULL;
    }

    on_exitBtn_clicked();

    delete ui;
}

void Windows::setupBrnMap()
{


  /*QPushButton *buttonUiTable[]=
    {
        ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
        ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
        ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
        ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
        ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
        ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn
    };

     BtnInfo btnInfo;
     for(unsigned int i=0;i<sizeof(btnObjName)/sizeof(btnObjName[0]);i++)
     {
         btnInfo.Button=buttonUiTable[i];
         btnInfo.btnPicName=btnPressPic[i];
         btnInfo.btnObjName=btnObjName[i];
         mapButton.insert(btnObjName[i],btnInfo);
     }*/

}

void Windows::hide_label()
{
    QLabel *labelUiTabel[]=
    {
        ui->samPrnlabel, ui->markPrnLbe, ui->codePrnLbe,
        ui->scanerRegLbe, ui->hscannerRegnLbe, ui->exitRegnLbe,
        ui->ydjBrandLbe, ui->yqxBrandLbe, ui->zdyBrandLbe,
        ui->hwInfoLbe, ui->iccardLbe, ui->magcardLbe,
        ui->secondVdoLbe, ui->secondPicLbe, ui->secondUsbLbe, ui->secondEixtLbe,
        ui->thirdVdoLbe, ui->thirdPicLbe, ui->thirdUsbLbe, ui->thirdEixtLbe
    };
    for(unsigned int i=0;i<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);i++)
    {
        labelUiTabel[i]->hide();
    }

}

void Windows::signed_slot_Init()    //信号槽初始化
{
    connect(ui->printBtn, SIGNAL(clicked()), this, SLOT(printerOpen()));
    connect(ui->smpPrnBtn, SIGNAL(clicked()), this, SLOT(samplePrint()));
    connect(ui->markPrnBtn, SIGNAL(clicked()), this, SLOT(markPrint()));
    connect(ui->codePrnBtn, SIGNAL(clicked()), this, SLOT(codePrint()));

    connect(ui->scanBtn, SIGNAL(clicked()), this, SLOT(scanOpen()));
    connect(ui->scanerRegnBtn, SIGNAL(clicked()), this, SLOT(scannerRun()));
    connect(ui->hscannerRegnBtn, SIGNAL(clicked()), this, SLOT(hscannerRun()));
    connect(ui->exitRegnBtn, SIGNAL(clicked()), this, SLOT(scanExit()));

    connect(ui->brandBtn, SIGNAL(clicked()), this, SLOT(brandPrnOpen()));
    connect(ui->ydjBrandBtn, SIGNAL(clicked()), this, SLOT(ydjBrandPrn()));
    connect(ui->yqxBrandBtn, SIGNAL(clicked()), this, SLOT(yqxBrandPrn()));
    connect(ui->zdyBrandBtn, SIGNAL(clicked()), this, SLOT(zdyBrandPrn()));

    connect(ui->showBtn, SIGNAL(clicked()), this, SLOT(screenShow()));
    connect(ui->secondVdoBtn, SIGNAL(clicked()), this, SLOT(secondVideo()));
    connect(ui->secondPicBtn, SIGNAL(clicked()), this, SLOT(secondPic()));
    connect(ui->secondUsbBtn, SIGNAL(clicked()), this, SLOT(secondUsb()));
    connect(ui->thirdVdoBtn, SIGNAL(clicked()), this, SLOT(thirdVideo()));
    connect(ui->thirdPicBtn, SIGNAL(clicked()), this, SLOT(thirdPic()));
    connect(ui->secondEixtBtn,SIGNAL(clicked()),this,SLOT(secondExit())); //第二屏播放退出
    connect(ui->thirdEixtBtn, SIGNAL(clicked()), this, SLOT(thirdExit()));
    connect(ui->thirdUsbBtn,SIGNAL(clicked()),this,SLOT(thirdUsb()));

    connect(ui->gaojiBtn,SIGNAL(clicked()),this,SLOT(gaojiOpen()));

    connect(printer, SIGNAL(showText(char *)), this, SLOT(showText(char *)));
    connect(printer, SIGNAL(showText_(char *)), this, SLOT(showText_(char *)));
    connect(scannerThread, SIGNAL(showTicketInfo( char *,int)), this, SLOT(showTicketInfo( char *,int)));
    connect(hscannerThread, SIGNAL(showTicketInfo(  char *,int )), this, SLOT(showTicketInfo( char *,int)));
    connect(scannerThread, SIGNAL(showText(char *)), this, SLOT(showText(char *)));
    connect(hscannerThread, SIGNAL(showText(char *)), this, SLOT(showText(char *)));
    connect(scannerThread, SIGNAL(showText_(char *)), this, SLOT(showText_(char *)));
    connect(hscannerThread, SIGNAL(showText_(char *)), this, SLOT(showText_(char *)));

    connect(hr200Thread, SIGNAL(showTicketInfo( char *,int)), this, SLOT(showTicketInfo(  char *,int)));
    connect(hr200Thread, SIGNAL(showText(char *)), this, SLOT(showText(char *)));
    connect(hr200Thread, SIGNAL(showText_(char *)), this, SLOT(showText_(char *)));



    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeCtrl(int)));

    connect(ui->hwInfoBtn,SIGNAL(clicked()),this,SLOT(hwInfoShow()));

   // connect(zdyBrandDialog,SIGNAL(getZdyInfoFinish(QString *zdyStr)),this,SLOT(getZdyInfo(QString *zdyStr)));


    connect(zdyBrandDialog,SIGNAL(zdyTextChange_v(int)),this,SLOT(zdyTextChanged(int)));//自定义打印文字变化
    connect(zdyBrandDialog,SIGNAL(zdyTextChange_ct(int)),this,SLOT(zdyTextChanged(int)));
    connect(zdyBrandDialog,SIGNAL(zdyTextChange_botm(int)),this,SLOT(zdyTextChanged(int)));
    connect(zdyBrandDialog,SIGNAL(posChoice_(QString)),this,SLOT(posChoice_(QString)));
    connect(ui->btn_jiaodui,SIGNAL(clicked()),this,SLOT(checkScreen()));

    connect(ui->magcardBtn,SIGNAL(clicked()),this,SLOT(magcardShow()));
    connect(ui->iccardBtn,SIGNAL(clicked()),this,SLOT(IC_card()));



    QPushButton *buttonUiTable[]=
      {
          ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
          ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
          ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
          ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
          ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
          ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn
      };

  for(unsigned int i=0;i<sizeof(buttonUiTable)/sizeof(buttonUiTable[0]);i++)
  {
      connect(buttonUiTable[i], SIGNAL(pressed()), this, SLOT(pressPicture()));
  }
  for(unsigned int i=0;i<sizeof(buttonUiTable)/sizeof(buttonUiTable[0]);i++)
  {
      connect(buttonUiTable[i], SIGNAL(released()), this, SLOT(releasedPicture()));
  }
}



void Windows::botton_Init() //按键状态初始化
{

    ui->prnLifeLabel->hide();//隐藏打印机寿命显示栏
    ui->printLcdNum->hide();
    ui->cutLcdNum->hide();
    ui->lengthLcdNum->hide();
    ui->operaTimeLcdNum->hide();


    ui->printBtn->setEnabled(true);
    ui->smpPrnBtn->setEnabled(false);
    ui->markPrnBtn->setEnabled(false);
    ui->codePrnBtn->setEnabled(false);



    ui->scanBtn->setEnabled(true);
    ui->scanerRegnBtn->setEnabled(false);
    ui->hscannerRegnBtn->setEnabled(false);
    ui->exitRegnBtn->setEnabled(false);

    ui->brandBtn->setEnabled(true);
    ui->ydjBrandBtn->setEnabled(false);
    ui->yqxBrandBtn->setEnabled(false);
    ui->zdyBrandBtn->setEnabled(false);
    //ui->zdytextEdit->setEnabled(false);

    ui->gaojiBtn->setEnabled(true);
    ui->hwInfoBtn->setEnabled(false);
    ui->iccardBtn->setEnabled(false);
    ui->magcardBtn->setEnabled(false);

    ui->showBtn->setEnabled(true);
    ui->secondVdoBtn->setEnabled(false);
    ui->secondPicBtn->setEnabled(false);
    ui->secondUsbBtn->setEnabled(false);
    ui->secondEixtBtn->setEnabled(false);
    ui->thirdVdoBtn->setEnabled(false);
    ui->thirdPicBtn->setEnabled(false);
    ui->thirdEixtBtn->setEnabled(false);
    ui->thirdUsbBtn->setEnabled(false);
    ui->volumeSlider->setEnabled(false);
    ui->pbn_voiceUp->setEnabled(false);
    ui->pbn_vDown->setEnabled(false);

    ui->rebootBtn->setEnabled(true);
    ui->shutdownBtn->setEnabled(true);
    ui->exitBtn->setEnabled(true);


    //ui->zdytextEdit->hide();
}

void  Windows::installFilter()
{
    QPushButton *btnUiTable[]=
     {
         ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
         ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
         ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
         ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
         ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
         ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn,ui->exitBtn,ui->shutdownBtn,ui->rebootBtn
     };
    for(unsigned int i=0;i<sizeof(btnUiTable)/sizeof(btnUiTable[0]);i++)
  {
      btnUiTable[i]->installEventFilter(this);
  }
}

void Windows::flag_Init()   //标志位初始化
{
    printerFlag = false;
    scannerFlag = false;
    hscannerFlag = false;
    hr200Flag = false;

    secondVdoFlag = false;
    secondPicFlag = false;
    secondUsbFlag = false;
    thirdVdoFlag = false;
    thirdPicFlag = false;
    thirdExitFlag = false;


    vedioPictureFlag=false;
}

void Windows::keyPressEvent(QKeyEvent *kEvent)  //终端键盘按键响应
{

   // if(seniorInfoDialog->hasFocus()) return;
   //key_counter->KeyPress(kEvent);//按键次数统计

  if (kEvent->key() == Qt::Key_F1)
    {
         zdyBrandDialog->hide();
         iccard->hide();
         magcard->hide();
         printerOpen();
         ui->smpPrnBtn->setFocus();
    }

    if (kEvent->key() == Qt::Key_F2)
    {
          zdyBrandDialog->hide();
          iccard->hide();
          magcard->hide();
        scanOpen();
        ui->scanerRegnBtn->setFocus();
    }

    if (kEvent->key() == Qt::Key_F3)
    {
          zdyBrandDialog->hide();
           iccard->hide();
           magcard->hide();
        brandPrnOpen();
        ui->ydjBrandBtn->setFocus();
    }

    if (kEvent->key() == Qt::Key_F4)
    {
          zdyBrandDialog->hide();
          iccard->hide();
          magcard->hide();
          gaojiOpen();
         // iccardOpen();
        //  magcardOpen();
          ui->hwInfoBtn->setFocus();
    }

    if(kEvent->key() == Qt::Key_F5)
    {
          zdyBrandDialog->hide();
           iccard->hide();
           magcard->hide();
          screenShow();
          ui->secondVdoBtn->setFocus();
    }

  /*  if(kEvent->key() == Qt::Key_Tab)
    {
        ui->rebootBtn->setFocusPolicy(Qt::StrongFocus);
        qDebug()<<"key Tab =======";
        ui->shutdownBtn->setFocusPolicy(Qt::StrongFocus);
        ui->exitBtn->setFocusPolicy(Qt::StrongFocus);
    }
    if(kEvent->key() == Qt::Key_Up || kEvent->key() == Qt::Key_Down || kEvent->key() == Qt::Key_Right || kEvent->key() == Qt::Key_Left)
    {
         qDebug()<<"key up right =======";
        if(!(ui->rebootBtn->hasFocus() || ui->shutdownBtn->hasFocus() || ui->exitBtn->hasFocus()))
        {
            ui->rebootBtn->setFocusPolicy(Qt::NoFocus);
            ui->shutdownBtn->setFocusPolicy(Qt::NoFocus);
            ui->exitBtn->setFocusPolicy(Qt::NoFocus);
        }
    }*/

}

void Windows::device_Init() //设备初始化，打印机、读票机、条码枪
{
//    int ret = printer->PInit("./conf/printer", "./conf/printer");
    int ret = printer->PInit(SO_PATH, SO_PATH);

    if (0 == ret)
    {
        qDebug() << "Printer Init Success... " << ret;

        ui->showTextEdit->setText("－－－打印机初始化成功");
        showPrinterLife();
        printerFlag = true;
    }
    else
    {
        qDebug() << "Printer Init Failed... " << ret;

        ui->showTextEdit->setText("－－－－打印机初始化失败");

        printerFlag = false;
    }

    if (scannerThread->InitDevice())
    {
        ui->showTextEdit->append("－－－读票机初始化成功");

        scannerFlag = true;

        scannerThread->StopRun();
    }
    else
    {
        ui->showTextEdit->append("－－－读票机初始化失败");

        scannerFlag = false;
    }

    if (hscannerThread->InitDevice("EM2028"))
    {
        ui->showTextEdit->append("－－－条码枪初始化成功");

        hscannerFlag = true;

        hscannerThread->StopRun();
    }
    else
    {
        ui->showTextEdit->append("－－－条码枪初始化失败");

        hscannerFlag = false;
    }

    if(hr200 != NULL)
    {
        if (hr200Thread->InitDevice("HR200"))
        {
            ui->showTextEdit->append("－－－外置条码枪初始化成功");

            hr200Flag = true;

            hr200Thread->StopRun();
        }
        else
        {
            ui->showTextEdit->append("－－－外置条码枪初始化失败");

            hr200Flag = false;
            hr200=NULL;
        }
    }
     /*   if(iccard->initICcard())
        {
            qDebug("ic card init true");
            icFlag=true;
        }
        else
        {
            qDebug("ic init false");
            icFlag=false;
        }*/

}

void Windows::on_exitBtn_clicked()  //退出软件按键－－退出程序
{
    if (true == secondVdoFlag)
    {
        videoDlg->ExitPlay();
        videoDlg->close();
        //system("killall NewPlayer2");
       // system("killall mplayer");
        secondVdoFlag = false;
    }
    if (true == secondPicFlag)
    {
        imageLbl->close();
        secondPicFlag = false;
    }
    if (true == secondUsbFlag)
    {
        videoDlg->close();
        //destroyDir(fileList);
        secondUsbFlag = false;
    }
    if (true == thirdVdoFlag)
    {
        system("killall NewPlayer");
        thirdVdoFlag = false;
    }
    if (true == thirdPicFlag)
    {
        system("killall eog");
        thirdPicFlag = false;
    }

    if(zdyBrandDialog)
    {
        delete zdyBrandDialog;
        zdyBrandDialog=NULL;
    }

    if(iccard)
    {
        delete iccard;
        iccard=NULL;
    }
    if(magcard)
    {
        delete magcard;
        magcard=NULL;
    }

    flag_Init();

    close();
}

void Windows::on_rebootBtn_clicked()    //重启终端按键－－系统重启
{
    system("reboot");
}

void Windows::on_shutdownBtn_clicked()  //关闭终端按键－－系统关机
{
    system("shutdown -h now");
}

void Windows::unclickPicture(int unClickedflag)
{
    if(1==unClickedflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/F1-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->printBtn->setIcon(iconp);
        ui->printBtn->setIconSize(QSize(270, 93));
    }

    if(2==unClickedflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/F2-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->scanBtn->setIcon(iconp);
        ui->scanBtn->setIconSize(QSize(270, 93));
    }
    if(3==unClickedflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/F3-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->brandBtn->setIcon(iconp);
        ui->brandBtn->setIconSize(QSize(270, 93));
    }
    if(4==unClickedflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/F5.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->gaojiBtn->setIcon(iconp);
        ui->gaojiBtn->setIconSize(QSize(270, 93));
    }
    if(5==unClickedflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/F4-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->showBtn->setIcon(iconp);
        ui->showBtn->setIconSize(QSize(271, 60));
    }

}

void Windows::clickedPicture(int clickedPicflag)
{
    if(1==clickedPicflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/disableIcon/F1-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->printBtn->setIcon(iconp);
        ui->printBtn->setIconSize(QSize(270, 93));

       /* QIcon iconz;
        iconz.addFile(QString::fromUtf8(":/images/pressIcon/1-1-3.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->smpPrnBtn->setIcon(iconz);
        ui->smpPrnBtn->setIconSize(QSize(270, 93));*/
    }
    if(2==clickedPicflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/disableIcon/F2-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->scanBtn->setIcon(iconp);
        ui->scanBtn->setIconSize(QSize(270, 93));

    }
    if(3==clickedPicflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/disableIcon/F3-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->brandBtn->setIcon(iconp);
        ui->brandBtn->setIconSize(QSize(270, 93));
    }
    if(4==clickedPicflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/disableIcon/F5-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->gaojiBtn->setIcon(iconp);
        ui->gaojiBtn->setIconSize(QSize(270, 93));
    }
    if(5==clickedPicflag)
    {
        QIcon iconp;
        iconp.addFile(QString::fromUtf8(":/images/disableIcon/F4-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->showBtn->setIcon(iconp);
        ui->showBtn->setIconSize(QSize(271, 60));
    }
}
void Windows::changPicture()
{
    for(int i=1;i<=5;i++)
    {
        if(1==fPictureFlag[i])
        {
        unclickPicture(i);
        fPictureFlag[i]=0;
        return;}
    }
    return;
}


void Windows::printerOpen() //打印机启动
{
    changPicture();
    fPictureFlag[1]=1;
    clickedPicture(1);
    show_hide_label();

    scanExit();
    botton_Init();
    ui->prnLifeLabel->show();//打印机寿命显示栏隐藏
    ui->printLcdNum->show();
    ui->cutLcdNum->show();
    ui->lengthLcdNum->show();
    ui->operaTimeLcdNum->show();


    ui->showTextEdit->setText("－－－彩票打印");

    if (true == printerFlag)
    {
        ui->showTextEdit->append("－－打印机启动成功");
        ui->smpPrnBtn->setEnabled(true);
        ui->markPrnBtn->setEnabled(true);
        ui->codePrnBtn->setEnabled(true);
    }
    if (false == printerFlag)
    {
        int ret = printer->PInit(SO_PATH, SO_PATH);
        if (0 == ret)
        {
            qDebug() << "Printer Init Success... " << ret;

            ui->showTextEdit->setText("－－－打印机初始化成功");
            ui->smpPrnBtn->setEnabled(true);
            ui->markPrnBtn->setEnabled(true);
            ui->codePrnBtn->setEnabled(true);
            printerFlag = true;
        }
        else
        {
            ui->showTextEdit->append("－－打印机启动失败");
            ui->smpPrnBtn->setEnabled(false);
            ui->markPrnBtn->setEnabled(false);
            ui->codePrnBtn->setEnabled(false);
        }

    }


      showPrinterLife();
}

void Windows::samplePrint() //样票打印，0大黑标、1小黑标、1一维码、2二维码、其它
{

    ui->showTextEdit->clear();
    //ui->showTextEdit->append("－打印大黑标样票");
    printer->PrintMarkTicket(0);
    showPrinterLife();

    //ui->showTextEdit->append("－打印小黑标样票");
    printer->PrintMarkTicket(1);
    showPrinterLife();

//    ui->showTextEdit->append("－打印一维码样票");
//    printer->PrintCodeTicket(1);
//    showPrinterLife();

    //ui->showTextEdit->append("－打印二维码样票");
    printer->PrintCodeTicket(2);
    showPrinterLife();

    //ui->showTextEdit->append("－打印定长票、非定长票");
    printer->PrintOtherTicket();

    printer->PrintHalfCut();
    printer->PrintFullCut();
    showPrinterLife();
}

void Windows:: markPrint()  //黑标打印，大黑标
{
    //ui->showTextEdit->append("－打印大黑标样票");
    ui->showTextEdit->clear();
    printer->PrintMarkTicket(0);
    showPrinterLife();
}

void Windows::codePrint()   //条码打印，二维码
{
    //ui->showTextEdit->append("－打印二维码样票");
    ui->showTextEdit->clear();
    printer->PrintCodeTicket(2);
    showPrinterLife();
}

void Windows::scanOpen() //识别彩票打开
{
    changPicture();
    fPictureFlag[2]=1;
    clickedPicture(2);
    show_hide_label();


    scanExit();
    botton_Init();

    scannerThread->ydjFlag = false;
    scannerThread->yqxFlag = false;
    scannerThread->zdyFlag = false;

    ui->showTextEdit->setText("－－－识别彩票");

    if (true == scannerFlag)
    {
        ui->showTextEdit->append("－－读票机启动成功");
        ui->scanerRegnBtn->setEnabled(true);
        ui->exitRegnBtn->setEnabled(true);
    }
    if (false == scannerFlag)
    {
        ui->showTextEdit->append("－－读票机启动失败");
        ui->scanerRegnBtn->setEnabled(false);
        ui->exitRegnBtn->setEnabled(true);
    }

    if (true == hscannerFlag)
    {
        ui->showTextEdit->append("－－条码枪启动成功");
        ui->hscannerRegnBtn->setEnabled(true);
        ui->exitRegnBtn->setEnabled(true);
    }
    if (false == hscannerFlag)
    {
        ui->showTextEdit->append("－－条码枪启动失败");
        ui->hscannerRegnBtn->setEnabled(false);
        ui->exitRegnBtn->setEnabled(true);
    }

    if(hr200 != NULL)
    {
        if (true == hr200Flag)
        {
            ui->showTextEdit->append("－－外置条码枪启动成功");
            ui->hscannerRegnBtn->setEnabled(true);
            ui->exitRegnBtn->setEnabled(true);
        }
        if (false == hr200Flag)
        {
            ui->showTextEdit->append("－－外置条码枪启动失败");
            ui->hscannerRegnBtn->setEnabled(false);
            ui->exitRegnBtn->setEnabled(true);
        }
    }


}

bool Windows::scannerRun() //读票机工作
{
//    if (scannerThread->InitDevice())
//    {
        ui->showTextEdit->append("－－读票机工作中...");
        scannerThread->StartRun();
//    }
//    else
//    {
//        ui->showTextEdit->append("－－读票机初始化失败");
//    }
    return true;
}

bool Windows::hscannerRun() //条码枪工作
{
//    if (hscannerThread->InitDevice())
//    {
        ui->showTextEdit->append("－－条码枪工作中...");
//        if (hscanner->BCRIsReady())
        {
             hscannerThread->StartRun();
        }


        if(hr200 != NULL)
        {

            if(hr200->BCRStopScan())
                qDebug()<<"BCRStopScan() ok";
            else
                qDebug()<<"BCRStopScan() false";
                hr200Thread->StartRun();

        }
//    }
//    else
//    {
//        ui->showTextEdit->append("－－条码枪初始化失败");
//    }
    return true;
}

bool Windows::scanExit() //退出识别彩票
{
    if (true == scannerThread->isWork)
    {
        scannerThread->StopRun();
    }
    if (true == hscannerThread->isWork)
    {
        hscannerThread->StopRun();

    }
   // hscannerThread->DisableBCR();
    if (true == hr200Thread->isWork)
    {
        hr200Thread->StopRun();
    }


    ui->scanerRegnBtn->setEnabled(false);
    ui->hscannerRegnBtn->setEnabled(false);
    ui->exitRegnBtn->setEnabled(false);

    return true;
}

void Windows::brandPrnOpen()    //打开标记打印
{
    changPicture();
    fPictureFlag[3]=1;
    clickedPicture(3);
    show_hide_label();

    scanExit();
    botton_Init();
    ui->showTextEdit->setText("－－－标记打印");

    if (true == scannerFlag)
    {
        ui->showTextEdit->append("－－读票机启动成功");
        ui->showTextEdit->append("－－读票机工作中...");
        scannerThread->StartRun();
        ui->ydjBrandBtn->setEnabled(true);
        ui->yqxBrandBtn->setEnabled(true);
        ui->zdyBrandBtn->setEnabled(true);

    }
    if (false == scannerFlag)
    {
        ui->showTextEdit->append("－－读票机启动失败");
        ui->ydjBrandBtn->setEnabled(false);
        ui->yqxBrandBtn->setEnabled(false);
        ui->zdyBrandBtn->setEnabled(false);
    }
    ui->zdyBrandBtn->setEnabled(true);

}

void Windows::ydjBrandPrn() //已兑奖－－1
{
   zdyBrandDialog->hide();
        //ui->showTextEdit->setText("zdyBrandDig close");

    scannerThread->ydjFlag = true;
    scannerThread->yqxFlag = false;
    scannerThread->zdyFlag = false;
//    brandPrnData(1);
}

void Windows::yqxBrandPrn() //已取消－－3
{
       zdyBrandDialog->hide();

    scannerThread->ydjFlag = false;
    scannerThread->yqxFlag = true;
    scannerThread->zdyFlag = false;
//    brandPrnData(3);
}

void Windows::zdyTextChanged(int pos)
{
     //scannerThread->zdyText = ui->zdytextEdit->toPlainText();
    QString zdyStr;
    zdyBrandDialog->getZdyStr(&zdyStr,pos);
    scannerThread->zdyText=zdyStr;
}

void Windows::posChoice_(QString zdyPos)
{
   scannerThread->zdyFlag = false;
    scannerThread->zdySetPos=zdyPos;
    qDebug("zdySetPos ...=%s",scannerThread->zdySetPos.toLatin1().data());
    if("bottom"==zdyPos)
        zdyTextChanged(1);
    if("center"==zdyPos)
        zdyTextChanged(2);
    if("Vertical"==zdyPos)
        zdyTextChanged(3);
     scannerThread->zdyFlag = true;
     //scannerThread->brandPrnData(0,scannerThread->zdySetPos);
}

void Windows::zdyBrandPrn() //自定义－－0
{
    scannerThread->ydjFlag = false;
    scannerThread->yqxFlag = false;
  //  ui->zdytextEdit->show();
   //  ui->zdytextEdit->setEnabled(true);
    //if(zdyBrandDialog != NULL)
    if(zdyBrandDialog)
    {delete zdyBrandDialog;
        zdyBrandDialog=NULL;
    }

     zdyBrandDialog=new zdyBrandDig();
     connect(zdyBrandDialog,SIGNAL(zdyTextChange_v(int)),this,SLOT(zdyTextChanged(int)));//自定义打印文字变化
     connect(zdyBrandDialog,SIGNAL(zdyTextChange_ct(int)),this,SLOT(zdyTextChanged(int)));
     connect(zdyBrandDialog,SIGNAL(zdyTextChange_botm(int)),this,SLOT(zdyTextChanged(int)));
     connect(zdyBrandDialog,SIGNAL(posChoice_(QString)),this,SLOT(posChoice_(QString)));
     zdyBrandDialog->show();
     zdyTextChanged(1);
     scannerThread->zdyFlag = true;
     //scannerThread->brandPrnData(0,"Vertical");



 //scannerThread->zdyText = ui->zdytextEdit->toPlainText();
//    brandPrnData(0);

}

/*void Windows::getZdyInfo(QString *zdyStr,int pos)
{
    if(true != scannerFlag)
        zdyBrandDialog->hide();
   scannerThread->zdyText=*zdyStr;
   zdyBrandDialog->hide();
}*/

void Windows::screenShow()  //视频演示开启
{
    changPicture();
    fPictureFlag[5]=1;
    clickedPicture(5);
    show_hide_label();

    scanExit();
    botton_Init();

    ui->secondVdoBtn->setEnabled(true);
    ui->secondPicBtn->setEnabled(true);
    ui->secondUsbBtn->setEnabled(true);
    ui->secondEixtBtn->setEnabled(true);
    ui->thirdVdoBtn->setEnabled(true);
    ui->thirdPicBtn->setEnabled(true);
    ui->thirdEixtBtn->setEnabled(true);
    ui->thirdUsbBtn->setEnabled(true);
    ui->volumeSlider->setEnabled(true);
    ui->pbn_voiceUp->setEnabled(true);
    ui->pbn_vDown->setEnabled(true);

    ui->showTextEdit->setText("－－－体彩终端多屏演示");
}

void Windows::secondVideo() //第二屏视频
{

    QString playLocalFile;
   playLocalFile = QFileDialog::getOpenFileName(this, "VGA第二屏本地视频文件选择", secondVdoPath,
                                             tr("videos (*.mpg *.avi *.mp4 *.vob *.mpeg *.wma *.wmv *.rm *.rmvb *.fli *.flv *.mkv *.wma *.m4a *.m4p *.aac *.mp3)"));
 if (NULL != playLocalFile)
 {

    if (true == secondPicFlag)
        {
             imageLbl->close();
            //delete imageLbl;  //注意delete掉图片储存
            ui->showTextEdit->setText("－－VGA第二屏本地图片播放退出");
            secondPicFlag = false;
        }
        if (true == secondVdoFlag)
        {
            videoDlg->ExitPlay();
            videoDlg->close();
            // system("killall NewPlayer2");
           //  system("killall mplayer");

            ui->showTextEdit->setText("－－VGA第二屏本地视频播放退出");
            secondVdoFlag = false;
        }
        if (true == secondUsbFlag)
        {
            videoDlg->ExitPlay();
            videoDlg->close();

            //destroyDir(fileList);
            ui->showTextEdit->setText("－－VGA第二屏U盘视频播放退出");
            secondUsbFlag = false;
        }

        {

            //QString playFile = "./videos/VGA/demo.mpg";
            videoDlg = new VideoPlay(playLocalFile, 1024, 768, this);
            videoDlg->move(0, 768);
            videoDlg->show();
            ui->showTextEdit->append("－－VGA第二屏本地视频播放中......");
            secondVdoFlag = true;

       /*
            system("./NewPlayer2 &");
            //system("ps -A | grep mplayer | awk '{print $1}'");
            ui->showTextEdit->setText("－－VGA第二屏本地视频播放中......");
            ui->secondVdoBtn->setFocus();
            secondVdoFlag = true;
            */

        }
   }
}

void Windows::secondPic()   //第二屏图片
{

    if (true == secondPicFlag)
    {
         imageLbl->close();
       // delete imageLble;
        ui->showTextEdit->setText("－－VGA第二屏本地图片播放退出");
        secondPicFlag = false;
    }
    if (true == secondVdoFlag)
    {
        videoDlg->ExitPlay();
        videoDlg->close();
       // system("killall NewPlayer2");
      //  system("killall mplayer");
        ui->showTextEdit->setText("－－VGA第二屏本地视频播放退出");
        secondVdoFlag = false;
    }
    if (true == secondUsbFlag)
    {
        videoDlg->ExitPlay();
        videoDlg->close();

        //destroyDir(fileList);
        ui->showTextEdit->setText("－－VGA第二屏U盘视频播放退出");
        secondUsbFlag = false;
    }

    {
        //const char *imageDir = "./images/VGA"; secondPicPath
       // imageLbl = new ImagePlay(secondPicPath, 1024, 768, this);
        imageLbl = new ImagePlay(secondPicPath, 1024, 768, this);
        imageLbl->move(0, 768);
        imageLbl->show();
        ui->showTextEdit->append("－－VGA第二屏本地图片播放中......");
        secondPicFlag = true;
    }

}

void Windows::secondUsb()   //第二屏U盘视频
{
    //return;

    char *filePath;

    if (!(filePath = searchFile(secondUsbPath)))
    {
        ui->showTextEdit->setText("－－找不到U盘视频文件！");
        return;
    }
//    if (!(filePath = searchFile("/media")))
//    QString playUsbFile = QString::fromLocal8Bit(filePath);
    else
    {
//    QString playUsbFile;
//    QFileDialog *fileDlg = new QFileDialog(this);
//    fileDlg->setDirectory("/media");
//    fileDlg->setFilter("*.mpg *.avi *.mp4 *.vob *.mpeg *.wma *.wmv *.rm *.rmvb *.fli *.flv");
//    fileDlg->show();
//    if (fileDlg->exec() == QDialog::Accepted)
//        playUsbFile =  fileDlg->selectedFiles()[0];

        QString playUsbFile;
        playUsbFile = QFileDialog::getOpenFileName(this, "VGA第二屏U盘视频文件选择", secondUsbPath,
                                                        tr("videos (*.mpg *.avi *.mp4 *.vob *.mpeg *.wma *.wmv *.rm *.rmvb *.fli *.flv *.mkv *.wma *.m4a *.m4p *.aac *.mp3)"));

        if (NULL != playUsbFile)
        {
            if (true == secondPicFlag)
            {
                imageLbl->close();
                delete imageLbl;
                ui->showTextEdit->setText("－－VGA第二屏本地图片播放退出");
                secondPicFlag = false;
            }
            if (true == secondVdoFlag)
            {
               videoDlg->ExitPlay();
                videoDlg->close();
              //  system("killall NewPlayer2");
               // system("killall mplayer");
               // delete videoDlg;
                ui->showTextEdit->setText("－－VGA第二屏本地视频播放退出");
                secondVdoFlag = false;
            }
            if (true == secondUsbFlag)
            {
                videoDlg->ExitPlay();
                videoDlg->close();
              //  delete videoDlg;
                //destroyDir(fileList);
                ui->showTextEdit->setText("－－VGA第二屏U盘视频播放退出");
                secondUsbFlag = false;
            }

            {
                videoDlg = new VideoPlay(playUsbFile, 1024, 768, this);
                videoDlg->move(0, 768);
                videoDlg->show();
                ui->showTextEdit->append("－－VGA第二屏U盘视频播放中......");
                secondUsbFlag = true;
            }
        }
    }
}

void Windows::secondExit() //退出第二屏播放
{
    if (true == secondVdoFlag)
    {
       videoDlg->ExitPlay();
       videoDlg->close();
        //system("killall NewPlayer2");
      //  system("killall mplayer");
        secondVdoFlag = false;
    }
    if (true == secondPicFlag)
    {
        imageLbl->close();
        secondPicFlag = false;
    }
    if (true == secondUsbFlag)
    {
        videoDlg->ExitPlay();
        videoDlg->close();
        //destroyDir(fileList);
        secondUsbFlag = false;
    }
      ui->showTextEdit->setText("－－第二屏播放演示退出");
}

void Windows::thirdVideo()  //第三屏视频
{
    if (true == thirdVdoFlag)
    {
        system("killall NewPlayer");
        ui->showTextEdit->setText("－－USB第三屏本地视频播放退出");
        thirdVdoFlag = false;
    }
    if (true == thirdPicFlag)
    {
        system("killall eog");
        ui->showTextEdit->setText("－－USB第三屏本地图片播放退出");
        thirdPicFlag = false;
    }

    {
        system("export DISPLAY=:0.1 && ./NewPlayer &");
        ui->showTextEdit->append("－－USB第三屏本地视频播放中......");
        thirdVdoFlag = true;
    }
}

void Windows::thirdPic()    //第三屏图片
{
    if (true == thirdVdoFlag)
    {
        system("killall NewPlayer");
        ui->showTextEdit->setText("－－USB第三屏本地视频播放退出");
        thirdVdoFlag = false;
    }
    if (true == thirdPicFlag)
    {
        system("killall eog");
        ui->showTextEdit->setText("－－USB第三屏本地图片播放退出");
        thirdPicFlag = false;
    }

    {
        system("eog -f -s --display=:0.1 ./images/USB &");
        ui->showTextEdit->append("－－USB第三屏本地图片播放中......");
        thirdPicFlag = true;
    }
}

void Windows::thirdUsb()
{
    qDebug("it is thirdUsbBtn press");
    return;
}



void Windows::thirdExit()   //退出第三屏播放
{
    //if (true == thirdVdoFlag)
    {
        system("killall NewPlayer");
        //ui->showTextEdit->setText("－－USB第三屏本地视频播放退出");
        thirdVdoFlag = false;
    }
    //if (true == thirdPicFlag)
    {
        system("killall eog");
        //ui->showTextEdit->setText("－－USB第三屏本地图片播放退出");
        thirdPicFlag = false;
    }
    ui->showTextEdit->setText("－－USB第三屏播放演示退出");
    thirdExitFlag = true;
}

void Windows::showTicketInfo(  char *info,int infotype)    //彩票黑标、PDF417解析并显示
{
    int i, j, m=0;
    char ms[5000], mm[5000];
    int type = 0, row = 0, col = 0, len = 0;
    char tab[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    char result[128] = { 0 };
    QString infoResult;

    type = info[0];
    len = info[1] + (info[2]<<8) + 7;
    col = info[3] + (info[4]<<8);
    row = info[5] + (info[6]<<8);

   /* int fd = open("./ticketData.sample", "+");
    write(fd, info+7, len);
    close(fd);*/

   // if(type == 1 && memcmp(info+6, buf) ==0)

    if (1 == type || 2 == type ||  3 == type)
    {
        if (1 == type)
        {
            qDebug() << "彩票投注单" << type;
            ui->showTextEdit->setText("彩票投注单");
        }
        else if (2 == type)
        {
            qDebug() << "彩票小黑标" << type;
            ui->showTextEdit->setText("彩票小黑标");
        }
        else if (3 == type)
        {
            qDebug() << "彩票大黑标" << type;
            ui->showTextEdit->setText("彩票大黑标");
        }

        for (i=7; i<len; i++)
        {

            for (j=0; j<8; j++)
            {
                if (info[i] & tab[j])
                {
                    if (2 == type)
                    {
                        ms[m] =   '*';
                        //ms[m] = 'x';
                    }
                    else
                    {
                        ms[m] = '*';
                    }
                    ms[m+1] = ' ';
                }
                else
                {
                    ms[m] = '_';
                    ms[m+1] = ' ';
                }
                m += 2;
            }
        }
        for (i=0; i<row; i++)
        {
            for (j=0; j<(m/row); j++)
            {
                mm[j] = ms[i*(m/row)+j];
            }
            mm[m/row] = 0;
            sprintf(result, "\n%s", mm);
            infoResult.append(result);
        }
    }
    else if (152 == type || -104 == type)
    {
        qDebug() << "pdf417码" << type;
        ui->showTextEdit->setText("PDF417条码");
        infoResult.append("\n");
        infoResult.append(info + 7);
    }
    else
    {
        if (101 == type)
        {
            qDebug() << "彩票一维码" << type;
            ui->showTextEdit->setText("彩票一维码");
        }
        else if (151 == infotype)
        {
            qDebug() << "Data Matrix" << type;
            ui->showTextEdit->setText("Data Matrix码");
        }
        else if (157 == infotype)
        {
            qDebug() << "QR码" << type;
            ui->showTextEdit->setText("QR码");
        }
        else//多条码
        {

            qDebug() << "其它彩票" << type;
            ui->showTextEdit->setText("其它彩票");
        }
      qDebug()<<"******stype"<<type<<" info[0]="<<info[0];
        infoResult.append("\n");
        //多条码 去除结束符
        if(info[7]>1){
            QString OutResult0 = "",OutResult ="";
            OutResult0.sprintf("%s", info + 11);
            OutResult = OutResult0 + OutResult.sprintf("%s", info+ 11+info[9]+2);//双条码，右移第一条条码所占数据位buffer[9]+2
            infoResult.append(OutResult);
            qDebug()<<"OutResult+25=="<<OutResult;
        }else {
              infoResult.append(info + 11);
        }

    }

    ui->showTextEdit->append(infoResult);
    qDebug()<<"TicketInfo: "<<infoResult;
    if(zdyBrandDialog)
        zdyBrandDialog->hide();

}

void Windows::showText(char *info)  //textEdit->setText槽函数
{
    ui->showTextEdit->setText(info);
}

void Windows::showText_(char *info) //textEdit->append槽函数
{
    ui->showTextEdit->append(info);
}

void Windows::volumeCtrl(int value) //系统音量控制
{
    value = ui->volumeSlider->value();
    QString volumeCmd;
    volumeCmd = "amixer set Master " + QString::number(value) +"%";
    system(volumeCmd.toLatin1().data());
}

void Windows::showPrinterLife() //打印机寿命显示
{
//    unsigned long printHead;
//    unsigned long halfCutHead;
//    unsigned long fullCutHead;
//    unsigned long operationTime;

//    int ret1 = printer->GetMaintenanceCounter(0x1, &printHead);
//    int ret2 = printer->GetMaintenanceCounter(0x2, &halfCutHead);
//    int ret3 = printer->GetMaintenanceCounter(0x3, &fullCutHead);
//    int ret4 = printer->GetMaintenanceCounter(0x4, &operationTime);

//    if (1==ret1 && 1==ret2 && 1==ret3&&1==ret4)
//    {
//        unsigned long printNum = printHead;
//        unsigned long cutNum = halfCutHead + fullCutHead;
//        unsigned long lengthNum=(printHead*30*10*254/203)/1000;
//        unsigned long printerWorkTime=operationTime;

//        ui->printLcdNum->display(QString::number(printNum));
//        ui->cutLcdNum->display(QString::number(cutNum));
//        ui->lengthLcdNum->display(QString::number(lengthNum));
//        ui->operaTimeLcdNum->display(QString::number(printerWorkTime));
//    }
}

void Windows::gaojiOpen() //高级功能开启
{
    changPicture();
    fPictureFlag[4]=1;
    clickedPicture(4);
    show_hide_label();

    scanExit();
    botton_Init();

    ui->hwInfoBtn->setEnabled(true);
    ui->iccardBtn->setEnabled(true);
    ui->magcardBtn->setEnabled(true);

    ui->showTextEdit->setText("   －－－高级功能");

}

void Windows::iccardOpen()
{
    changPicture();
    fPictureFlag[4]=1;
    clickedPicture(4);
    show_hide_label();

    scanExit();
    botton_Init();


   ui->iccardBtn->setEnabled(true);
   // ui->al1234armBtn->setEnabled(true);


}

void Windows::magcardOpen()
{

}


void Windows::hwInfoShow() //获取硬件信息
{
    QString showText;
    char mbinfo[4096]={0};
    char cpuinfo[4096] = {0};
    char meminfo[4096]={0};
    char diskinfo[4096]={0};
   // ui->showTextEdit->clear();
 //    showText = "";

   if(hwinfoquery->GetCPUInfo(cpuinfo, sizeof(cpuinfo))) //获取cpu信息
            {
                   // showText += tr("CPU信息：\n") + QString(cpuinfo) + "\n";
                  //   QTime temTimer;
                  //   temTimer.start();

              QString tmpstr = cpuinfo;
             QStringList tmpList = tmpstr.split('\n');
             qDebug("tmplist.size = %d\n",tmpList.size());
            for(int i=0; i<tmpList.size(); i++)
           {
              QStringList  list2 = tmpList.at(i).split('=');
              if(list2.size() == 2 && list2.at(0) == "CPUName")
               {
                                    //showText += list2.at(1) + "\n";
                                     showText += tr("CPU: ") + list2.at(1)+"\n\n";
                                   // showText += tr("CPU:  Intel(R) Core(TM)2 Duo CPU T9600 @2.53GHz") +"\n\n";
               }
           }
     //   hwTime=temTimer.elapsed();

        }
 else {qDebug("getCPUInfo false\n");
        //  showText +="getCPUInfo false\n";
          }

   if(hwinfoquery->GetMainboardInfo(mbinfo, sizeof(mbinfo)))//获取bios信息
          {
                          //showText += tr(tmp)+"\n\n";
              QString tmpstr = mbinfo;
              QStringList tmpList = tmpstr.split('\n');

              for(int i=0; i<tmpList.size(); i++)
              {
                  //qDebug()<<tmpList.at(i);
                  QStringList list2;
                  list2 = tmpList.at(i).split('=');


               /*   if(list2.size() == 2 && list2.at(0) == "Vendor")
                  {
                      //ui->labelProductVendor->setText("制造厂商："+list2.at(1));
                      showText += tr("终端厂商 =") +  list2.at(1)+ "\n";

                  }

                                 if(list2.size() == 2 && list2.at(0) == "HWVersion")
                                  {
                                          showText += tr("主板版本 = ") + list2.at(1) + "\n";
                                  }
                                  if(list2.size() == 2 && list2.at(0) == "SN")
                  {
                                          showText += tr("整机序列号 = ") + list2.at(1) + "\n";

                  }
                  if(list2.size() == 2 && list2.at(0) == "ProductDate")
                  {
                                          showText += tr("整机生产日期 = ") + list2.at(1) + "\n";

                  }*/


                                  if(list2.size() == 2 && list2.at(0) == "BIOSVendor")
                                  {
                                          showText += tr("BIOS厂商 = ") + list2.at(1) + "\n";
                                  }
                                  if(list2.size() == 2 && list2.at(0) == "BIOSVersion")
                                  {
                                          showText += tr("BIOS版本 = ") + list2.at(1) + "\n\n";
                                  }
                                /*  if(list2.size() == 2 && list2.at(0) == "UUID")
                                  {
                                          showText += tr("主板产品UUID = ") + list2.at(1) + "\n\n";
                                  }*/
              }
          }
   else {qDebug("GetMainboardInfo false\n");
          //showText +="GetMainboardInfo false\n";
            }



   if(hwinfoquery->GetMemoryInfo(meminfo, sizeof(meminfo)))//内存信息
       {
           qDebug("GetMemInfo ok\n diskinfo=\n%s",meminfo);
             //showText += tr("meminfo：\n") + QString(meminfo) + "\n\n";
                    showText += tr("内存：");
                    QString tmpstr = meminfo;
                    QStringList tmpList = tmpstr.split('\n');
                 for(int i=0; i<tmpList.size(); i++)
                     {
                            QStringList  list2 = tmpList.at(i).split('=');

                            if(list2.size() == 2 && list2.at(0) == "HWVersion")
                            {
                                    if(productType.contains("700"))
                                        showText += tr(" 类型 = ")+ pSettings->value("Memory/Type").toString() ;
                                    else
                                      // showText += tr(" 类型 = ")+  list2.at(1);
                                        showText += tr(" 类型 = ")+  "DDR3";
                            }
                            if(list2.size() == 2 && list2.at(0) == "Capacity")
                             {
                                    //showText += tr(" 类型 = ")+ pSettings->value("Memory/Type").toString() +
                                      showText += tr(", 容量 = ") +list2.at(1)+"\n";
                                }
                     }
      }
   else {qDebug("GetMemoryInfo false\n");
       // showText +="GetMemoryInfo false\n";
            }


     if(hwinfoquery->GetDiskInfo(diskinfo, sizeof(diskinfo)))//磁盘信息
      {
        qDebug("GetDiskInfo ok\n diskinfo=\n%s",diskinfo);
          //showText += tr("Disk：\n") + QString(diskinfo) + "\n\n";
                    QString tmpstr = diskinfo;
        QStringList tmpList = tmpstr.split('\n');
        qDebug("list size2 =%d",tmpList.size());
        for(int i=0; i<tmpList.size(); i++)
        {
            qDebug()<<tmpList.at(i);
            QStringList  list2 = tmpList.at(i).split('=');

            if(list2.size() == 2 && list2.at(0) == "ProductType")
            {

                    showText += tr("存储： 类型 = ")+ list2.at(1)+
                // showText += tr("存储： 类型 = ")+ pSettings->value("Disk/Type").toString() +
                 tr(",  容量 = ") + QString::number(128)+ "G\n\n";;

            }

            else qDebug("ProductType false");


        }


     }
     else {qDebug("GetDiskInfo false\n");
         //showText +="GetDiskInfoo false\n";
                  }


     char pSwinfo[1024] = {0};
     if(printer->PGetSWVersion(pSwinfo,1024))//打印机驱动库版本
     {
                         qDebug()<<pSwinfo;
                         showText += tr("打印机驱动库版本 = ") + QString(pSwinfo) + "\n";
     }



     char sSwinfo[1024] = {0};
     if(scanner->SGetSWVersion(sSwinfo,1024))//读票机驱动库版本
     {
                         qDebug()<<sSwinfo;
                         showText += tr("读票机驱动库版本 = ") + QString(sSwinfo) + "\n";
     }
     else
         qDebug("SGetSWVersion false\n");


       char hSwinfo[1024] = {0};
       hscanner->BCRGetSWVersion(hSwinfo,sizeof(hSwinfo));//条码枪驱动库版本
          qDebug()<<hSwinfo;
          showText += tr("条码枪驱动库版本 = ") + QString(hSwinfo) + "\n";

          char hr200Swinfo[1024] = {0};
         if(hr200 != NULL)
          {
                 hr200->BCRGetSWVersion(hr200Swinfo,sizeof(hr200Swinfo));//外置条码枪驱动库版本
                 qDebug()<<hr200Swinfo;
                 showText += tr("HR200驱动库版本 = ") + QString(hr200Swinfo) + "\n";
          }

 ui->showTextEdit->setText(showText);

}


void Windows::showCount(int keycode)
{
    qDebug("keycode = =%d",keycode);
     //  keyCountDig->KeyCount(keycode);
}


void  Windows::pressPicture()
{
    //QPushButton *pressedButton = qobject_cast<QPushButton *>(sender());
   // qDebug()<<pressedButton->objectName();

 /*BtnInfo btnInfo;
 QMap<QString, BtnInfo>::iterator iter;
 iter = mapButton.find(pressedButton->objectName());
 if(iter == mapButton.end())
 {
    qDebug("no found match Button\n");
    return;
 }
 btnInfo = iter.value();
QIcon iconp;
QString btnPIcPath=":/images/pressIcon/"+btnInfo.btnPicName+".png";

iconp.addFile(QString::fromUtf8(btnPIcPath.toLatin1().data()), QSize(), QIcon::Normal, QIcon::Off);
btnInfo.Button->setIcon(iconp);
btnInfo.Button->setIconSize(QSize(270,93));*/

   QPushButton *pressedButton = qobject_cast<QPushButton *>(sender());
    QPushButton *btnUiTable[]=
      {
          ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
          ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
          ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
          ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
          ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
          ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn
      };
  for(unsigned int i=0;i<sizeof(btnUiTable)/sizeof(btnUiTable[0]);i++)
 {
     if(pressedButton->objectName()==btnObjName[i])
      {
          QString btnPIcPath=":/images/pressIcon/"+btnPressPic[i]+".png";
          QIcon iconp;
          iconp.addFile(QString::fromUtf8(btnPIcPath.toLatin1().data()), QSize(), QIcon::Normal, QIcon::Off);
          btnUiTable[i]->setIcon(iconp);
          btnUiTable[i]->setIconSize(QSize(270,93));
      }
 }

}

void Windows::releasedPicture()
{
    qDebug("it is release\n");
    QPushButton *releasedButton = qobject_cast<QPushButton *>(sender());
    qDebug()<<releasedButton->objectName();


    QPushButton *btnUiTable[]=
      {
          ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
          ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
          ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
          ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
          ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
          ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn
      };
  for(unsigned int i=0;i<sizeof(btnUiTable)/sizeof(btnUiTable[0]);i++)
 {
     if(releasedButton->objectName()==btnObjName[i])
      {
         // QString btnPIcPath=":/images/"+btnReleasePic[i]+".png";
          QString btnPIcPath=":/images/active-off/"+btnFocusPic[i]+".png";
          QIcon iconp;
          iconp.addFile(QString::fromUtf8(btnPIcPath.toLatin1().data()), QSize(), QIcon::Normal, QIcon::Off);
          btnUiTable[i]->setIcon(iconp);
          btnUiTable[i]->setIconSize(QSize(124,93));
      }
 }
  if(releasedButton->objectName()=="exitRegnBtn")
  {
      ui->scanerRegLbe->show();
      ui->hscannerRegnLbe->show();
      ui->exitRegnLbe->show();

      ui->scanerRegnBtn->hide();
      ui->hscannerRegnBtn->hide();
      ui->exitRegnBtn->hide();
  }
}

void Windows::show_hide_label()
{
  QLabel *labelUiTabel[] =
    {
        ui->samPrnlabel, ui->markPrnLbe, ui->codePrnLbe,
        ui->scanerRegLbe, ui->hscannerRegnLbe, ui->exitRegnLbe,
        ui->ydjBrandLbe, ui->yqxBrandLbe, ui->zdyBrandLbe,
        ui->hwInfoLbe, ui->iccardLbe, ui->magcardLbe,
        ui->secondVdoLbe, ui->secondPicLbe, ui->secondUsbLbe, ui->secondEixtLbe,
        ui->thirdVdoLbe, ui->thirdPicLbe, ui->thirdUsbLbe, ui->thirdEixtLbe
    };
  QPushButton *btnUiTable[]=
    {
        ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
        ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
        ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
        ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
        ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
        ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn
    };
  if (1==fPictureFlag[1])
  {
       for(int j=0;j<=2;j++)
       {
           labelUiTabel[j]->hide();
           btnUiTable[j]->show();
       }

      for(unsigned int i=3;i<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);i++)
         {
             labelUiTabel[i]->show();
             btnUiTable[i]->hide();
         }
  }
  if (1==fPictureFlag[2])
  {
        for(int i=0;i<=2;i++)
         {
             labelUiTabel[i]->show();
             btnUiTable[i]->hide();

             labelUiTabel[i+3]->hide();
             btnUiTable[i+3]->show();
         }
      for(unsigned int j=6;j<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);j++)
      {
          labelUiTabel[j]->show();
          btnUiTable[j]->hide();
      }
  }

  if (1==fPictureFlag[3])
  {
        for(int i=0;i<=5;i++)
         {
             labelUiTabel[i]->show();
             btnUiTable[i]->hide();
         }
        for(int j=6;j<=8;j++)
         {
            labelUiTabel[j]->hide();
             btnUiTable[j]->show();
         }
      for(int m=9;m<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);m++)
      {
          labelUiTabel[m]->show();
          btnUiTable[m]->hide();
      }
  }
    if (1==fPictureFlag[4])
    {
        for(int i=0;i<=8;i++)
         {
             labelUiTabel[i]->show();
             btnUiTable[i]->hide();
         }
        for(int j=9;j<=11;j++)
         {
            labelUiTabel[j]->hide();
            btnUiTable[j]->show();
         }
        // labelUiTabel[10]->show();
       //  btnUiTable[10]->hide();//高级功能 温度监控等功能隐藏

        for(unsigned int m=12;m<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);m++)//设置F5以下按钮图片
        {
            labelUiTabel[m]->show();
            btnUiTable[m]->hide();
        }
    }
    if (1==fPictureFlag[5])
    {
        for(int i=0;i<=11;i++)
         {
             labelUiTabel[i]->show();
             btnUiTable[i]->hide();
         }
        for(unsigned int m=12;m<sizeof(labelUiTabel)/sizeof(labelUiTabel[0]);m++)
        {
            labelUiTabel[m]->hide();
            btnUiTable[m]->show();
        }
    }

}

void Windows::checkScreen()
{
    qDebug("it is screenTests");
    system("./touchcfg.sh");
}

void Windows::show_Keyboard()
{
    // keyCountDig->keyShow();
}


bool  Windows::eventFilter(QObject *watched, QEvent *event)
{
     QPushButton *btnUiTable[]=
      {
          ui->smpPrnBtn, ui->markPrnBtn, ui->codePrnBtn,
          ui->scanerRegnBtn, ui->hscannerRegnBtn, ui->exitRegnBtn,
          ui->ydjBrandBtn, ui->yqxBrandBtn, ui->zdyBrandBtn,
          ui->hwInfoBtn, ui->iccardBtn, ui->magcardBtn,
          ui->secondVdoBtn, ui->secondPicBtn, ui->secondUsbBtn, ui->secondEixtBtn,
          ui->thirdVdoBtn, ui->thirdPicBtn, ui->thirdUsbBtn, ui->thirdEixtBtn,ui->exitBtn,ui->rebootBtn,ui->shutdownBtn
      };
      for(unsigned int i=0;i<sizeof(btnUiTable)/sizeof(btnUiTable[0]);i++)
    {
         if(watched == btnUiTable[i])
         {
             if(event->type() == QEvent::FocusIn)
            {
                QString btnPIcPath=":/images/active-off/"+btnFocusPic[i]+".png";
                QIcon iconp;
                iconp.addFile(QString::fromUtf8(btnPIcPath.toLatin1().data()), QSize(), QIcon::Normal, QIcon::Off);
                btnUiTable[i]->setIcon(iconp);
                btnUiTable[i]->setIconSize(QSize(124,93));
             }

             if(event->type() == QEvent::FocusOut)
            {
                QString btnPIcPath=":/images/"+btnReleasePic[i]+".png";
                QIcon iconp;
                iconp.addFile(QString::fromUtf8(btnPIcPath.toLatin1().data()), QSize(), QIcon::Normal, QIcon::Off);
                btnUiTable[i]->setIcon(iconp);
                btnUiTable[i]->setIconSize(QSize(124,93));
             }


         }
    }



   /*
         if(event->type() == QEvent::FocusOut)
         {
             QIcon icon;
             QPixmap pixmap(":/images/5-2-1.png");
             icon.addPixmap(pixmap);
             ui->iccardBtn->setIcon(icon);

         }
    }*/

    return QWidget::eventFilter(watched,event);

}






void Windows::on_pbn_voiceUp_clicked()
{
    int value = ui->volumeSlider->value();
    value +=5;
    ui->volumeSlider->setValue(value);
    QString volumeCmd;
    volumeCmd = "amixer set Master " + QString::number(8) +"%+";
    system(volumeCmd.toLatin1().data());
}

void Windows::on_pbn_vDown_clicked()
{
        int value = ui->volumeSlider->value();
        value -=5;
        if(value>=0)
        ui->volumeSlider->setValue(value);
        QString volumeCmd;
        volumeCmd = "amixer set Master " + QString::number(5) +"%-";
        system(volumeCmd.toLatin1().data());
}

void Windows::on_btn_toLHSystem_clicked()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<老化烤机程序>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);



    if(msg == QMessageBox::Yes)
    {
        if (printer)
        {
            delete printer;
          //  printer = NULL;
        }
        if (scanner)
        {
            delete scanner;

        }
        if (hscanner)
        {
            delete hscanner;

        }
        if (scannerThread)
        {
            delete scannerThread;

        }
        if (hscannerThread)
        {
            delete hscannerThread;

        }
        if(hr200Thread)
        {
            delete hr200Thread;
        }
        if(hwinfoquery)
        {
            delete hwinfoquery;

        }

        if(zdyBrandDialog)
        {
            delete zdyBrandDialog;

        }
        if(iccard)
        {
            delete iccard;
            iccard=NULL;
        }

        if(magcard)
        {
            delete magcard;
            magcard=NULL;
        }

//        qApp->processEvents();

//        system("ln -s /root/TestSystem/start.sh /root/TestSystem/localsetup  -f");
//        bool ret = QProcess::startDetached("sh",QStringList("/root/TestSystem/start.sh"));
//        qDebug("ret = %d",ret);
//        on_exitBtn_clicked();
//        qApp->quit();
        system("ln -s /root/TestSystem/changeLATECH.sh /root/TestSystem/localsetup  -f");
          qApp->closeAllWindows();
              exit(1);
    }
    else
        qDebug()<<"no";

}

void Windows::on_btn_toSCSystem_clicked()
{

    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<生产测试程序>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if(msg == QMessageBox::Yes)
    {
        if (printer)
        {
            delete printer;
          //  printer = NULL;
        }
        if (scanner)
        {
            delete scanner;

        }
        if (hscanner)
        {
            delete hscanner;
        }
        if (scannerThread)
        {
            delete scannerThread;

        }
        if (hscannerThread)
        {
            delete hscannerThread;

        }
        if(hr200Thread)
        {
            delete hr200Thread;
        }
        if(hwinfoquery)
        {
            delete hwinfoquery;

        }

        if(zdyBrandDialog)
        {
            delete zdyBrandDialog;

        }

        if(iccard)
        {
            delete iccard;
            iccard=NULL;
        }

        if(magcard)
        {
            delete magcard;
            magcard=NULL;
        }
//        qApp->processEvents();
//        system("ln -s /root/TestSystem/startTestSystem.sh /root/TestSystem/localsetup  -f");
//        bool ret = QProcess::startDetached("sh",QStringList("/root/TestSystem/startTestSystem.sh"));
//        qDebug("ret = %d",ret);
//        on_exitBtn_clicked();
//        qApp->quit();
         system("ln -s /root/TestSystem/changeTestSystem.sh /root/TestSystem/localsetup  -f");
          qApp->closeAllWindows();
           exit(3);
    }
    else
        qDebug()<<"no";
}

void Windows::on_btn_tochoiceDialog_clicked()
{
    QMessageBox msgBox;
    int msg=msgBox.information(this,"warning", "下次开机将进入<<选择引导界面>>", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);


    if(msg == QMessageBox::Yes)
    {
        if (printer)
        {
            delete printer;
        }
        if (scanner)
        {
            delete scanner;

        }

        if (hscanner)
        {
            delete hscanner;

        }
        if (scannerThread)
        {
            delete scannerThread;

        }
        if(hr200Thread)
        {
            delete hr200Thread;
        }
        if (hscannerThread)
        {
            delete hscannerThread;

        }
        if(hwinfoquery)
        {
            delete hwinfoquery;

        }

        if(zdyBrandDialog)
        {
            delete zdyBrandDialog;

        }
        if(iccard)
        {
            delete iccard;
            iccard=NULL;
        }
        if(magcard)
        {
            delete magcard;
            magcard=NULL;
        }
        qApp->processEvents();

//        system("ln -s /root/TestSystem/choiceDialog  /root/TestSystem/localsetup  -f");
//        bool ret =  QProcess::startDetached("/root/TestSystem/choiceDialog",QStringList());
//        qDebug("ret = %d",ret);
//        on_exitBtn_clicked();
//        qApp->quit();
        system("ln -s /root/TestSystem/changeChoice.sh /root/TestSystem/localsetup  -f");
         qApp->closeAllWindows();
            exit(4);
    }
    else
        qDebug()<<"no";
}

void Windows::IC_card()
{
    if(iccard)
        delete iccard;

    iccard = new ICcard();
    iccard->show();

}
void Windows::magcardShow()
{
    if(magcard)
        delete magcard;

    magcard = new Dialogmagcard();
    magcard->show();
}


/*void Windows::show_loginWindow()
{
    loginWindow->show();
    loginWindow->move(0,0);
    loginWindow->raise();
}*/


