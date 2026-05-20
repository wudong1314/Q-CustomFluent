
#include "QCusFluentMessageTipButton.h"
#include "QCusFluentMessageTipHelper_p.h"

#include <QEvent>
#include <QPainter>
#include <QStyleOption>

#include <assert.h>

struct QCusFluentMessageTipHelper::Impl
{
    QSize size{ 14, 16 };
    int margin{ 5 };
    int radius{ 25 };

    QColor tipColor;
};

QCusFluentMessageTipHelper::QCusFluentMessageTipHelper(QCusFluentMessageTipButton* parent) : QObject(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->size = parent->fontMetrics().size(Qt::TextSingleLine, "99");
    parent->installEventFilter(this);
}

QCusFluentMessageTipHelper::~QCusFluentMessageTipHelper() {}

void QCusFluentMessageTipHelper::drawMessageTip(int msgCount, QPainter& p, QWidget* w)
{
    if (!msgCount)
        return;
    if (msgCount > 99)
        msgCount = 99;
    const auto& rect = tipsRect(QRect(QPoint(0, 0), w->size()));
    QStyleOption opt;
    opt.initFrom(w);
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(m_impl->tipColor);
    p.drawRoundedRect(rect, m_impl->radius, m_impl->radius, Qt::RelativeSize);
    p.restore();

    QTextOption to;
    to.setAlignment(Qt::AlignCenter);
    p.save();
    p.setPen(opt.palette.windowText().color());
    p.drawText(rect, QString::number(msgCount), to);
    p.restore();
}

void QCusFluentMessageTipHelper::setTipColor(const QColor& color) { m_impl->tipColor = color; }

const QColor& QCusFluentMessageTipHelper::tipColor() const { return m_impl->tipColor; }

QRect QCusFluentMessageTipHelper::tipsRect(const QRect& rect)
{
    auto nrect = rect.normalized();
    return QRect(QPoint(nrect.right() - m_impl->size.width() - m_impl->margin, nrect.top() + m_impl->margin), m_impl->size);
}

bool QCusFluentMessageTipHelper::eventFilter(QObject* watched, QEvent* event)
{
    auto w = qobject_cast<QCusFluentMessageTipButton*>(watched);
    if (w && event->type() == QEvent::FontChange)
    {
        m_impl->size = w->fontMetrics().size(Qt::TextSingleLine, "99");
        ;
    }
    return false;
}