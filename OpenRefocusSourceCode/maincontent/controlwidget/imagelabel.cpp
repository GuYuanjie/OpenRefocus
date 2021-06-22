#include "imagelabel.h"
#include "common.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>



ImageLabel::ImageLabel(QWidget *parent)
    :QLabel(parent)
{

}
ImageLabel::~ImageLabel(){
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(imageRemovePath);
    if (exist){
            createfile->remove(imageRemovePath);
    }
}
bool ImageLabel::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    sourceDir.replace("\\","/");
    toDir.replace("\\","/");

    QString curDir = QDir::currentPath();

    QDir* dir = new QDir();
    if(!dir->exists(curDir+"/imageData")){
        dir->mkpath(curDir+"/imageData");
    }

    toDir = curDir + toDir;
   imageRemovePath = toDir;
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        QMessageBox::warning(this,tr("error"),tr("File's dir fialed"));
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if
    if(!QFile::copy(sourceDir, toDir))
    {
        QMessageBox::warning(this,tr("error"),tr("Copy file fialed"));
        return false;

    }
    //QMessageBox::warning(this,tr("SUCCESS"),tr("Ready!"));
    return true;
}

void ImageLabel::mousePressEvent(QMouseEvent *ev)
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Images ( *.bmp *.jpg)"));
    if(fileName == "")
    {
        return;
    }
    else
    {
        QImage img(fileName);
        this->setPixmap(QPixmap(QPixmap::fromImage(img.scaled(this->size()))));

        QFileInfo fileinfo;
        fileinfo = QFileInfo(fileName);
        QString toDir = "/imageData/image.jpg";
        QString sourceDir = fileName;
        imageflag = copyFileToPath(sourceDir,toDir,true);

    }
    return QWidget::mousePressEvent(ev);
}

