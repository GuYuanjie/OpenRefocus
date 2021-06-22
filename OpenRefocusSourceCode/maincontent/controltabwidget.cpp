
#include "controltabwidget.h"
#include "ui_controltabwidget.h"


ControlTabWidget::ControlTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlTabWidget)
{
    ui->setupUi(this);
    this->initValue();

    connect(ui->tabPos, SIGNAL(SigToPage(int)), this, SLOT(DealSigToPage(int)));
    connect(ui->tabImage, SIGNAL(SigToPage(int)), this, SLOT(DealSigToPage(int)));
    connect(ui->tabConfig, SIGNAL(SigToPage(int)), this, SLOT(DealSigToPage(int)));
    connect(ui->tabRefocus, SIGNAL(SigToPage(int)), this, SLOT(DealSigToPage(int)));
    connect(ui->tabSave, SIGNAL(SigToPage(int)), this, SLOT(DealSigToPage(int)));


    connect(ui->tabConfig, SIGNAL(SigParam(RefocusParam*)), ui->tabRefocus, SLOT(DealSigParam(RefocusParam*)));
}


ControlTabWidget::~ControlTabWidget()
{
    delete ui;
}

void ControlTabWidget::DealSigToPage(int index)
{
    receiveShowCurrentTab((WidgetTabType)index);
    emit sendShowIndex(index);
}


void ControlTabWidget::initValue()
{
    ui->tabWidgetControl->tabBar()->setHidden(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(30, 30, 30));
    ui->tabWidgetControl->setAutoFillBackground(true);
    ui->tabWidgetControl->setPalette(palette);
    ui->tabWidgetControl->tabBar()->setPalette(palette);

    //new add
    {
        TabWidgetData tabWidgetData;
        tabWidgetData.currentWidget = ui->tabPos;
        tabWidgetData.currentTabText = ui->tabWidgetControl->tabText(TAB_POS);
        mapTabWidget[TAB_POS] = tabWidgetData;
    }
    {
        TabWidgetData tabWidgetData;
        tabWidgetData.currentWidget = ui->tabImage;
        tabWidgetData.currentTabText = ui->tabWidgetControl->tabText(TAB_IMAGE);
        mapTabWidget[TAB_IMAGE] = tabWidgetData;
    }
    {
        TabWidgetData tabWidgetData;
        tabWidgetData.currentWidget = ui->tabConfig;
        tabWidgetData.currentTabText = ui->tabWidgetControl->tabText(TAB_CONFIG);
        mapTabWidget[TAB_CONFIG] = tabWidgetData;
    }
    {
        TabWidgetData tabWidgetData;
        tabWidgetData.currentWidget = ui->tabRefocus;
        tabWidgetData.currentTabText = ui->tabWidgetControl->tabText(TAB_REFOCUS);
        mapTabWidget[TAB_REFOCUS] = tabWidgetData;
    }
    {
        TabWidgetData tabWidgetData;
        tabWidgetData.currentWidget = ui->tabSave;
        tabWidgetData.currentTabText = ui->tabWidgetControl->tabText(TAB_SAVE);
        mapTabWidget[TAB_SAVE] = tabWidgetData;
    }

    ui->tabWidgetControl->setCurrentIndex(0);
}


void ControlTabWidget::receiveShowCurrentTab(WidgetTabType widgetTabType)
{
    if(widgetTabType >= TAB_MAX) return;
    TabWidgetData tabWidgetData = mapTabWidget.value(widgetTabType);
    if (ui->tabWidgetControl->indexOf(tabWidgetData.currentWidget) < 0) {
        ui->tabWidgetControl->addTab(tabWidgetData.currentWidget, tabWidgetData.currentTabText);
    }
    ui->tabWidgetControl->setCurrentWidget(tabWidgetData.currentWidget);

    if(widgetTabType == TAB_SAVE)
    {
        ui->tabSave->LoadResult();
    }
}
