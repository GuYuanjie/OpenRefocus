#include "loadimagewgt.h"
#include "ui_loadimage.h"
#include "common.h"
#include <QMessageBox>


LoadImageWgt::LoadImageWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadImageWgt)
{
    ui->setupUi(this);
}

LoadImageWgt::~LoadImageWgt()
{
    delete ui;
}

void LoadImageWgt::on_btnNextPage_clicked()
{
    if(imageflag)emit SigToPage(2);
    else {
        QMessageBox message(QMessageBox::Warning,"Warning","<font size='5' color='white'>Please import image</font>", QMessageBox::Yes , NULL);
        message.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        message.setStyleSheet("QMessageBox { background-color:#1A1A1A; color:#1A1A1A; }");
        message.exec();
    }

}
