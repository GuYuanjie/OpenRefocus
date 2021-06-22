#include "display.h"


void on_MouseHandle(int event, int x, int y, int flags, void *param)
{
    DISPLAY *display= (DISPLAY*) param;
    switch (event)
    {

    case cv::EVENT_LBUTTONDOWN:
    {
        display->leftButtonDown=true;
        display->clickX=x;
        display->clickY=y;
        break;
    }
    case cv::EVENT_LBUTTONUP:
    {
        display->leftButtonDown=false;
        break;
    }
    default:
        break;
    }
}


void initParam(DISPLAY &display)
{
    display.clickX=0;
    display.clickY=0;
    display.leftButtonDown=false;
}
