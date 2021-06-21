
#ifndef REFOCUS_H
#define REFOCUS_H

#include <QWidget>
#include <QString>
#include <opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <QMutex>
#include <QTimer>
#include <QCoreApplication>
#include <QThread>
#include <QDir>
#include "../display.h"
#include "common.h"
using namespace std;

namespace Ui {
class Refocus;
}

class CRefocus;
class Refocus : public QWidget
{
    Q_OBJECT

public:
    explicit Refocus(QWidget *parent = nullptr);
    ~Refocus();

    void SetImage();
    void SetProcessValue(int iValue);
signals:
    void SigToPage(int);
public slots:
    void DealSigParam(RefocusParam *param);
    void DealSigShowDebug(const string &info);
    void DealSigProcess(int iCurrent, int iMax, string str);
    void DealTimeout();
    void DealShowMessageBox(QString str);
    void DealGoToPage(int num);
private:
    Ui::Refocus *ui;
    RefocusParam m_param;
    QStringList lstInfo;
    QMutex m_mutexInfo;
    QTimer *m_timer;
    int m_iCur;
    int m_iMax;
};



class CRefocus: public QThread
{
    Q_OBJECT
public:
    static CRefocus *Instance();
    CRefocus(QObject *parent = nullptr);
    void SetParam(cv::Mat &srcImg, string camDataPath, int radius, float alphaMin, float alphaMax, int imgH, int imgW,int DepthResolution);

    void GetResultMat(vector<cv::Mat> &vcMat);
    void GetROIGrad(int x,int y,vector<float> &gradList);
    void SetCudaParam(cv::Mat lightFieldImg,int imgHeight,int imgWidth,int imgHeightLF,int imgWidthLF,int depthResolution);
    int GetIndex(vector<float> &gradList);
    ~CRefocus(){}
signals:
    void SigShowDebug(const string &info);
    void SigProcess(int, int, string info);
    void SigShowMessageBox(const QString &info);
    void GoToPage(int num);

private:
    static CRefocus *m_Instance;
    string cameraDataPath;
    cv::Mat lightFieldImg;
    int imgHeight;
    int imgWidth;
    int imgHeightLF;
    int imgWidthLF;
    int radius;
    int diameter;
    int depthResolution;
    float alphaMin;
    float alphaMax;
    vector<cv::Mat> refocusImgList;
    vector<cv::Mat> refocusImg2Grad;
    bool isFirst;
    bool errorFlag=false;
    uchar *pLFImg, *pRefocusLFImg, *pRefocusImg;

    int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
    double gray_prob[256] = { 0 };  //记录灰度分布密度
    double gray_distribution[256] = { 0 };  //记录累计密度
    int gray_equal[256] = { 0 };  //均衡化后的灰度值
    int gray_sum = 0;  //像素总数

public:
    QString CurfileName = QCoreApplication::applicationDirPath();
    int getDepthResolutionParam();





private:

    int index(int n, int m);

    void getROIGrad(int x, int y, vector<float> &gradList);

    float computeGrad(cv::Mat &img);


public:

    //void cudaRefocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha);

    void showRefocus(int x, int y);
    void readTxt(cv::Mat &mapx, cv::Mat &mapy);
    void raw2Remap(cv::Mat &imgLF, cv::Mat &mapx, cv::Mat &mapy, cv::Mat &LFRemapImg);
    void remap2Refocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha);
    int findBestImg(vector<float> &gradList);
    void gradient2D(cv::Mat input, cv::Mat& output);
    void getDepthEstimation();
    void getHighDetail(int idx);
    float getAverageColorValue(float b,float g,float r);
    cv::Mat cptGradMat(cv::Mat &img);
    cv::Mat cptSobelGradMat(cv::Mat &img);
    cv::Mat equalizeHist(cv::Mat& input);
    void show_histogram(cv::Mat& img);
    virtual void run();

};



#endif // REFOCUS_H
