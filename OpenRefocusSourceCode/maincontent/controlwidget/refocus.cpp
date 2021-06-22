#include "refocus.h"
//#include <globaldef.hpp>
#include "ui_refocus.h"
#include <opencv.hpp>
#include <string>
#include <QMessageBox>
#include <windows.h>
#include <QDebug>
#include "cudaRefocus.h"




using namespace cv;
using namespace std;

extern QString imageRemovePath;
extern QString dataPath;



CRefocus* CRefocus::m_Instance = nullptr;
void CRefocus::readTxt(cv::Mat &mapx, cv::Mat &mapy)
{
    string fileX = cameraDataPath + "datax.txt";
    string fileY = cameraDataPath + "datay.txt";
    ifstream infileX, infileY;
    infileX.open(fileX.data());
    if(!infileX.is_open()) qDebug() << "Datax.txt is error!"<<"\n";
    else qDebug() << "DataX is OK!\n"<<"\n";
    infileY.open(fileY.data());
    if(!infileY.is_open()) qDebug() << "Datay.txt is error!"<<"\n";
    else qDebug() << "DataY is OK!\n"<<"\n";
    for (int j = 0; j < imgHeight; j++)
    {
        for (int i = 0; i< imgWidth; i++)
        {
            int valueX, valueY;
            infileX >> valueX;
            infileY >> valueY;
            mapx.at<float>(j, i) = valueX;
            mapy.at<float>(j, i) = valueY;
        }
    }
    infileX.close();
    infileY.close();
}

void CRefocus::raw2Remap(cv::Mat &LFImg, cv::Mat &mapx, cv::Mat &mapy, cv::Mat &LFRemapImg)
{
    int sizex=imgWidth;
    int sizey=imgHeight;
    int uvRadius=radius;
    int uvDiameter=diameter;
    for (int i = 4; i < (sizex-4); i++)
    {
        for (int j = 4; j < (sizey-4); j++)
        {
            int cordCenterX = (int)mapx.at<float>(j, i);
            int cordCenterY = (int)mapy.at<float>(j, i);

            for (int adjustX = -uvRadius; adjustX <= uvRadius; adjustX++)
            {
                for (int adjustY = -uvRadius; adjustY <= uvRadius; adjustY++)
                {

                    int indexInputX = cordCenterX - 1 + adjustX;
                    int indexInputY = cordCenterY - 1 + adjustY;


                    int indexOutputX = adjustX + uvRadius  + i * uvDiameter;
                    int indexOutputY = adjustY + uvRadius  + j * uvDiameter;


                    LFRemapImg.at<uchar>(indexOutputY, indexOutputX * 3 + 0)= LFImg.at<uchar>(indexInputY, indexInputX * 3 + 0);
                    LFRemapImg.at<uchar>(indexOutputY, indexOutputX * 3 + 1)= LFImg.at<uchar>(indexInputY, indexInputX * 3 + 1);
                    LFRemapImg.at<uchar>(indexOutputY, indexOutputX * 3 + 2)= LFImg.at<uchar>(indexInputY, indexInputX * 3 + 2);
                }
            }
        }
    }
}

CRefocus *CRefocus::Instance()
{
    if(m_Instance == nullptr)
    {
        m_Instance = new CRefocus();
    }
    return m_Instance;
}

CRefocus::CRefocus(QObject *parent)
    :QThread(parent)
{

}

void CRefocus::SetParam(Mat &srcImg, string camDataPath, int radius, float alphaMin, float alphaMax, int imgH, int imgW,int DepthResolution)
{
    this->cameraDataPath=camDataPath;
    this->lightFieldImg=srcImg.clone();
    this->radius=radius;
    this->diameter=2*radius+1;
    this->alphaMin=alphaMin;
    this->alphaMax=alphaMax;
    this->imgHeight=imgH;
    this->imgWidth=imgW;
    this->depthResolution=DepthResolution;

    isFirst=true;
    pLFImg=NULL;
    pRefocusImg=NULL;
    pRefocusLFImg=NULL;
}



void CRefocus::GetResultMat(vector<Mat> &vcMat)
{
    vcMat.assign(refocusImgList.begin(), refocusImgList.end());
}
void CRefocus::GetROIGrad(int x,int y,vector<float> &gradList)
{
    getROIGrad(x,y,gradList);
}
int CRefocus::GetIndex(vector<float> &gradList)
{
    int index = findBestImg(gradList);
    return index;
}
int CRefocus:: index(int n, int m)
{
    if (0 <= n && n < m)
        return n;
    else if (n < 0)
        return 0;
    else
        return m - 1;
}

void CRefocus::remap2Refocus(cv::Mat &remapImg, cv::Mat& remapAlphaImg, cv::Mat &refocAlphaImg, float alpha)
{
    uchar* im_in_remap = remapImg.data;
    uchar* im_out_remap = remapAlphaImg.data;
    uchar* output_image = refocAlphaImg.data;
    int width = imgWidth;
    int height =imgHeight;
    int window_side = diameter;
    int stereo_diff = radius;

    unsigned int        x_1, x_2, y_1, y_2;
    int                 i, j;
    double              x_ind, y_ind;
    double              x_floor, y_floor;
    double              x_1_w, x_2_w, y_1_w, y_2_w;
    unsigned int        x_1_index, x_2_index, y_1_index, y_2_index;
    unsigned int        x_index_remap, y_index_remap;
    double              interp_color_R, interp_color_G, interp_color_B;
    double              output_color_R, output_color_G, output_color_B;
    unsigned int        height_of_remap, width_of_remap, pixels_of_remap;
    int                 window_size;

    window_size = window_side*window_side;

    height_of_remap = height*window_side;
    width_of_remap = width*window_side;
    pixels_of_remap = height_of_remap*width_of_remap;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output_color_B = 0;
            output_color_G = 0;
            output_color_R = 0;

            for (j = -stereo_diff; j < stereo_diff + 1; j++)
            {
                for (i = -stereo_diff; i < stereo_diff + 1; i++)
                {
                    x_ind = i*(1 - 1 / alpha) + x;
                    y_ind = j*(1 - 1 / alpha) + y;

                    x_floor = floor(x_ind);
                    y_floor = floor(y_ind);

                    x_1 = index(x_floor, width);
                    y_1 = index(y_floor, height);
                    x_2 = index(x_floor + 1, width);
                    y_2 = index(y_floor + 1, height);

                    x_1_w = 1 - (x_ind - x_floor);
                    x_2_w = 1 - x_1_w;
                    y_1_w = 1 - (y_ind - y_floor);
                    y_2_w = 1 - y_1_w;


                    x_1_index = i + stereo_diff + (x_1) * window_side;
                    y_1_index = j + stereo_diff + (y_1) * window_side;
                    x_2_index = i + stereo_diff + (x_2) * window_side;
                    y_2_index = j + stereo_diff + (y_2) * window_side;


                    interp_color_B = y_1_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_1_index * 3 + 0 ] +
                            y_2_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_2_index * 3 + 0 ] +
                            y_1_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_1_index * 3 + 0 ] +
                            y_2_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_2_index * 3 + 0 ];

                    interp_color_G = y_1_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_1_index * 3 + 1] +
                            y_2_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_2_index * 3 + 1] +
                            y_1_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_1_index * 3 + 1] +
                            y_2_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_2_index * 3 + 1];

                    interp_color_R = y_1_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_1_index * 3 + 2] +
                            y_2_w * x_1_w * im_in_remap[y_1_index * width_of_remap * 3 + x_2_index * 3 + 2] +
                            y_1_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_1_index * 3 + 2] +
                            y_2_w * x_2_w * im_in_remap[y_2_index * width_of_remap * 3 + x_2_index * 3 + 2];


                    x_index_remap = i + stereo_diff + (x) * window_side;
                    y_index_remap = j + stereo_diff + (y) * window_side;

                    im_out_remap[x_index_remap * 3 + y_index_remap * width_of_remap * 3 + 0] = interp_color_B;
                    im_out_remap[x_index_remap * 3 + y_index_remap * width_of_remap * 3 + 1] = interp_color_G;
                    im_out_remap[x_index_remap * 3 + y_index_remap * width_of_remap * 3 + 2] = interp_color_R;


                    output_color_R = interp_color_R + output_color_R;
                    output_color_G = interp_color_G + output_color_G;
                    output_color_B = interp_color_B + output_color_B;
                }
            }

            output_image[x * 3 + y * width * 3 + 0] = output_color_B / window_size;
            output_image[x * 3 + y * width * 3 + 1] = output_color_G / window_size;
            output_image[x * 3 + y * width * 3 + 2] = output_color_R / window_size;
        }
    }
}

int CRefocus::findBestImg(vector<float> &gradList)
{
    float max = -1;
    int index=0;
    for (int i = 0; i < depthResolution; i++)
    {
        if (gradList[i]>max)
        {
            max = gradList[i];
            index = i;
        }
    }

    gradList.clear();
    return index;
}

void CRefocus::getDepthEstimation(){
    //get blur
    vector<cv::Mat> refocusGrad;
    int NormMax = 0.001;
    cv::Mat MaxGradIdx(refocusImgList[0].size(),CV_8UC1);
    cv::Mat MaxGrad(refocusImgList[0].size(),CV_8UC1);
    int maxGradValueIdx=0;
    float V=1,U=1,F=0.08;
    float alpha = 0;
    float alphaStep = (alphaMax-alphaMin)/depthResolution;

    for(int idx=0;idx<depthResolution;idx++){

        refocusGrad.push_back(cptSobelGradMat(refocusImgList[idx]));
        cv::blur(refocusGrad[idx],refocusGrad[idx],cv::Size(3,3));//get data after blur
    }

    for(int idxx=0;idxx<imgHeight;idxx++){
        for(int idxy=0;idxy<imgWidth;idxy++){
            for(int idx=0;idx<depthResolution;idx++){
                if(refocusGrad[idx].at<float>(idxx,idxy)>maxGradValueIdx) maxGradValueIdx=idx;
            }

            //            alpha = alphaMin + maxGradValueIdx*alphaStep;
            //            V = alpha*refocusGrad[maxGradValueIdx].at<float>(idxx,idxy);

            //            if((V-F)!=0) U = (F*V)/(V-F);
            //            else U =  (F*V)/1;


            MaxGradIdx.at<uchar>(idxx,idxy)=maxGradValueIdx;
            MaxGrad.at<uchar>(idxx,idxy)=refocusGrad[maxGradValueIdx].at<float>(idxx,idxy) ;

        }
    }


    //    double minv = 0.0, maxv = 0.0;
    //    double* minp = &minv;
    //    double* maxp = &maxv;

    //    cv::minMaxIdx(MaxGrad,minp,maxp);
    //    MaxGrad=(MaxGrad/maxv)*255;

    string PicDir =  CurfileName.toStdString();
    imwrite(PicDir+"/MaxGrad.jpg",MaxGrad);


}



void CRefocus::getHighDetail(int idx){

    cv::Mat showImg;
    cv::Mat showImg32;

    showImg=cptSobelGradMat(refocusImgList[idx]);

    imwrite(CurfileName.toStdString()+"/MaxGradIdx.jpg",showImg);
    showImg32=imread(CurfileName.toStdString()+"/MaxGradIdx.jpg");
    imshow("SobelGradient", showImg32);

}



cv::Mat  CRefocus::equalizeHist(cv::Mat& input){
    cv::Mat output = input.clone();
    gray_sum = input.cols * input.rows;

    //统计每个灰度下的像素个数
    for (int i = 0; i < input.rows; i++)
    {
        uchar* p = input.ptr<uchar>(i);
        for (int j = 0; j < input.cols; j++)
        {
            int vaule = p[j];
            gray[vaule]++;
        }
    }
    //统计灰度频率
    for (int i = 0; i < 256; i++)
    {
        gray_prob[i] = ((double)gray[i] / gray_sum);
    }
    //计算累计密度
    gray_distribution[0] = gray_prob[0];
    for (int i = 1; i < 256; i++)
    {
        gray_distribution[i] = gray_distribution[i-1] +gray_prob[i];
    }
    //重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
    for (int i = 0; i < 256; i++)
    {
        gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
    }
    //直方图均衡化,更新原图每个点的像素值
    for (int i = 0; i < output.rows; i++)
    {
        uchar* p = output.ptr<uchar>(i);
        for (int j = 0; j < output.cols; j++)
        {
            p[j] = gray_equal[p[j]];
        }
    }

    return output;
}

void CRefocus::show_histogram(Mat& img){
    //为计算直方图配置变量
    //首先是需要计算的图像的通道，就是需要计算图像的哪个通道（bgr空间需要确定计算 b或g货r空间）
    int channels = 0;
    //然后是配置输出的结果存储的 空间 ，用MatND类型来存储结果
    MatND dstHist;
    //接下来是直方图的每一个维度的 柱条的数目（就是将数值分组，共有多少组）
    int histSize[] = { 256 };       //如果这里写成int histSize = 256;   那么下面调用计算直方图的函数的时候，该变量需要写 &histSize
    //最后是确定每个维度的取值范围，就是横坐标的总数
    //首先得定义一个变量用来存储 单个维度的 数值的取值范围
    float midRanges[] = { 0, 256 };
    const float *ranges[] = { midRanges };

    calcHist(&img, 1, &channels, Mat(), dstHist, 1, histSize, ranges, true, false);

    //calcHist  函数调用结束后，dstHist变量中将储存了 直方图的信息  用dstHist的模版函数 at<Type>(i)得到第i个柱条的值
    //at<Type>(i, j)得到第i个并且第j个柱条的值

    //开始直观的显示直方图——绘制直方图
    //首先先创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像
    Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
    //因为任何一个图像的某个像素的总个数，都有可能会有很多，会超出所定义的图像的尺寸，针对这种情况，先对个数进行范围的限制
    //先用 minMaxLoc函数来得到计算直方图后的像素的最大个数
    double g_dHistMaxValue;
    minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
    //将像素的个数整合到 图像的最大范围内
    //遍历直方图得到的数据
    for (int i = 0; i < 256; i++)
    {
        int value = cvRound(dstHist.at<float>(i) * 256 * 0.9 / g_dHistMaxValue);

        line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(255, 255, 255));
    }

    imshow("Histogram", drawImage);
}

float CRefocus::getAverageColorValue(float b,float g,float r){
    return (b+g+r)/3;
}
int CRefocus::getDepthResolutionParam(){
    return depthResolution;
}


void CRefocus::gradient2D(cv::Mat input, cv::Mat& output){
    cv::Mat Ix(input.size(), CV_32F);
    cv::Mat Iy(input.size(), CV_32F);
    //get Iy
    for (int nrow = 0; nrow < input.rows; nrow++)
    {
        for (int ncol = 0; ncol < input.cols; ncol++)
        {
            if (ncol == 0)
            {
                Ix.at<float>(nrow, ncol) = abs(input.at<uchar>(nrow, 1) - input.at<uchar>(nrow, 0));
            }
            else if (ncol == input.cols - 1)
            {
                Ix.at<float>(nrow, ncol) = abs(input.at<uchar>(nrow, ncol) - input.at<uchar>(nrow, ncol - 1));
            }
            else
            {
                Ix.at<float>(nrow, ncol) = abs((input.at<uchar>(nrow, ncol + 1) - input.at<uchar>(nrow, ncol - 1)) / 2.0);
            }
        }
    }
    //get Ix
    for (int nrow = 0; nrow < input.rows; nrow++){
        for (int ncol = 0; ncol < input.cols; ncol++) {
            if (nrow == 0)
            {
                Iy.at<float>(nrow, ncol) = abs(input.at<uchar>(1, ncol) - input.at<uchar>(0, ncol));
            }
            else if (nrow == input.rows - 1)
            {
                Iy.at<float>(nrow, ncol) = abs(input.at<uchar>(nrow, ncol) - input.at<uchar>(nrow - 1, ncol));
            }
            else
            {
                Iy.at<float>(nrow, ncol) = abs((input.at<uchar>(nrow + 1, ncol) - input.at<uchar>(nrow - 1, ncol)) / 2.0);
            }
        }
    }
    cv::magnitude(Ix, Iy, output);
}

float CRefocus::computeGrad(cv::Mat &img)
{
    cv::Mat grayImg;
    cvtColor(img, grayImg, COLOR_BGR2GRAY);
    cv::Mat gradxImg, gradyImg;
    cv::Mat mag;

    Sobel(grayImg, gradxImg, CV_32F, 1, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    Sobel(grayImg, gradyImg, CV_32F, 0, 1, 1, 1, 0, cv::BORDER_DEFAULT);

    magnitude(gradxImg, gradyImg, mag);

    cv::Scalar s = sum(mag);
    float average = s.val[0] / (mag.cols * mag.rows);

    return average;
}

cv::Mat CRefocus::cptGradMat(cv::Mat &img)
{
    cv::Mat grayImg;
    cvtColor(img, grayImg, COLOR_BGR2GRAY);
    cv::Mat gradxImg, gradyImg;
    cv::Mat mag;

    Scharr(grayImg, gradxImg, CV_32F, 1, 0, 1, 1, 0);
    Scharr(grayImg, gradyImg, CV_32F, 0, 1, 1, 1, 0);

    magnitude(gradxImg, gradyImg, mag);

    return mag;
}

cv::Mat CRefocus::cptSobelGradMat(cv::Mat &img)
{
    cv::Mat grayImg;
    cvtColor(img, grayImg, COLOR_BGR2GRAY);
    cv::Mat gradxImg, gradyImg;
    cv::Mat mag;
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;

    Sobel(grayImg, gradxImg, CV_32F, 1, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    Sobel(grayImg, gradyImg, CV_32F, 0, 1, 1, 1, 0, cv::BORDER_DEFAULT);

    magnitude(gradxImg, gradyImg, mag);
    cv::minMaxIdx(mag,minp,maxp);
    mag=(mag/maxv)*255;

    return mag;
}

void CRefocus::getROIGrad(int x, int y, vector<float> &gradList)
{
    int winx=7;
    int winy=7;
    int offsetx=(winx-1)/2;
    int offsety=(winy-1)/2;
    cv::Rect ROI(x-offsetx, y-offsety, winx, winy);
    if ( (x-offsetx>=0) && (y-offsety>=0) && (x+offsetx<imgWidth) && (y+offsety<imgHeight))
    {
        for (int i = 0; i < depthResolution; i++)
        {
            cv::Mat srcImg = refocusImgList[i].clone();
            cv::Mat img = srcImg(ROI);
            float value = computeGrad(img);
            gradList.push_back(value);
        }
    }
    else
    {
        for (int i = 0; i < depthResolution; i++)
        {
            gradList.push_back(0);
        }
    }
}

void CRefocus::run()
{

    cv::Mat mapx(cv::Size(imgWidth, imgHeight), CV_32FC1);
    cv::Mat mapy(cv::Size(imgWidth, imgHeight), CV_32FC1);

    imgHeightLF=imgHeight*diameter;
    imgWidthLF=imgWidth*diameter;
    cv::Mat lightFieldRemapImg=cv::Mat::zeros(cv::Size(imgWidthLF, imgHeightLF), CV_8UC3);
    try{
        readTxt(mapx, mapy);
    }
    catch(exception txt){
        QString str="Datax/y.txt error, please check!";
        emit SigShowMessageBox(str);
        emit GoToPage(2);
        return;
    }

    try{
        raw2Remap(lightFieldImg, mapx, mapy, lightFieldRemapImg);
    }
    catch(exception remap){
        QString str="Params  error, please check!";
        emit SigShowMessageBox(str);
        emit GoToPage(2);
        return;
    }


    float alphaStep=(alphaMax-alphaMin)/depthResolution;
    cv::Mat refocusImg=cv::Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC3);
    cv::Mat refocusImgLF=cv::Mat::zeros(cv::Size(imgWidthLF, imgHeightLF), CV_8UC3);
    try{
        for(int i=0; i<depthResolution; i++)
        {

            float alpha = alphaMin+ i*alphaStep;

            if(CPUflag)  {
                remap2Refocus(lightFieldRemapImg, refocusImgLF, refocusImg, alpha);
                refocusImgList.push_back(refocusImg.clone());
            }
            else
            {
                cudaErr err;

                uint64_t avail;
                uint64_t total;



                cudaRefocus(lightFieldRemapImg, refocusImgLF, refocusImg, alpha, isFirst, imgHeightLF,\
                            imgWidthLF, imgHeight, imgWidth, radius, pLFImg, pRefocusLFImg, pRefocusImg, &err);

                myprint(&avail, &total);
                cout<<"avail:"<<avail<<" total:"<<total<<endl;
                if(err.errNum!=0){
                    QString str("Cuda error, please check !");
                    errorFlag=true;
                    emit SigShowMessageBox(str);
                    exec();
                }
                refocusImgList.push_back(refocusImg.clone());
            }
            qDebug() << "Get refocus images #"<< i <<"\n";
            char buf[20];
            sprintf(buf,"%d",i+1);
            string strNum(buf);
            string str("Get refocus images #");
            str += strNum;

            emit SigProcess(i, depthResolution, str);


        }
        qDebug() << "refocus processing done!" <<"\n";

        string str("refocus processing done!");
        //emit SigShowDebug(str);
        emit SigProcess(depthResolution, depthResolution, str);
        exec();
    }catch(exception refocus){
        QString str="Params  error, please check!";
        emit SigShowMessageBox(str);
        emit GoToPage(2);
    }

    return;


}
//-------------------------------------------------------------------
Refocus::Refocus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Refocus)
{

    ui->setupUi(this);
    memset(&m_param, 0, sizeof(RefocusParam));
    bool bRet = connect(CRefocus::Instance(), SIGNAL(SigProcess(int, int, string)), this, SLOT(DealSigProcess(int,int, string)), Qt::DirectConnection);
    connect(CRefocus::Instance(), SIGNAL(SigShowMessageBox(const QString)), this, SLOT(DealShowMessageBox(QString)));
    connect(CRefocus::Instance(), SIGNAL(GoToPage(int)), this, SLOT(DealGoToPage(int)));
    m_iMax = 1000000;
    m_iCur = 0;
    m_timer = new QTimer;
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(DealTimeout()));

}

Refocus::~Refocus()
{
    delete ui;
}

void Refocus::SetImage()
{

}
using namespace cv;
void Refocus::SetProcessValue(int iValue)
{
    ui->progressBar->setValue(iValue);
}

void Refocus::DealSigParam(RefocusParam *param)
{
    memcpy(&m_param, param, sizeof(RefocusParam));

    string imgPath ="";
    string camDataPath ="";
    QString curDir = QDir::currentPath();
    imgPath = curDir.toStdString()+"/imageData/image.jpg";
    camDataPath=curDir.toStdString()+"/cameraData/";
    cv::Mat srcImg=cv::imread(imgPath);

    int radius=m_param.radius;
    int imgH=m_param.imgH;
    int imgW=m_param.imgW;
    int depthresolution = m_param.DepthResolution;
    float alphaMin=m_param.alphaMin;
    float alphaMax=m_param.alphaMax;


    CRefocus::Instance()->SetParam(srcImg, camDataPath, radius, alphaMin, alphaMax , imgH, imgW,depthresolution);
    //vector<Mat> refocusImgList = refocus->run();
    CRefocus::Instance()->start();
    m_timer->start();

}

void Refocus::DealSigShowDebug(const string &info)
{
    m_mutexInfo.lock();
    lstInfo.append(QString(info.c_str()));
    m_mutexInfo.unlock();
    //ui->textEdit->append("ddddd");
    //ui->textEdit->append("\n");
}


void Refocus::DealGoToPage(int num){
    emit SigToPage(num);
}

void Refocus::DealSigProcess(int iCurrent, int iMax, string str)
{

    m_mutexInfo.lock();


    lstInfo.append(QString(str.c_str()));

    m_iCur = iCurrent;
    m_iMax = iMax;
    m_mutexInfo.unlock();
}

void Refocus::DealTimeout()
{
    ui->textEdit->clear();
    m_mutexInfo.lock();
    foreach (QString str, lstInfo) {
        ui->textEdit->append(str);
    }

    if(m_iCur == m_iMax)
    {

        m_timer->stop();

        emit SigToPage(4);
    }
    else
    {
        ui->progressBar->setValue(((double)(m_iCur+2)/(double)m_iMax) *100.0);
    }
    m_mutexInfo.unlock();
}

void Refocus::DealShowMessageBox(QString str)
{
    //qDebug() << "show messagebox";
    QMessageBox Cudamessage(QMessageBox::Information, "Error", str, QMessageBox::Ok);
    //    QMessageBox Cudamessage(QMessageBox::Warning,"CudaError","<font size='5' color='white'>CudaError: Out of memory</font>", QMessageBox::Yes , NULL);
    Cudamessage.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    Cudamessage.setStyleSheet("QMessageBox { background-color:#1A1A1A; color:#1A1A1A; }");
    Cudamessage.exec();
    CRefocus::Instance()->exit();
}
