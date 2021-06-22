#ifndef LOADPOS_H
#define LOADPOS_H

#include <QWidget>



namespace Ui {
class LoadPos;
}

class LoadPos : public QWidget
{
    Q_OBJECT
private:
    bool xdataState=false;
    bool ydataState=false;
public:
    explicit LoadPos(QWidget *parent = nullptr);
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    ~LoadPos();
signals:
    void SigToPage(int);
private slots:
    void on_btnLoad1_clicked();

    void on_btnLoad2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::LoadPos *ui;
};

#endif // LOADPOS_H
