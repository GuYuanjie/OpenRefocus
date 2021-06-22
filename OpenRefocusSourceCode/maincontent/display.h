#ifndef DISPLAY_H
#define DISPLAY_H


#include "opencv.hpp"
#include <iostream>

using namespace std;
//using namespace cv;

typedef struct display
{
    int clickX;
    int clickY;
    bool leftButtonDown;
} DISPLAY;

void on_MouseHandle(int event, int x, int y, int flags, void *param);
void initParam(DISPLAY &display);

#endif // DISPLAY_H
