#ifndef DIALOGMAGCARD_H
#define DIALOGMAGCARD_H

#include <QDialog>

namespace Ui {
    class Dialogmagcard;
}

class Dialogmagcard : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogmagcard(QWidget *parent = 0);
    ~Dialogmagcard();

private slots:
    void on_lineEdit_tip_textChanged(const QString &arg1);

private:
    Ui::Dialogmagcard *ui;
    void showinfo();
};

#endif // DIALOGMAGCARD_H
