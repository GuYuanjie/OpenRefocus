/*****************************************
 * 作者: YYC
 * 日期: 2020-04-26
 * 功能：界面左侧的TreeWidget，用来列出当前
 * 已有的界面，通过点击显示对应的Tab页
 * ***************************************/
#ifndef CONTROLTREEWIDGET_H
#define CONTROLTREEWIDGET_H

#include <QWidget>
#include "globaldef.hpp"

namespace Ui {
class ControlTreeWidget;
}

class ControlTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlTreeWidget(QWidget *parent = 0);
    ~ControlTreeWidget();
    void setCurrentIndex(int index);
signals:
    void sendShowIndex(WidgetTabType);
public slots:
    void on_treeWidgetMenu_clicked(const QModelIndex &index);
    void DealSendShowIndex(int);

private:
    Ui::ControlTreeWidget *ui;
    void initValue();
};

#endif // CONTROLTREEWIDGET_H
