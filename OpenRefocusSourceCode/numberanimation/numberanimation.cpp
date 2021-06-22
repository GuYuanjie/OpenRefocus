
#include "numberanimation.h"
#include <QDebug>
#include <QApplication>
#include <QDebug>

NumberAnimation::NumberAnimation(QWidget *parent, double *targetValue): m_parentWidget(parent), m_targetValue(targetValue), m_loopFlage(false)
{
    connect(this, SIGNAL(valueChanged(const QVariant &)), this, SLOT(receiveValueChanged(const QVariant &)));
    connect(this, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void NumberAnimation::startAnimation(const QVariant &startValue, const QVariant &endValue)
{
    if (this->state() == QVariantAnimation::Running)
    {
        this->stop();
    }
    this->setStartValue(startValue);
    this->setEndValue(endValue);
    this->start();
}

void NumberAnimation::setLoopFlage(bool loopFlage)
{
    m_loopFlage = loopFlage;
}

void NumberAnimation::receiveValueChanged(const QVariant &value)
{
    *m_targetValue = value.toDouble();
    this->updateParentWidget();
}

void NumberAnimation::animationFinished()
{
    if (m_loopFlage)
    {
        this->startAnimation(this->endValue(), this->startValue());
    }
}

void NumberAnimation::updateParentWidget()
{
    if (nullptr != m_parentWidget)
    {
        m_parentWidget->update();
    }
}
