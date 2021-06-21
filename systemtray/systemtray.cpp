/*****************************************
 * 作者: YYC
 * 日期: 2020-04-26
 * 功能：系统托盘，未进行优化，只有最简单的
 * 功能，后续会继续优化
 * ***************************************/
#include "systemtray.h"
#include "globaldef.hpp"
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QDesktopServices>


SystemTray::SystemTray(QWidget * parent)
    :QSystemTrayIcon(parent)
{
    this->createAction();
    this->addAction();
}


SystemTray::~SystemTray()
{

}

void SystemTray::addAction()
{
    myMenu->addAction(showWidget);
    myMenu->addAction(minWidget);
    myMenu->addAction(aboutSoftWare);
    myMenu->addAction(exitSoftWare);
}


void SystemTray::createAction()
{
    myMenu = new QMenu();
    showWidget = new QAction(tr("Display the window"), this);
    minWidget = new QAction(tr("Hide the window"), this);
    aboutSoftWare = new QAction(tr("About the application"), this);
    exitSoftWare = new QAction(tr("Quit the application"), this);

    this->setIcon(QIcon(GlobalSpace::LOGO_PATH));
    this->setToolTip(tr("OpenRefocus"));

    connect(exitSoftWare, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    connect(aboutSoftWare, SIGNAL(triggered(bool)), this, SLOT(aboutSlot()));

    this->setContextMenu(myMenu);
    this->show();
}


QAction *SystemTray::getAboutSoftWare() const
{
    return aboutSoftWare;
}


void SystemTray::aboutSlot()
{
    QDesktopServices::openUrl(QUrl("https://www.baidu.com"));
}


QAction *SystemTray::getMinWidget() const
{
    return minWidget;
}


QAction *SystemTray::getShowWidget() const
{
    return showWidget;
}
