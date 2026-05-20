#include "QCusFluentDialog.h"
#include "QCusFluentFramelessDelegate_p.h"
#include "QCusFluentFramelessDelegate_win.h"
#include "QCusFluentTitleBar.h"

#include <QApplication>
#include <QMoveEvent>
#include <QPointer>
#include <QScreen>
#include <QVBoxLayout>

struct QCusFluentDialog::Impl
{
    QPointer<QCusFluentTitleBar> title;
    QWidget* content { nullptr };
    QVBoxLayout* layout { nullptr };
#ifdef Q_OS_WIN
    QCusFluentFramelessDelegate_win* delegate { nullptr };
#else
    QCusFluentFramelessDelegate* delegate { nullptr };
#endif
};

/*!
    \class      QCusFluentDialog
    \brief      这是一个模态窗口，类似于QDialog，去掉了系统的边框和标题栏，用自定义的标题栏替代.
    \inherits   QDialog
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCusFluentDialog.h
*/

/*!
    \brief      构造一个父窗口为 \a parent 的窗口对象.
*/
QCusFluentDialog::QCusFluentDialog(QWidget* parent) : QDialog(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->title = new QCusFluentTitleBar(this);

    m_impl->layout = new QVBoxLayout(this);
    m_impl->layout->setContentsMargins(0, 0, 0, 0);
    m_impl->layout->setSpacing(0);

    m_impl->layout->addWidget(m_impl->title);
    m_impl->layout->setStretch(0, 0);
    m_impl->layout->setStretch(1, 1);
    m_impl->layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

#ifdef Q_OS_WIN
    m_impl->delegate = new QCusFluentFramelessDelegate_win(this, m_impl->title);
#else
    m_impl->delegate = new QCusFluentFramelessDelegate(this, m_impl->title);
    setWindowFlag(Qt::Dialog);
#endif
    setCentralWidget(new QWidget(this));
}

/*!
    \brief      销毁该窗口对象.
*/
QCusFluentDialog::~QCusFluentDialog() {}

/*!
    \brief      设置 \a widget 为窗口的中央窗口, 当 \a widget 为nullptr时删除中央窗口.
    \sa         centralWidget
 */
void QCusFluentDialog::setCentralWidget(QWidget* widget)
{
    if (widget == m_impl->content)
        return;
    if (m_impl->content)
    {
        m_impl->content->hide();
        m_impl->content->deleteLater();
    }
    m_impl->content = widget;
    if (widget)
    {
        QVBoxLayout* l = qobject_cast<QVBoxLayout*>(layout());
        l->addWidget(widget, 1);
        setWindowTitle(widget->windowTitle());
        m_impl->content->installEventFilter(this);
    }
}

/*!
    \brief      返回中央窗口地址, 如果中央窗口不存在则返回nullptr.
    \sa         setCentralWidget
*/
QWidget* QCusFluentDialog::centralWidget() const { return m_impl->content; }

/*!
    \brief      设置标题栏 \a titleBar, 当 \a titleBar 为 nullptr 时删除标题栏.
    \sa         titleBar
*/
void QCusFluentDialog::setTitleBar(QCusFluentTitleBar* titleBar)
{
    if (m_impl->title == titleBar)
        return;

    if (m_impl->title)
    {
        m_impl->delegate->removeMoveBar(m_impl->title);
        m_impl->title->hide();
        m_impl->title->deleteLater();
    }
    m_impl->title = titleBar;
    if (titleBar)
    {
        m_impl->layout->insertWidget(0, titleBar, 0);
        m_impl->delegate->addMoveBar(titleBar);
    }
}

/*!
    \brief      返回窗口的标题栏，如果标题栏不存在，自动创建并返回一个新的标题栏.
    \sa         setTitleBar
*/
QCusFluentTitleBar* QCusFluentDialog::titleBar() const
{
    auto title = m_impl->title;
    if (!title)
    {
        auto self = const_cast<QCusFluentDialog*>(this);
        title     = new QCusFluentTitleBar(self);
        self->setTitleBar(title);
    }
    return title;
}

/*!
    \brief      添加鼠标可拖拽区域 \a moveBar, moveBar必须为该窗口的子控件之一.
    \sa         removeMoveBar
*/
void QCusFluentDialog::addMoveBar(QWidget* moveBar) { m_impl->delegate->addMoveBar(moveBar); }

/*!
    \brief      移除鼠标拖拽区域 \a moveBar.
    \sa         addMoveBar
*/
void QCusFluentDialog::removeMoveBar(QWidget* moveBar) { m_impl->delegate->removeMoveBar(moveBar); }
#ifndef Q_OS_WIN

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Sets the window being shadowless, \a flag.
    \sa         shadowless()
    \else
    \internal
    \endif
*/
void QCusFluentDialog::setShadowless(bool flag) { m_impl->delegate->setShadowless(flag); }

/*!
    \if         !defined(Q_OS_WIN)
    \brief      Returns the window is shadowless.
    \sa         setShadowless
    \else
    \internal
    \endif
*/
bool QCusFluentDialog::shadowless() const { return m_impl->delegate->shadowless(); }
#endif

/*!
    \reimp
*/
void QCusFluentDialog::hideEvent(QHideEvent* e)
{
    auto closeBtn = m_impl->title->findChild<QWidget*>("closeBtn");
    if (closeBtn)
    {
        auto e = new QEvent(QEvent::Type::Leave);
        qApp->sendEvent(closeBtn, e);
    }
    QDialog::hideEvent(e);
}

/*!
    \reimp
*/
bool QCusFluentDialog::eventFilter(QObject* o, QEvent* e)
{
    if (o == m_impl->content && e->type() == QEvent::WindowTitleChange)
    {
        setWindowTitle(m_impl->content->windowTitle());
    }
    return QDialog::eventFilter(o, e);
}

/*!
    \reimp
*/
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QCusFluentDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#else
bool QCusFluentDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#endif
{
#ifdef Q_OS_WIN
    if (!m_impl->delegate)
        return QDialog::nativeEvent(eventType, message, result);
    if (!m_impl->delegate->nativeEvent(eventType, message, result))
        return QDialog::nativeEvent(eventType, message, result);
    else
        return true;
#else
    return QDialog::nativeEvent(eventType, message, result);
#endif
}
