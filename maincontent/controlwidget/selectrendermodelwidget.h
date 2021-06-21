#ifndef SELECTRENDERMODELWIDGET_H
#define SELECTRENDERMODELWIDGET_H

#include <QWidget>

namespace Ui {
class SelectRenderModelWidget;
}

class SelectRenderModelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectRenderModelWidget(QWidget *parent = nullptr);
    ~SelectRenderModelWidget();

private:
    Ui::SelectRenderModelWidget *ui;
};

#endif // SELECTRENDERMODELWIDGET_H
