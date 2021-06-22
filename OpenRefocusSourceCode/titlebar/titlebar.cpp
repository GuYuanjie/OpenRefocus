/*****************************************
 * 作者: YYC
 * 日期: 2020-04-26
 * 功能：封装了鼠标的一系列事件，用于鼠标移动窗口
 * 只需继承这个类，就能得到鼠标移动窗口的功能
 * ***************************************/
#include "titlebar.h"
#include <QDebug>


TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    this->parentWidget = parent;
}


void TitleBar::setParentWidget(QWidget *parentWidget)
{
    this->parentWidget = parentWidget;
}


void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    if(parentWidget)
    {
        movePoint = event->globalPos() - parentWidget->pos();
    }
}


void TitleBar::mouseReleaseEvent(QMouseEvent *)
{
    mousePress = false;
}


void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress && parentWidget)
    {
        QPoint movePos = event->globalPos();
        parentWidget->move(movePos - movePoint);
    }
}
