

#ifndef NUMBERANIMATION_H
#define NUMBERANIMATION_H

#include <QWidget>
#include <QBasicTimer>
#include <QVariant>
#include <QVariantAnimation>

class NumberAnimation : public QVariantAnimation
{
    Q_OBJECT
public:
    explicit NumberAnimation(QWidget *parent, double *targetValue);
    void startAnimation(const QVariant &startValue, const QVariant &endValue);
    void setLoopFlage(bool loopFlage);
private slots:
    void receiveValueChanged(const QVariant &value);
    void animationFinished();
private:
    void updateParentWidget();
private:
    QWidget *m_parentWidget;
    double *m_targetValue;
    bool m_loopFlage;
};

#endif // NUMBERANIMATION_H
