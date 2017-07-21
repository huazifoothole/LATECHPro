#include "dialogmagcardlogin.h"
#include "ui_dialogmagcardlogin.h"
#include<QMessageBox>
#include<QTimer>

DialogMagcardLogin::DialogMagcardLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMagcardLogin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
   // ui->lineEdit->setPlaceholderText("请刷卡");
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

DialogMagcardLogin::~DialogMagcardLogin()
{
    delete ui;
}

void DialogMagcardLogin::keyPressEvent(QKeyEvent *kEvent)
{
    if(kEvent->key() == Qt::Key_Return)
    {
        if(ui->lineEdit->text().compare("123456")==0 || ui->lineEdit->text().compare(";18423651524?")==0)
        {
            ui->label->setText("验证通过");
            QTimer::singleShot(750,this,SLOT(close()));
         }
        else
        {
            QMessageBox *msgBox =new QMessageBox(QMessageBox::Information,"waring","个人信息验证失败",QMessageBox::Yes,this);
            msgBox->show();
            QTimer::singleShot(1000,msgBox,SLOT(close()));
            ui->lineEdit->clear();
            ui->lineEdit->setFocus();
        }
    }
}

void DialogMagcardLogin::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.compare("123456")==0 || arg1.compare(";18423651524?")==0)
    {
        ui->label->setText("验证通过");
        QTimer::singleShot(750,this,SLOT(close()));
    }
}

void DialogMagcardLogin::on_pushButtonEnsure_clicked()
{
    if(ui->lineEdit->text().compare("123456")==0||ui->lineEdit->text().compare(";18423651524?")==0)
    {
         QTimer::singleShot(750,this,SLOT(close()));
    }
    else
    {
        QMessageBox *msgBox =new QMessageBox(QMessageBox::Information,"waring","个人信息验证失败",QMessageBox::Yes,this);
        msgBox->show();
        QTimer::singleShot(1000,msgBox,SLOT(close()));
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
}

