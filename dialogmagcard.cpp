#include "dialogmagcard.h"
#include "ui_dialogmagcard.h"

Dialogmagcard::Dialogmagcard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogmagcard)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    move(662,63);
    ui->lineEdit_tip->setPlaceholderText("请刷卡");
    ui->lineEdit_tip->setFocus();
}

Dialogmagcard::~Dialogmagcard()
{
    delete ui;
}

void Dialogmagcard::on_lineEdit_tip_textChanged(const QString &arg1)
{
    if(arg1.compare(";18423651524?")==0)
    {
        sleep(0.4);
        ui->lineEdit_tip->clear();
        ui->label_show->setText("信息验证成功");
        showinfo();
    }
}

void Dialogmagcard::showinfo()
{
    ui->lineEdit->setText("李益阳");
    ui->lineEdit_2->setText("18423651524");
    ui->lineEdit_3->setText("450221200004119021");
    ui->lineEdit_4->setText("93917313");
    ui->lineEdit_5->setText("2000");
    ui->lineEdit_6->setText("100");
    ui->lineEdit_7->setText("150");
}
