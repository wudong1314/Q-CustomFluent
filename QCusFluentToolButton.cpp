#include "QCusFluentToolButton_p.h"

#include <QAction>
#include <QEvent>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>

struct QCusFluentToolButton::Impl
{
    bool showToolTip{ true };
    bool selected{ false };
};

QCusFluentToolButton::QCusFluentToolButton(QWidget* parent) : QToolButton(parent), m_impl(std::make_unique<Impl>())
{
    connect(this,
        &QAbstractButton::toggled,
        this,
        [=](bool)
        {
            style()->unpolish(this);
            style()->polish(this);
        });
}

QCusFluentToolButton::~QCusFluentToolButton() {}

void QCusFluentToolButton::setIcon(const QIcon& icon)
{
    if (defaultAction())
    {
        defaultAction()->setIcon(icon);
    }
    else
        QToolButton::setIcon(icon);
}

void QCusFluentToolButton::setShowToolTips(bool show) { m_impl->showToolTip = show; }

bool QCusFluentToolButton::showToolTips() const { return m_impl->showToolTip; }

void QCusFluentToolButton::setSelected(bool select) { m_impl->selected = select; }

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void QCusFluentToolButton::initStyleOption(QStyleOptionToolButton* option) const
{
    QToolButton::initStyleOption(option);
    if (m_impl->selected)
        option->icon = this->icon().pixmap(
            iconSize(), m_impl->selected ? QIcon::Mode::Selected : QIcon::Mode::Normal, isChecked() ? QIcon::State::On : QIcon::State::Off);
}
#else
void QCusFluentToolButton::paintEvent(QPaintEvent*)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    if (m_impl->selected)
        opt.icon = this->icon().pixmap(
            iconSize(), m_impl->selected ? QIcon::Mode::Selected : QIcon::Mode::Normal, isChecked() ? QIcon::State::On : QIcon::State::Off);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}
#endif
bool QCusFluentToolButton::event(QEvent* e)
{
    if (e->type() == QEvent::ToolTip)
    {
        return m_impl->showToolTip ? QToolButton::event(e) : true;
    }
    return QToolButton::event(e);
}