#include "QCusFluentTitleBar.h"
#include "QCusFluentWidgetItem_p.h"
#include "QCusFluentUtil_p.h"
#include "ui_QCusFluentTitleBar.h"

#include <QMenuBar>
#include <QPainter>
#include <QPointer>
#include <QResizeEvent>
#include <QStyleOption>
#include <QPainterPath>

Q_CONSTEXPR int leftMargin = 5;
Q_CONSTEXPR int titleSpacing = 5;

struct QCusFluentTitleBar::Impl
{
    QPixmap windowIcon;
    QPointer<QMenuBar> menuBar;
    bool showIcon{ true };
    QList<QCusFluentWidgetItemPtr> items;
    QSize iconSize{ 16, 16 };
};

/*!
    \class      QCusFluentTitleBar
    \brief      QCusFluentWindow 和 QCusFluentDialog 的自定义标题栏.
    \note       最大化，最小化，关闭按钮等显示控制由 QWidget::windowFlags 决定，图标显示由 QWidget::windowIcon
   控制，标题栏文字由centralWidget的QWidget::windowTitle决定. \inherits   QWidget \ingroup    QCustomUi \inmodule   QCustomUi \inheaderfile
   QCusFluentTitleBar.h

    \b          {截图:}
    \image      QCusFluentTitleBarDetail.png
*/

/*!
    \property   QCusFluentTitleBar::iconIsVisible
    \brief      是否显示图标.
*/

/*!
    \brief      构造函数 \a parent.
*/
QCusFluentTitleBar::QCusFluentTitleBar(QWidget* parent)
    : QWidget(parent),
    ui(new Ui::QCusFluentTitleBar),
    m_impl(std::make_unique<Impl>())
{
    ui->setupUi(this);
    Qt::WindowFlags flags = this->windowFlags();
    flags &= ~Qt::WindowSystemMenuHint; // 移除系统菜单提示
    this->setWindowFlags(flags);
    setFocusPolicy(Qt::StrongFocus);
    //connect(ui->closeBtn, &QPushButton::clicked, this, &QCusFluentTitleBar::onCloseBtn);
    connect(ui->closeBtn, &QPushButton::clicked, this, &QCusFluentTitleBar::sigClose);
    connect(ui->minimumSizeBtn, &QPushButton::clicked, this, &QCusFluentTitleBar::onMinimumSizeBtn);
    connect(ui->maximumSizeBtn, &QPushButton::clicked, this, &QCusFluentTitleBar::onMaximumSizeBtn);
    ui->maximumSizeBtn->setProperty("qcustomui_maximumSizeButton", true);

    parent->installEventFilter(this);
    setAttribute(Qt::WA_StyledBackground);
    //setStyleSheet("QWidget{background-image:url(:/QCustomUi/Resources/tipback.png);background-repeat:repeat;}");
}

/*!
    \brief      析构函数.
*/
QCusFluentTitleBar::~QCusFluentTitleBar() { delete ui; }

/*!
    \brief      设置菜单栏 \a menu, 当 \a menu 为nullptr时删除菜单栏.
    \sa         menuBar
*/
void QCusFluentTitleBar::setMenuBar(QMenuBar* menu)
{
    if (menu == m_impl->menuBar)
        return;
    if (m_impl->menuBar)
    {
        m_impl->menuBar->hide();
        m_impl->menuBar->setParent(nullptr);
        m_impl->menuBar->deleteLater();
    }
    if (menu)
    {
        menu->setAutoFillBackground(false);
        menu->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
        ui->horizontalLayout->insertWidget(0, menu);
    }
    m_impl->menuBar = menu;
    update();
}

/*!
    \brief      返回菜单栏, 当菜单栏对象不存在时，自动创建并返回一个新的菜单栏.
    \sa         setMenuBar
*/
QMenuBar* QCusFluentTitleBar::menuBar() const
{
    auto menuBar = m_impl->menuBar;
    if (!menuBar)
    {
        auto self = const_cast<QCusFluentTitleBar*>(this);
        menuBar = new QMenuBar(self);
        self->setMenuBar(menuBar);
    }
    return menuBar;
}

/*!
    \brief      设置是否显示图标 \a show.
    \sa         iconIsVisible()
*/
void QCusFluentTitleBar::setIconVisible(bool show)
{
    m_impl->showIcon = show;
    if (show)
        ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->windowIcon.width(), 0, 0, 0);
    else
        ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
}

/*!
    \brief      返回是否显示图标.
    \sa         setIconVisible
*/
bool QCusFluentTitleBar::iconIsVisible() const { return m_impl->showIcon; }

/*!
    \brief      设置Action图标大小 \a size.
    \sa         iconSize
*/
void QCusFluentTitleBar::setIconSize(const QSize& size) { m_impl->iconSize = size; }

/*!
    \brief      返回Action图标大小.
    \sa         setIconSize
*/
const QSize& QCusFluentTitleBar::iconSize() const { return m_impl->iconSize; }

/*!
    \brief      响应关闭按钮.
    \sa         onMaximumSizeBtn(), onMinimumSizeBtn()
*/
void QCusFluentTitleBar::onCloseBtn() { this->window()->close(); }

/*!
    \brief      响应最大化按钮.
    \sa         onCloseBtn(), onMinimumSizeBtn()
*/
void QCusFluentTitleBar::onMaximumSizeBtn()
{
    if (this->window()->isMaximized() || this->window()->isFullScreen())
        this->window()->showNormal();
    else
        this->window()->showMaximized();
}

/*!
    \brief      响应最小化按钮.
    \sa         onCloseBtn(), onMaximumSizeBtn()
*/
void QCusFluentTitleBar::onMinimumSizeBtn() { this->window()->showMinimized(); }

/*!
    \reimp
*/
void QCusFluentTitleBar::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    const auto& iconRect = doIconRect();
    //if (!m_impl->windowIcon.isNull() && m_impl->showIcon)
    //{
    //    p.drawPixmap(iconRect, m_impl->windowIcon);
    //}

    if (parentWidget())
    {
        auto text = parentWidget()->windowTitle();
        QRect rect(QPoint{ 0, 0 }, opt.fontMetrics.size(Qt::TextSingleLine, text));
        if (m_impl->menuBar)
        {
            if (m_impl->menuBar->geometry().right() >= (this->width() - rect.width()) / 2)
            {
                rect.moveTo((this->width() - m_impl->menuBar->geometry().right() - rect.width()) / 2, 0);
            }
            else
                rect.moveTo((width() - rect.width()) / 2, 0);
        }
        else
            rect.moveTo(leftMargin + iconRect.width() + titleSpacing, 0);
        rect.setHeight(this->height());
        QTextOption to;
        to.setWrapMode(QTextOption::NoWrap);
        to.setAlignment(Qt::AlignCenter);
        QFont font = this->font();
        font.setFamily("Microsoft YaHei");
        p.setFont(font);
        // p.setPen(this->palette().windowText().color());
        p.setPen(QColor(255, 255, 255));
        p.drawText(rect, text, to);
    }

    // //设置背景色;
    //QPainterPath pathBack;
    //pathBack.setFillRule(Qt::WindingFill);
    //pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    //p.setRenderHint(QPainter::Antialiasing, true);
    //p.fillPath(pathBack, QBrush(QColor(104, 104, 104)));
}

/*!
    \reimp
*/
void QCusFluentTitleBar::showEvent([[maybe_unused]] QShowEvent* event)
{
    auto w = qobject_cast<QWidget*>(this->parent());
    if (w)
    {
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowCloseButtonHint))
            ui->closeBtn->hide();
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowMinimizeButtonHint))
            ui->minimumSizeBtn->hide();
        if (!w->windowFlags().testFlag(Qt::WindowType::WindowMaximizeButtonHint))
            ui->maximumSizeBtn->hide();
        if (w->windowState().testFlag(Qt::WindowState::WindowMaximized))
            ui->maximumSizeBtn->setMaximumSized(true);
        else
            ui->maximumSizeBtn->setMaximumSized(false);
    }
}

/*!
    \reimp
*/
bool QCusFluentTitleBar::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == parent())
    {
        auto w = qobject_cast<QWidget*>(this->parent());
        if (!w)
            return false;
        if (event->type() == QEvent::WindowStateChange)
        {
            auto w = qobject_cast<QWidget*>(this->parent());
            if (w)
            {
                if (w->windowState().testFlag(Qt::WindowState::WindowMaximized))
                    ui->maximumSizeBtn->setMaximumSized(true);
                else
                    ui->maximumSizeBtn->setMaximumSized(false);
            }
        }
        else if (event->type() == QEvent::WindowIconChange)
        {
            auto sizes = w->windowIcon().availableSizes();
            if (!sizes.isEmpty())
            {
                m_impl->windowIcon = w->windowIcon().pixmap(sizes.front());
#if QT_VERSION <= QT_VERSION_CHECK(5, 8, 0)
                int size = 16;
#else
                int size = this->style()->pixelMetric(QStyle::PM_TitleBarButtonIconSize);
#endif
                if (m_impl->windowIcon.height() > size)
                {
                    m_impl->windowIcon = m_impl->windowIcon.scaledToHeight(size, Qt::SmoothTransformation);
                }
                if (m_impl->showIcon)
                    ui->horizontalLayout->setContentsMargins(leftMargin + titleSpacing + m_impl->windowIcon.width(), 0, 0, 0);
            }
        }
        else if (event->type() == QEvent::WindowTitleChange)
        {
            update();
        }
        else if (event->type() == QEvent::Resize)
        {
            if (w->windowFlags().testFlag(Qt::WindowMaximizeButtonHint))
            {
                if (w->maximumSize() == w->minimumSize())
                    ui->maximumSizeBtn->hide();
                else
                    ui->maximumSizeBtn->show();
            }
        }
        else if (event->type() == QEvent::NonClientAreaMouseButtonPress ||
            event->type() == QEvent::NonClientAreaMouseButtonDblClick)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            // 如果是右键点击，就拦截它！
            if (mouseEvent->button() == Qt::RightButton)
            {
                return true; // 返回true表示事件已处理，系统菜单不会弹出
            }
        }
    }

    return false;
}

/*!
    \reimp
*/
void QCusFluentTitleBar::actionEvent(QActionEvent* event)
{
    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCusFluentWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCusFluentTitleBar::iconSizeChanged, item.get(), &QCusFluentWidgetItem::iconSizeChanged);
        CusFluentUtil::addItem(item, m_impl->items, event->before(), ui->actionLayout);
    }
    else
    {
        CusFluentUtil::removeItem(event->action(), m_impl->items, ui->actionLayout);
    }
}

/*!
    \brief      计算图标包围盒.
*/
QRect QCusFluentTitleBar::doIconRect() const
{
    return { leftMargin, (this->height() - m_impl->windowIcon.height()) / 2, m_impl->windowIcon.width(), m_impl->windowIcon.height() };
}