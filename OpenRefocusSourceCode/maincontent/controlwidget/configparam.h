#ifndef CONFIGPARAM_H
#define CONFIGPARAM_H

#include <QWidget>
#include <QDialog>
#include <QDebug>
#include "common.h"
#include "refocus.h"
namespace Ui {
class ConfigParam;
}

class ConfigParam : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigParam(QWidget *parent = nullptr);
    ~ConfigParam();
signals:
    void SigToPage(int);
    void SigParam(RefocusParam*);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void SlotCpuBtnClicked();
    void SlotGpuBtnClicked();

private:
    Ui::ConfigParam *ui;
    RefocusParam *m_RefocusParam;
    Refocus *focus;
    QDialog *m_dlg;
};

#endif // CONFIGPARAM_H
