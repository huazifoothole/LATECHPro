#include "iccard.h"
#include "ui_iccard.h"
#include <string.h>
#include "stdio.h"

ICcard::ICcard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ICcard)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    move(662,63);
    iclib = new ICcardLib();
    ICdev=-1;
    initICcard();
    ui->lineEdit->setFocus();
    on_queryButton_clicked();
}

ICcard::~ICcard()
{
    delete ui;
}

bool ICcard::initICcard()
{
            int icdev,st;
            int iNode = 0;
            char szNode[128];
           unsigned char szVer[128];
    //usb port ...
            do
           {
                    if(iNode >3) return false;

                    sprintf(szNode, "/dev/usb/hiddev%d", iNode);
                    printf("now is %s\n",szNode);
                    iNode++;
                    if((icdev=iclib->IC_InitPort_ex(2,szNode,115200))== -1)
                    {
                            printf("fw_init_ex ERR %d\n",icdev);
                            return false;
                    }
                    else
                    {
                            printf("Init Port ok\n");
                            st = iclib->IC_ReadVer(icdev, szVer);
                            if(st ==0)
                            {
                                    printf("dev:%s\n",szVer);
                                    ICdev=icdev;
                                    qDebug()<<"ICDEV=="<<ICdev;
                                    iclib->IC_DevBeep(icdev,5);                 
                                    return true;
                            }
                            else
                            {
                                    printf("%s ReadVer error\n",szNode);
                            }
                    }
           }while(icdev != -1);
}

void ICcard::on_rechargeButton_clicked()
{
    QString text=ui->lineEdit->text().trimmed();
    if(text==" ")
        return;
    int st,icReadLen,icWriteLen,count1,count2 ,writeCount;
    unsigned char icRead[2000];

    icReadLen=180;
    icWriteLen=180;

    qDebug()<<"ICdev=="<<ICdev;
    iclib->IC_Choose_24Cxx(ICdev);
    if(iclib->IC_Read(ICdev,0, 30, icRead)!=0)
      {
      qDebug("srd_24c64 ERR\n");
       }
    icRead[icReadLen]='\x0';

   qDebug()<<"Read data="<<QString(QLatin1String((char*)icRead));

   count1 = QString(QLatin1String((char*)icRead)).toInt();
   count2 = ui->lineEdit->text().trimmed().toInt();
   writeCount = count1 + count2;
   qDebug()<<"writeCount="<<writeCount;


    QString str2=QString::number(writeCount);
    int strlen=str2.length();
    qDebug()<<"length="<<strlen;
    QString str3=str2+QString("\x0");
    qDebug()<<"str3=="<<str3;


    iclib->IC_Choose_24Cxx(ICdev);
    if((st=iclib->IC_Write(ICdev,0, str3.length()+1, (unsigned char*)str3.toLatin1().data()))!=0)
     {
       qDebug()<<"swr_24c64 ERR="<<st;
       ui->label_tip->setText("充值失败");
       return;
      }
    else
    {
       //  ui->lineEdit->setText("充值成功");
         ui->label_tip->setText("充值成功");
         qDebug("swr_24c64 OK\n");
    }


     if(iclib->IC_Read(ICdev,0, str3.length()+1, icRead)!=0)
       {
       qDebug("srd_24c64 ERR\n");

        }
    qDebug()<<"Read data="<<icRead;
  //  ui->lineEdit->setText(string2+"\n"+"余额="+string);
     ui->label_tip->setText("充值成功");
    ui->label_show->setText("卡内余额: "+QString(QLatin1String((char*)icRead)));

}

void ICcard::on_queryButton_clicked()
{
    int icReadLen,icWriteLen ;
    unsigned char icRead[2000];

    icReadLen=180;
    icWriteLen=180;

    bool flag=true;
    int ret=iclib->IC_Choose_24Cxx(ICdev);
    qDebug()<<"IC_Choose_24Cxx return=="<<ret;
    if(iclib->IC_Read(ICdev,0, 30, icRead)!=0)
      {
      qDebug("srd_24c64 ERR\n");
      ui->label_tip->setText("读取错误");
      flag=false;
       }
    icRead[icReadLen]='\x0';
   printf("Read data: %s\n",icRead);
  // ui->lineEdit->setText("余额:"+QString(QLatin1String((char*)icRead)));
   if(flag)
       ui->label_show->setText("卡内余额:"+QString(QLatin1String((char*)icRead)));
   else
       ui->label_show->setText("卡内余额:");

}
