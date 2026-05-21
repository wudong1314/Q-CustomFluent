#include "QCusFluentFramelessDelegate_p.h"

#include <QWidget>
#include <QEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

struct QCusFluentFramelessDelegate::Impl
{
    QWidgetList moveBars;
    bool shadowlessFlag{false};
};

QCusFluentFramelessDelegate::QCusFluentFramelessDelegate(QWidget* parent, const QWidgetList& moveBars)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->moveBars = moveBars;
    if (parent)
        parent->installEventFilter(this);
}

QCusFluentFramelessDelegate::QCusFluentFramelessDelegate(QWidget* parent, QWidget* title)
    : QCusFluentFramelessDelegate(parent, QWidgetList { title })
{
}

QCusFluentFramelessDelegate::~QCusFluentFramelessDelegate() = default;

void QCusFluentFramelessDelegate::addMoveBar(QWidget* widget)
{
    if (!m_impl->moveBars.contains(widget))
        m_impl->moveBars.append(widget);
}

void QCusFluentFramelessDelegate::removeMoveBar(QWidget* widget)
{
    m_impl->moveBars.removeAll(widget);
}

void QCusFluentFramelessDelegate::setShadowless(bool flag)
{
    m_impl->shadowlessFlag = flag;
}

bool QCusFluentFramelessDelegate::shadowless() const
{
    return m_impl->shadowlessFlag;
}

bool QCusFluentFramelessDelegate::eventFilter(QObject* /*obj*/, QEvent* /*e*/)
{
    return false;
}

void QCusFluentFramelessDelegate::resizeEvent(QResizeEvent* /*e*/) {}
void QCusFluentFramelessDelegate::mousePressEvent(QMouseEvent* /*e*/) {}
void QCusFluentFramelessDelegate::mouseMoveEvent(QMouseEvent* /*e*/) {}
void QCusFluentFramelessDelegate::mouseReleaseEvent(QMouseEvent* /*e*/) {}
void QCusFluentFramelessDelegate::mouseDoubleClickEvent(QMouseEvent* /*e*/) {}
void QCusFluentFramelessDelegate::paintEvent(QPaintEvent* /*e*/) {}
void QCusFluentFramelessDelegate::paintShadow(QPainter& /*p*/, int /*shadowWidth*/) {}
void QCusFluentFramelessDelegate::styleChangeEvent(QEvent* /*e*/) {}
void QCusFluentFramelessDelegate::platformSurfaceEvent(QPlatformSurfaceEvent* /*e*/) {}
