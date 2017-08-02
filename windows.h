#ifndef WINDOWS_H
#define WINDOWS_H

#include <QDialog>
#include <QFile>
#include <QKeyEvent>
#include <QFileDialog>
#include <QSettings>
#include "printer.h"
#include "scanner.h"
#include "hscanner.h"
#include "scannerthread.h"
#include "hscannerthread.h"
#include "videoplay.h"
#include "imageplay.h"
#include "hwinfoquery.h"
#include"zdybranddig.h"
#include "iccard.h"
#include"dialogmagcardlogin.h"
#include"dialogmagcard.h"

struct BtnInfo
{
    QPushButton *Button;
    QString btnPicName;
    QString btnObjName;
};



namespace Ui {
    class Windows;
}

class Windows : public QDialog
{
    Q_OBJECT

public:
    explicit Windows(QWidget *parent = 0);
    ~Windows();

    Ui::Windows *ui;
    Printer *printer;
    Scanner *scanner;
    HScanner *hscanner;
    HScanner *hr200;
    ScannerThread *scannerThread;
    HScannerThread *hscannerThread;
    HScannerThread *hr200Thread;
    VideoPlay *videoDlg;
    ImagePlay *imageLbl;
    HWInfoQuery *hwinfoquery;
  //  keycounter *keyCountDig;
   zdyBrandDig *zdyBrandDialog;
   ICcard *iccard;
 //  DialogMagcardLogin *loginWindow;
   Dialogmagcard *magcard;

    int  i_hwinfoinex;
    bool printerFlag;
    bool scannerFlag;
    bool hscannerFlag;
    bool secondVdoFlag;
    bool secondPicFlag;
    bool secondUsbFlag;
    bool thirdVdoFlag;
    bool thirdPicFlag;
    bool thirdExitFlag;
    bool vedioPictureFlag;
    int fPictureFlag[6];
    bool hr200Flag;
    bool icFlag;

   /* bool F1PictureFlag;
    bool F2PictureFlag;
    bool F3PictureFlag;
    bool F4PictureFlag;
    bool F5PictureFlag;*/


    bool keyflag;

    const char *secondVdoPath;
    const char *secondPicPath;
    const char *secondUsbPath;
    const char *thirdVdoPath;
    const char *thirdPicPath;

    void signed_slot_Init();
    void botton_Init();
    void flag_Init();
    void device_Init();
    void unclickPicture(int);
    void clickedPicture(int);
    void changPicture();
    void setupBrnMap();
    void show_hide_label();
    void hide_label();
    void installFilter();

    void setUserIni(int type);


protected:
    void keyPressEvent(QKeyEvent *);

    bool eventFilter(QObject *, QEvent *);



 signals:
    void zdyTextChage_v();
    void zdyTextChage_ct();
    void zdyTextChage_botm();




public slots:
    void  showCount(int);
    void on_shutdownBtn_clicked();
    void on_rebootBtn_clicked();
    void on_exitBtn_clicked();

    void printerOpen();
    void samplePrint();
    void markPrint();
    void codePrint();

    void scanOpen();
    bool scannerRun();
    bool hscannerRun();
    bool scanExit();

    void brandPrnOpen();
    void ydjBrandPrn();
    void yqxBrandPrn();
    void zdyBrandPrn();

    void screenShow();
    void secondVideo();
    void secondPic();
    void secondUsb();
    void thirdVideo();
    void thirdPic();
    void thirdExit();
    void secondExit();
    void thirdUsb();

    void gaojiOpen();

    void showTicketInfo( char * , int);
    void showText(char *);
    void showText_(char *);

    void volumeCtrl(int);

    void showPrinterLife();

    void pressPicture();
    void releasedPicture();
   // void pressTest();
     void posChoice_(QString);

     void IC_card();
     void magcardShow();
     void iccardOpen();
     void magcardOpen();

private slots:
    void hwInfoShow();
    void zdyTextChanged(int);
    void checkScreen();
    void show_Keyboard();

    void on_pbn_voiceUp_clicked();

    void on_pbn_vDown_clicked();

    void on_btn_toLHSystem_clicked();

    void on_btn_toSCSystem_clicked();

    void on_btn_tochoiceDialog_clicked();


 //   void show_loginWindow();




private:
    QSettings* pSettings;
    QSettings *typeSettings;
    QString productType;
    char* printerPath, *scanPath,  *bcrPath;
    QSettings*pBlackMark;
    QMap<QString, BtnInfo> mapButton;

};

#endif // WINDOWS_H
