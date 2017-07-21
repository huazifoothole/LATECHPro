#ifndef ZDYBRANDDIG_H
#define ZDYBRANDDIG_H

#include <QDialog>

namespace Ui {
    class zdyBrandDig;
}

class zdyBrandDig : public QDialog
{
    Q_OBJECT

public:
    explicit zdyBrandDig(QWidget *parent = 0);
    ~zdyBrandDig();

    void posChoice(QString *);
    void getZdyStr(QString *,int);

signals:
   void getZdyInfoFinish(QString *);
   void zdyTextChange_v(int);
   void zdyTextChange_ct(int);
   void zdyTextChange_botm(int);
   void posChoice_(QString);


private slots:
   void setZdyType();
   void emitTextChge_ct();
   void emitTextChge_v();
   void emitTextChge_botm();
   void closeDig();
   void posChoice();



private:
    Ui::zdyBrandDig *ui;
};

#endif // ZDYBRANDDIG_H
