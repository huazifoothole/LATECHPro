#include "imageplay.h"
#include "ui_imageplay.h"

#include <dirent.h>

static int imageFilter(const struct dirent *dir)
{
    if (NULL != strstr(dir->d_name, ".jpg") ||
        NULL != strstr(dir->d_name, ".png") ||
        NULL != strstr(dir->d_name, ".jpeg"))
        return 1;
    else
        return 0;
}

static int abcSort(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

static char** readDir(const char *path)
{
    char **fileNames = NULL;
    int i, j, total;
    struct dirent **imageList;

    total = scandir(path, &imageList , imageFilter, abcSort);
    for (i=0, j=0; i<total; )
    {
        fileNames = (char**)realloc(fileNames,sizeof(char*)*(j+1));
        fileNames[j++] =  strdup(imageList[i++]->d_name);
    }

    fileNames = (char **)realloc(fileNames,sizeof(char*)*(j+1));
    fileNames[j] = NULL;

    return fileNames;
}

static void destroyDir(char **fileNames)
{
        int i;
        for (i=0; fileNames[i] != NULL; i++)
        {
                free(fileNames[i]);
        }
        free(fileNames);
}



ImagePlay::ImagePlay(const char *dir, int width, int height, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImagePlay)
{
    ui->setupUi(this);
    //setWindowTitle("乐得瑞体彩终端多屏演示");
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    setGeometry(0, 0, width, height);
    ui->imageLabel->setGeometry(0, 0, width, height);
    ui->imageLabel->setScaledContents(true);

    dirName = dir;
    imageList = readDir(dirName);
    imagePlay();

    QTimer *timer = new QTimer(this);
    timer->start(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(imagePlay()));

}

ImagePlay::~ImagePlay()
{
    destroyDir(imageList);
    delete ui;
}

void ImagePlay::imagePlay()
{
    static int index = 0;
    char imagePath[128] = {0};

    if (NULL == imageList[0])
    {
        return;
    }
    else
    {
        sprintf(imagePath, "%s/%s", dirName, imageList[index++]);
        QPixmap image;
        image.load(imagePath);

        ui->imageLabel->setPixmap(image);
        update();

        if (NULL == imageList[index])
        {
            index = 0;
        }
    }
}
