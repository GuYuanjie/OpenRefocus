#include "loadpos.h"
#include "ui_loadpos.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QFile>
#include <iostream>
//#include <globaldef.hpp>
QString dataPath;


LoadPos::LoadPos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadPos)
{
    ui->setupUi(this);
}

LoadPos::~LoadPos()
{
    delete ui;
}
bool LoadPos::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    sourceDir.replace("\\","/");
    toDir.replace("\\","/");

    QString curDir = QDir::currentPath();

    QDir* dir = new QDir();
    if(!dir->exists(curDir+"/cameraData")){
        dir->mkpath(curDir+"/cameraData");
    }

    toDir = curDir + toDir;
    dataPath = curDir+"/cameraData";

    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        QMessageBox::warning(this,tr("error"),tr("file's dir error！"));
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
        QMessageBox::warning(this,tr("error"),tr("copy file error!"));
        return false;

    }
    return true;
}

void LoadPos::on_btnLoad1_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File (*.txt)"));
    if(fileName == "")
    {
        return;
    }
    else
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("error"),tr("open file fialed!"));
            return;
        }
        else
        {
            if(!file.isReadable())
            {
                QMessageBox::warning(this,tr("error"),tr("read file fialed!"));
            }
            else
            {
                //Copy the txt which is choosed
                file.close();
                QString toDir = "/cameraData/datax.txt";
                QString sourceDir = fileName;
                bool setDataX = copyFileToPath(sourceDir,toDir,true);
                if(!setDataX) ui->btnLoad1->setText("Failed");
                else {
                    xdataState=true;
                    ui->btnLoad1->setText("Ready");
                }
            }
        }
    }
}

void LoadPos::on_btnLoad2_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File (*.txt)"));
    if(fileName == "")
    {
        return;
    }
    else
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,tr("error"),tr("open file fialed!"));
            return;
        }
        else
        {
            if(!file.isReadable())
            {
                QMessageBox::warning(this,tr("error"),tr("read file fialed!"));
            }
            else
            {

                file.close();
                QString toDir = "/cameraData/datay.txt";
                QString sourceDir = fileName;
                bool setDataX = copyFileToPath(sourceDir,toDir,true);
                if(!setDataX) ui->btnLoad2->setText("Failed");
                else {
                    ydataState=true;
                    ui->btnLoad2->setText("Ready");
                }
            }
        }
    }
}

void LoadPos::on_pushButton_3_clicked()
{
    if(xdataState&&ydataState){
        emit SigToPage(1);
    }
    else {
        QMessageBox message(QMessageBox::Warning,"Warning","<font size='5' color='white'>Please import data</font>", QMessageBox::Yes , NULL);
        message.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        message.setStyleSheet("QMessageBox { background-color:#1A1A1A; color:#1A1A1A; }");
        message.exec();
//        QMessageBox::warning(NULL, "Warning", "Please import data!", QMessageBox::Yes);
//        QMessageBox{Qt::black}
    }
}
