#include "QCusFluentMaximumSizeButton_p.h"

#include <QStyle>

QCusFluentMaximumSizeButton::QCusFluentMaximumSizeButton(QWidget* parent) : QPushButton(parent), m_maxsized(false) {}

QCusFluentMaximumSizeButton::~QCusFluentMaximumSizeButton() {}

void QCusFluentMaximumSizeButton::setMaximumSized(bool max)
{
    this->style()->unpolish(this);
    m_maxsized = max;
    this->style()->polish(this);
}

bool QCusFluentMaximumSizeButton::maximumSized() const { return m_maxsized; }