
#include "controltreewidget.h"
#include "ui_controltreewidget.h"

ControlTreeWidget::ControlTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlTreeWidget)
{
    ui->setupUi(this);
    this->initValue();
}

ControlTreeWidget::~ControlTreeWidget()
{
    delete ui;
}

void ControlTreeWidget::setCurrentIndex(int index)
{
    emit sendShowIndex(WidgetTabType(index));
    QTreeWidgetItem *treeWidgetItem = ui->treeWidgetMenu->topLevelItem(index);
    if (nullptr != treeWidgetItem)
    {
        ui->treeWidgetMenu->clearSelection();
        treeWidgetItem->setSelected(true);
    }
}

void ControlTreeWidget::initValue()
{
    ui->treeWidgetMenu->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeWidgetMenu->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->treeWidgetMenu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidgetMenu->setHeaderHidden(true);
    ui->treeWidgetMenu->header()->setStretchLastSection(true);
    ui->treeWidgetMenu->topLevelItem(0)->setSelected(true);
    ui->treeWidgetMenu->setFocusPolicy(Qt::NoFocus);
}

void ControlTreeWidget::on_treeWidgetMenu_clicked(const QModelIndex &index)
{
    emit sendShowIndex(WidgetTabType(index.row()));
    QTreeWidgetItem *treeWidgetItem = ui->treeWidgetMenu->topLevelItem(index.row());
    if (nullptr != treeWidgetItem)
    {
        ui->treeWidgetMenu->clearSelection();
        treeWidgetItem->setSelected(true);
    }
}

void ControlTreeWidget::DealSendShowIndex(int type)
{
    QTreeWidgetItem *treeWidgetItem = ui->treeWidgetMenu->topLevelItem(type);
    if (nullptr != treeWidgetItem)
    {
        ui->treeWidgetMenu->clearSelection();
        treeWidgetItem->setSelected(true);
    }
}
