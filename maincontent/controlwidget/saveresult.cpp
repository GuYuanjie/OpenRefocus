#include "saveresult.h"
#include "ui_saveresult.h"
#include <QImage>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <opencv.hpp>

SaveResult::SaveResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveResult)
{
    ui->setupUi(this);

}

SaveResult::~SaveResult()
{
    delete ui;
}

void SaveResult::mousePressEvent(QMouseEvent *ev)
{
    vector<float> gradList;
    int x=ev->x();
    int y=ev->y();
    CRefocus::Instance()->GetROIGrad(x,y,gradList);
    int index= CRefocus::Instance()->GetIndex(gradList);
    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    if(!vcMat.empty())
    {
        QImage img(cvMat2QImage(vcMat[index]));
        CurrentImgIdx = index;
        ui->labelImage->setPixmap(QPixmap::fromImage(img));
    }
    ui->horizontalSlider->setValue(index-1);
    return QWidget::mousePressEvent(ev);
}

void SaveResult::LoadResult()
{
    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    if(!vcMat.empty())
    {
        QImage img(cvMat2QImage(vcMat[0]));
        ui->labelImage->setPixmap(QPixmap::fromImage(img.scaled(ui->labelImage->size())));
        ui->labelImage->setScaledContents(true);
    }

//    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(ValueChanged(int)));
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(CRefocus::Instance()->getDepthResolutionParam()-1);
    ui->horizontalSlider->setSingleStep(1);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_verticalSlider_slidered()));
    ui->horizontalSlider->setTickPosition(QSlider::TicksAbove);

}

//cv::Mat iniImg=refocusImgList[0].clone();
//cv::namedWindow("refocus", 1);
//imshow("refocus", iniImg);
//DISPLAY display;
//initParam(display);
//cv::setMouseCallback("refocus", on_MouseHandle, &display);



QImage SaveResult::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}




void SaveResult::on_btnSavfeCur_clicked()
{

    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    QString filename= QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.jpg)")); //选择路径
    string fileAsSave = filename.toStdString();
    if(!filename.isEmpty()){
        imwrite(fileAsSave,vcMat[CurrentImgIdx]);
    }

}

string intToString(int v)
{
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%u", v);

    string str = buf;
    return str;
}

void SaveResult::on_btnSaveAll_clicked()
{

    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.jpg)")); //选择路径
    QString imgnum,curfileName;
    string fileAsSave;
    for(int i=0;i<CRefocus::Instance()->getDepthResolutionParam();i++){
        curfileName=filename;
        imgnum= QString::number(i+1);
        fileAsSave=curfileName.replace(".jpg","_"+imgnum+".jpg").toStdString();
        if(!filename.isEmpty()){
            imwrite(fileAsSave, vcMat[i]);
        }

    }
}

void SaveResult::on_btnFirstPage_clicked()
{
    qApp->exit(RETCODE_RESTART);
//    emit SigToPage(0);
}

void SaveResult::on_btnDepthPic_clicked()
{
    vector<cv::Mat> vcMat;
    cv::Mat CurDepthPic,ColorDepthPic;
    QString CurfileName = QCoreApplication::applicationDirPath();
    CRefocus::Instance()->GetResultMat(vcMat);
    CRefocus::Instance()->getDepthEstimation();
    CurDepthPic=cv::imread(CurfileName.toStdString()+"/MaxGrad.jpg");
    cv::applyColorMap(CurDepthPic, ColorDepthPic, cv::COLORMAP_OCEAN);
    if(!CurDepthPic.empty())
    {
        QImage img(cvMat2QImage(ColorDepthPic));
        ui->labelImage->setPixmap(QPixmap::fromImage(img));
    }

}

void SaveResult::on_btnGrad_clicked()
{
    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    if(!vcMat.empty())
    {
       CRefocus::Instance()->getHighDetail(CurrentImgIdx);
    }

}

void SaveResult::on_btnHistogram_clicked()
{
    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    CRefocus::Instance()->show_histogram(vcMat[CurrentImgIdx]);
}
void SaveResult::ValueChanged(int val)
{
//    qDebug() << "valueChanged" << val;
}
void SaveResult::on_verticalSlider_slidered()
{
    int curIndex = ui->horizontalSlider->value();
    vector<cv::Mat> vcMat;
    CRefocus::Instance()->GetResultMat(vcMat);
    if(!vcMat.empty())
    {
        QImage img(cvMat2QImage(vcMat[curIndex]));
        ui->labelImage->setPixmap(QPixmap::fromImage(img));
    }
}
