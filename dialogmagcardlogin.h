#ifndef DIALOGMAGCARDLOGIN_H
#define DIALOGMAGCARDLOGIN_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
    class DialogMagcardLogin;
}

class DialogMagcardLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMagcardLogin(QWidget *parent = 0);
    ~DialogMagcardLogin();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButtonEnsure_clicked();


private:
    Ui::DialogMagcardLogin *ui;
protected:
      void keyPressEvent(QKeyEvent *);
};

#endif // DIALOGMAGCARDLOGIN_H
