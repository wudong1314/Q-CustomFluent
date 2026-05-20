#include "QCusFluentNavigationImageButton_p.h"

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

constexpr auto Padding = 10;
constexpr auto Margins = QMargins{ Padding, Padding, Padding, Padding };

struct QCusFluentNavigationImageButton::Impl
{
    bool pressed = false;
    QPixmap cache;
    inline void generateCache(QCusFluentNavigationImageButton* q, const QSize& size)
    {
        auto st = q->isChecked() ? QIcon::On : QIcon::Off;
        const auto& sizes = q->icon().availableSizes(QIcon::Mode::Normal, st);
        if ((cache.isNull() || cache.size().grownBy(Margins) != size) && !sizes.isEmpty())
        {
            cache = q->icon()
                .pixmap(sizes.front(), QIcon::Normal, st)
                .scaledToHeight(qMin(size.shrunkBy(Margins).height(), sizes.front().height()),
                    Qt::TransformationMode::SmoothTransformation);
            q->updateGeometry();
        }
    }
};

QCusFluentNavigationImageButton::QCusFluentNavigationImageButton(QWidget* parent) : QCusFluentToolButton(parent), m_impl(std::make_unique<Impl>())
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    connect(this,
        &QCusFluentNavigationImageButton::pressed,
        this,
        [=]()
        {
            m_impl->pressed = true;
            update();
        });
    connect(this,
        &QCusFluentNavigationImageButton::released,
        this,
        [=]()
        {
            m_impl->pressed = false;
            update();
        });
}

QCusFluentNavigationImageButton::~QCusFluentNavigationImageButton() {}

void QCusFluentNavigationImageButton::paintEvent([[maybe_unused]] QPaintEvent* e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QStyleOptionButton opt;
    opt.initFrom(this);
    opt.features = QStyleOptionButton::DefaultButton | QStyleOptionButton::AutoDefaultButton;
    ;
    if (m_impl->pressed)
    {
        opt.state |= QStyle::State_Sunken;
        opt.state |= QStyle::State_Raised;
    }
    else if (this->isChecked())
    {
        opt.state |= QStyle::State_On;
    }
    opt.text = this->text();
    style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);

    { // draw icon
        if (!m_impl->cache.isNull())
        {
            style()->drawItemPixmap(&p, opt.rect, Qt::AlignCenter, m_impl->cache);
        }
        else
        {
            m_impl->generateCache(this, this->size());
        }
    }
}

void QCusFluentNavigationImageButton::resizeEvent(QResizeEvent* e)
{
    m_impl->generateCache(this, e->size());
    QCusFluentToolButton::resizeEvent(e);
}

QSize QCusFluentNavigationImageButton::sizeHint() const
{
    if (!m_impl->cache.isNull())
    {
        return m_impl->cache.size().grownBy(Margins);
    }
    return QSize(5, 5);
}