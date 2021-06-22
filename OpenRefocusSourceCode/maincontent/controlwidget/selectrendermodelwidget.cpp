#include "selectrendermodelwidget.h"
#include "ui_selectrendermodelwidget.h"

SelectRenderModelWidget::SelectRenderModelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectRenderModelWidget)
{
    ui->setupUi(this);
}

SelectRenderModelWidget::~SelectRenderModelWidget()
{
    delete ui;
}
