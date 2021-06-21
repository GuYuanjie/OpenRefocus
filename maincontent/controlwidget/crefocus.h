#ifndef CREFOCUS_H
#define CREFOCUS_H

#include <opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>



using namespace std;
using namespace cv;

class CRefocus
{
public:
    CRefocus(Mat &srcImg, string camDataPath, int radius, float alphaMin, float alphaMax, int imgH, int imgW)
    {
        this->cameraDataPath=camDataPath;
        this->lightFieldImg=srcImg.clone();
        this->radius=radius;
        this->diameter=2*radius+1;
        this->alphaMin=alphaMin;
        this->alphaMax=alphaMax;
        this->imgHeight=imgH;
        this->imgWidth=imgW;
        this->depthResolution=8;

        isFirst=true;
        pLFImg=NULL;
        pRefocusImg=NULL;
        pRefocusLFImg=NULL;
    }
    ~CRefocus(){}

private:
    string cameraDataPath;
    Mat lightFieldImg;
    int imgHeight;
    int imgWidth;
    int imgHeightLF;
    int imgWidthLF;
    int radius;
    int diameter;
    int depthResolution;
    float alphaMin;
    float alphaMax;
    vector<Mat> refocusImgList;
    bool isFirst;
    uchar *pLFImg, *pRefocusLFImg, *pRefocusImg;

private:

    int index(int n, int m);

    void getROIGrad(int x, int y, vector<float> &gradList);

    float computeGrad(Mat &img);

    int findBestImg(vector<float> &gradList);

public:

    //void cudaRefocus(Mat &LFRemapImg, Mat &refocusImgLF, Mat &refocusImg, float alpha);

    //void cudaRefocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha);

    void showRefocus(int x, int y);

    void readTxt(cv::Mat &mapx, cv::Mat &mapy);

    void raw2Remap(cv::Mat &imgLF, cv::Mat &mapx, cv::Mat &mapy, cv::Mat &LFRemapImg);

    void remap2Refocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha);

    int findBestImg(vector<float> &gradList);

    //vector<cv::Mat> run();
    virtual void run();
};

#endif // CREFOCUS_H





