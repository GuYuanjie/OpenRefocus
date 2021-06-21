

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initValue();
    SetStatusText("Hello，welcome to OpenRefocus!", 360000);
    ui->statusBar->setStyleSheet("color:white;");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetStatusText(const QString &strText, int iTime)
{
    ui->statusBar->showMessage(strText, iTime);
}

void MainWindow::initValue()
{

    syetemTray = new SystemTray(this);
    connect(syetemTray->getShowWidget(), SIGNAL(triggered(bool)), this, SLOT(showNormal()));
    connect(syetemTray->getMinWidget(), SIGNAL(triggered(bool)), this, SLOT(showMinimized()));
    ui->widgetTitle->setParentWidget(this);
    this->setWindowFlags(Qt::FramelessWindowHint);


    connect(ui->widgetTree, SIGNAL(sendShowIndex(WidgetTabType)), ui->widgetTab, SLOT(receiveShowCurrentTab(WidgetTabType)));
    connect(ui->widgetTab, SIGNAL(sendShowIndex(int)), ui->widgetTree, SLOT(DealSendShowIndex(int)));


}

void MainWindow::SetCurrentTab(int index)
{
    ui->widgetTree->setCurrentIndex(index);
}

