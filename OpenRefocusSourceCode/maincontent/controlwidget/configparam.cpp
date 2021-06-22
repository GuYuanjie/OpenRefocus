#include "configparam.h"
#include "ui_configparam.h"
#include "maincontent/controlwidget/refocus.h"
#include <QMessageBox>

ConfigParam::ConfigParam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigParam)
{
    ui->setupUi(this);
    m_RefocusParam = new RefocusParam;
    memset(m_RefocusParam, 0, sizeof(RefocusParam));
    //focus = new Refocus;
}

ConfigParam::~ConfigParam()
{
    delete ui;
}

void ConfigParam::on_pushButton_2_clicked()
{
    ui->imgH->clear();
    ui->imgW->clear();
    ui->alphaMax->clear();
    ui->alphaMin->clear();
    ui->resolution->clear();
    ui->radius->clear();
}

void ConfigParam::on_pushButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    m_dlg = nullptr;
    QHBoxLayout *lay = new QHBoxLayout(&dlg);
    QPushButton *btnCpu = new QPushButton("CPU");
    btnCpu->setObjectName("cpu");
    connect(btnCpu, SIGNAL(clicked()), this, SLOT(SlotCpuBtnClicked()));
    QPushButton *btnGpu = new QPushButton("GPU");
    connect(btnGpu, SIGNAL(clicked()), this, SLOT(SlotGpuBtnClicked()));
    btnGpu->setObjectName("gpu");
    lay->addSpacerItem(new QSpacerItem(20,20));
    lay->addWidget(btnCpu);
    lay->addWidget(btnGpu);
    lay->addSpacerItem(new QSpacerItem(20,20));
    dlg.setFixedSize(300, 60);
    btnCpu->setFixedSize(100, 28);
    btnGpu->setFixedSize(100, 28);
    m_dlg = &dlg;
    dlg.exec();

    m_RefocusParam->radius = ui->radius->text().toInt();
    m_RefocusParam->imgH = ui->imgH->text().toInt();
    m_RefocusParam->imgW = ui->imgW->text().toInt();
    m_RefocusParam->alphaMin = ui->alphaMin->text().toFloat();
    m_RefocusParam->alphaMax = ui->alphaMax->text().toFloat();
    m_RefocusParam->DepthResolution = ui->resolution->text().toInt();



    emit SigParam(m_RefocusParam);


    emit SigToPage(3);


}

void ConfigParam::SlotCpuBtnClicked()
{
    CPUflag = true;
    qDebug() << "cpu clicked!";
    if(m_dlg != nullptr)
    {
        m_dlg->hide();
        m_dlg = nullptr;
    }
}

void ConfigParam::SlotGpuBtnClicked()
{
    CPUflag = false;
    qDebug() << "gpu clicked!";
    if(m_dlg != nullptr)
    {
        m_dlg->hide();
        m_dlg = nullptr;
    }
}
