#ifndef LOADIMAGEWGT_H
#define LOADIMAGEWGT_H

#include <QWidget>

namespace Ui {
class LoadImageWgt;
}

class LoadImageWgt : public QWidget
{
    Q_OBJECT

public:
    explicit LoadImageWgt(QWidget *parent = nullptr);

    ~LoadImageWgt();
signals:
    void SigToPage(int);
private slots:

    void on_btnNextPage_clicked();


private:
    Ui::LoadImageWgt *ui;
};

#endif // LOADIMAGEWGT_H
