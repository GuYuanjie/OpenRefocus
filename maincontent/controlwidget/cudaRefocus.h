#ifndef CUDAREFOCUS_H
#define CUDAREFOCUS_H

#include <opencv.hpp>
#include <string>
using namespace cv;

struct cudaErr{
    const char *cudaError;
    int errNum;
};

void cudaRefocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha,
                 bool &isFirst, int imgHeightLF, int imgWidthLF, int imgHeight, int imgWidth,
                 int radius, uchar *pLFImg, uchar *pRefocusLFImg, uchar *pRefocusImg,cudaErr * Err);

void myprint(uint64_t* avail, uint64_t* total );

#endif // CUDAREFOCUS_H
