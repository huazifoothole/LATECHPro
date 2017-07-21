#ifndef IMAGEPLAY_H
#define IMAGEPLAY_H

#include <QDialog>
#include <QTimer>
#include <QDebug>
#include <stdio.h>


namespace Ui {
    class ImagePlay;
}

class ImagePlay : public QDialog
{
    Q_OBJECT

public:
    explicit ImagePlay(const char *dir, int width, int height, QWidget *parent = 0);
    ~ImagePlay();

private:
    Ui::ImagePlay *ui;

    const char *dirName;
    char **imageList;

public slots:
    void imagePlay();

};

#endif // IMAGEPLAY_H
