#include "QCusFluentSeparator_p.h"

#include <QPainter>

QCusFluentSeparator::QCusFluentSeparator(Qt::Orientation orientation, QWidget* parent) : QWidget(parent), m_orientation(orientation)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QCusFluentSeparator::~QCusFluentSeparator() {}

Qt::Orientation QCusFluentSeparator::orientation() const { return m_orientation; }

QSize QCusFluentSeparator::sizeHint() const
{
    QStyleOption opt;
    initStyleOption(&opt);
    const int extent = style()->pixelMetric(QStyle::PM_ToolBarSeparatorExtent, &opt, parentWidget());
    return QSize(extent, extent);
}

void QCusFluentSeparator::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QStyleOption opt;
    initStyleOption(&opt);
    style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, &p, parentWidget());
}

void QCusFluentSeparator::initStyleOption(QStyleOption* opt) const
{
    opt->initFrom(this);
    if (orientation() == Qt::Horizontal)
        opt->state |= QStyle::State_Horizontal;
    const auto& rect = style()->subElementRect(QStyle::SE_FrameContents, opt, this);
    if (rect.isValid())
    {
        opt->rect = rect;
    }
}

void QCusFluentSeparator::setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }