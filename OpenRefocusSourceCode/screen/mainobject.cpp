/*****************************************
 * 作者: YYC
 * 日期: 2021-01-07
 * 功能：初始化类
 * ***************************************/
#include "mainobject.h"
#include "mainwindow.h"
#include "splashscreen.h"
#include <QDebug>


MainObject::MainObject(QObject *parent) :
    QObject(parent)
{
}

MainObject::~MainObject()
{
    delete m_mainWindow;
}

void MainObject::setInit()
{
    SplashScreen::getInstance()->setStagePercent(0,tr("Initializing the application ..."));

    m_mainWindow = new MainWindow(nullptr);
    m_mainWindow->setWindowTitle(tr("OpenRefocus"));
    SplashScreen::getInstance()->setStart(m_mainWindow, tr("OpenRefocus"), QString(":/res/res/image/image.png"));
    SplashScreen::getInstance()->setStagePercent(40, tr("Initializing the main interface ..."));
    SplashScreen::getInstance()->setStagePercent(80, tr("Loading the interface ..."));
    SplashScreen::getInstance()->setStagePercent(100, tr("Loading completed!"));
    SplashScreen::getInstance()->setFinish();

    m_mainWindow->showNormal();
}


