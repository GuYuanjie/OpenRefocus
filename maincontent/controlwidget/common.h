#ifndef COMMON_H
#define COMMON_H

extern bool CPUflag;
extern bool imageflag;

typedef struct RefocusParam
{
    int radius;
    int imgH;
    int imgW;
    float alphaMin;
    float alphaMax;
    int DepthResolution;
}RefocusParam;


#endif // COMMON_H
