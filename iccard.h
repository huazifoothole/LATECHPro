#ifndef ICCARD_H
#define ICCARD_H

#include <QDialog>
#include"iccardlib.h"

namespace Ui {
    class ICcard;
}

class ICcard : public QDialog
{
    Q_OBJECT

public:
    explicit ICcard(QWidget *parent = 0);
    ~ICcard();
   bool initICcard();

private slots:
    void on_rechargeButton_clicked();

    void on_queryButton_clicked();

private:
    Ui::ICcard *ui;
    ICcardLib *iclib;
    int ICdev;
};

#endif // ICCARD_H
