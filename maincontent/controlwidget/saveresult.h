#ifndef SAVERESULT_H
#define SAVERESULT_H

#include <QWidget>
#include <QMouseEvent>
#include "refocus.h"
namespace Ui {
class SaveResult;
}

class SaveResult : public QWidget
{
    Q_OBJECT

public:
    explicit SaveResult(QWidget *parent = nullptr);
    ~SaveResult();
    void LoadResult();
    QImage cvMat2QImage(const cv::Mat& mat);


public:
    vector<cv::Mat> refocusImgBlur;
    int CurrentImgIdx=0;
    static int const RETCODE_RESTART = -12345678;
    void ValueChanged(int value);

signals:
    void SigToPage(int);
    void sliderMoved(int value);
    void sliderPressed();
    void sliderReleased();
    void valueChanged(int value);
private slots:

    void on_btnSavfeCur_clicked();
    void on_btnSaveAll_clicked();
    void on_btnFirstPage_clicked();
    void on_btnDepthPic_clicked();
    void on_btnGrad_clicked();
    void on_btnHistogram_clicked();
    void on_verticalSlider_slidered();
private:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    //virtual void mouseMoveEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    //virtual void mouseReleaseEvent(QMouseEvent *event);
    //virtual void mouseDoubleClickEvent(QMouseEvent *event);
    //virtual void wheelEvent(QWheelEvent *event);

    void setSingleStep(int);
    void setPageStep(int);
    void setValue(int);
    int value() const;
    void setTracking(bool enable);
    void setOrientation(Qt::Orientation);



    Ui::SaveResult *ui;

};

#endif // SAVERESULT_H
