#include "zdybranddig.h"
#include "ui_zdybranddig.h"

zdyBrandDig::zdyBrandDig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::zdyBrandDig)
{
    ui->setupUi(this);
     setWindowFlags(Qt::FramelessWindowHint);
     move(662,63);
     ui->zdyBottomBtn->setFocus();
    // ui->label->setFocus();
     connect(ui->enterBtn,SIGNAL(clicked()),this,SLOT(closeDig()));
     connect(ui->zdyBottomBtn,SIGNAL(clicked()),this,SLOT(posChoice()));
     connect(ui->zdyCenterBtn,SIGNAL(clicked()),this,SLOT(posChoice()));
     connect(ui->zdyVerBtn,SIGNAL(clicked()),this,SLOT(posChoice()));
     connect(ui->zdyTextEdit_v,SIGNAL(textChanged()),this,SLOT(emitTextChge_v()));
     connect(ui->zdyTextEdit_ct,SIGNAL(textChanged()),this,SLOT(emitTextChge_ct()));
     connect(ui->zdyTextEdit_botm,SIGNAL(textChanged()),this,SLOT(emitTextChge_botm()));
}


zdyBrandDig::~zdyBrandDig()
{
    delete ui;

}

void zdyBrandDig::setZdyType()
{
   /* if(ui->zdyCheckBox_1->isChecked())
    {
        ui->zdyCheckBox_2->toggle();
        ui->zdyCheckBox_3->toggle();
    }
    if(ui->zdyCheckBox_2->isChecked())
    {
        ui->zdyCheckBox_1->toggle();
        ui->zdyCheckBox_3->toggle();
    }
    if(ui->zdyCheckBox_3->isChecked())
    {
        ui->zdyCheckBox_1->setCheckState(Qt::Unchecked);
        ui->zdyCheckBox_2->setCheckState(Qt::Unchecked);
    }
    return;*/
}

void zdyBrandDig::getZdyStr(QString *zdyStr,int pos)
{
    if(pos==1)
    {
        qDebug("getdyStr bottom");
    *zdyStr=ui->zdyTextEdit_botm->toPlainText();
    return;
    }
    if(pos==2)
    {
        qDebug("getdyStr center");
    *zdyStr=ui->zdyTextEdit_ct->toPlainText();
    return;
    }
    if(pos==3)
    {
        qDebug("getdyStr v");
    *zdyStr=ui->zdyTextEdit_v->toPlainText();
    return;
    }
}

void zdyBrandDig::posChoice()
{
    QString zdyPos;
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(btn->objectName()=="zdyBottomBtn")
       {
           zdyPos="bottom";
           qDebug("bottomBtn is clicked...");
           emit posChoice_(zdyPos);
         //  hide();
        }
           if(btn->objectName()=="zdyCenterBtn")
        {
            zdyPos="center";
            qDebug("centerBtn is clicked... ");
             emit posChoice_(zdyPos);
             // hide();
          }
            if(btn->objectName()=="zdyVerBtn")
        {
           zdyPos="Vertical";
           qDebug("VerticalBtn is clicked... ");
           emit posChoice_(zdyPos);
          //  hide();
        }
}


void zdyBrandDig::closeDig()
{
    close();
}

void zdyBrandDig::emitTextChge_botm()
{
    emit zdyTextChange_v(1);
    emit posChoice_("bottom");
}

void zdyBrandDig::emitTextChge_ct()
{
    emit zdyTextChange_ct(2);
    emit posChoice_("center");
}

void zdyBrandDig::emitTextChge_v()
{
    emit zdyTextChange_botm(3);
    emit posChoice_("Vertical");
}

