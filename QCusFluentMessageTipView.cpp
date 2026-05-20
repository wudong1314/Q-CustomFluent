#include "QCusFluentMessageTipView.h"
#include "QCusFluentMessageViewDelegate_p.h"
#include "QCusFluentAbstractMessageTipModel.h"
#include "QCusFluentMessageTipData.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QHelpEvent>
#include <QListView>
#include <QScrollBar>
#include <QToolButton>
#include <QToolTip>

struct QCusFluentMessageTipView::Impl
{
    QListView* view{ nullptr };
    QCusFluentAbstractMessageTipModel* model{ nullptr };
    QCusFluentMessageViewDelegate* delegate{ nullptr };
    QToolButton* clearAll;

    QColor titleColor;
    QColor timeColor;
};

/*!
    \class      QCusFluentMessageTipView
    \brief      QCusFluentMessageTipView 是显示具体消息的列表.
    \inherits   QCusFluentAbstractMessageTipView
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCusFluentMessageTipView.h
*/

/*!
    \property   QCusFluentMessageTipView::decoration
    \brief      装饰条的颜色.
*/

/*!
    \property   QCusFluentMessageTipView::titleColor
    \brief      消息标题的颜色.
*/

/*!
    \property   QCusFluentMessageTipView::timeColor
    \brief      时间的颜色.
*/

/*!
    \property   QCusFluentMessageTipView::closeButtonIcon
    \brief      关闭按钮的图标.
*/

/*!
    \fn         void QCusFluentMessageTipView::closeButtonClicked(const QModelIndex& index)
    \brief      当  \a index 位置的消息的关闭按钮被点击时，发送该信号.
    \sa         messageClicked
*/

/*!
    \fn         void QCusFluentMessageTipView::messageClicked(QCusFluentMessageTipDataPtr message);
    \brief      当消息被点击时，发送该信号 \a message.
    \sa         closeButtonClicked
*/

/*!
    \fn         bool QCusFluentMessageTipView::aboutToCloseMessage(QCusFluentMessageTipDataPtr message);
    \brief      如果要在关闭消息时，弹出确认窗口，连接该信号，并在槽函数中返回是否删除 \a message.
*/

/*!
    \fn         bool QCusFluentMessageTipView::aboutToClearAllMessages();
    \brief      如果要在清除所有消息时，弹出确认窗口，连接该信号，并在槽函数中返回是否删除.
*/

/*!
    \brief      构造一个父对象为 \a parent 的消息列表.
*/
QCusFluentMessageTipView::QCusFluentMessageTipView(QCusFluentNavigationBar* parent) : QCusFluentAbstractMessageTipView(parent), m_impl(std::make_unique<Impl>())
{
    this->setTitleBarVisible(true);
    setPopup(false);
    m_impl->clearAll = new QToolButton(this);
    m_impl->clearAll->setObjectName("qcustomui_clearAll");
    m_impl->clearAll->setCursor(Qt::PointingHandCursor);
    m_impl->clearAll->setText("Clear All");
    m_impl->view = new QListView(this);
    m_impl->view->setObjectName("qcustomui_tipview");
    m_impl->view->setMouseTracking(true);

    auto base = new QWidget(this);
    {
        auto layout = new QVBoxLayout(base);
        layout->setContentsMargins(0, 0, 0, 0);
        {
            auto btnBase = new QWidget(this);
            btnBase->setObjectName("qcustomui_clearAllBase");
            auto l = new QHBoxLayout(btnBase);
            l->addStretch(1);
            l->addWidget(m_impl->clearAll);
            l->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(btnBase);
        }
        layout->addWidget(m_impl->view);
        layout->setSpacing(0);
        setWidget(base);
    }

    m_impl->delegate = new QCusFluentMessageViewDelegate(m_impl->view);
    m_impl->view->setItemDelegate(m_impl->delegate);
    setFixedWidth(300);

    connect(m_impl->delegate, &QCusFluentMessageViewDelegate::closeButtonClicked, this, &QCusFluentMessageTipView::closeButtonClicked);
    connect(m_impl->delegate, &QCusFluentMessageViewDelegate::closeButtonClicked, this, &QCusFluentMessageTipView::onCloseButtonClicked);
    connect(m_impl->delegate, &QCusFluentMessageViewDelegate::titleClicked, this, &QCusFluentMessageTipView::onTitleClicked);
    connect(m_impl->clearAll, &QToolButton::clicked, this, &QCusFluentMessageTipView::clearAll);
    connect(this, &QCusFluentMessageTipView::aboutToCloseMessage, this, [] { return true; });
    connect(this, &QCusFluentMessageTipView::aboutToClearAllMessages, this, [] { return true; });
    m_impl->view->verticalScrollBar()->installEventFilter(this);

    qRegisterMetaType<QCusFluentAbstractMessageTipDataPtr>("QCusFluentAbstractMessagePtr");
    setDecoration(0xA7A7A7);
    setCloseButtonIcon(QPixmap(":/QCustomUi/Resources/close-light.png"));
    setTitleColor(0x303030);
    setTimeColor(0x656464);
}

/*!
    \brief      销毁该消息列表.
*/
QCusFluentMessageTipView::~QCusFluentMessageTipView() {}

/*!
    \brief      设置消息 \a model.
    \sa         model()
*/
void QCusFluentMessageTipView::setModel(QCusFluentAbstractMessageTipModel* model)
{
    m_impl->model = model;
    setTimeColor(m_impl->timeColor);
    setTitleColor(m_impl->titleColor);
    m_impl->view->setModel(model);
}

/*!
    \brief      返回消息 model.
    \sa         setModel
*/
QCusFluentAbstractMessageTipModel* QCusFluentMessageTipView::model() const { return m_impl->model; }

/*!
    \brief      设置装饰颜色 \a color.
    \sa         decoration()
*/
void QCusFluentMessageTipView::setDecoration(const QColor& color) { m_impl->delegate->setDecoration(color); }

/*!
    \brief      返回装饰颜色.
    \sa         setDecoration
*/
const QColor& QCusFluentMessageTipView::decoration() const { return m_impl->delegate->decoration(); }

/*!
    \brief      设置标题栏颜色 \a color.
    \sa         titleColor()
*/
void QCusFluentMessageTipView::setTitleColor(const QColor& color)
{
    m_impl->titleColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("titlecolor", color);
}

/*!
    \brief      返回标题栏颜色.
    \sa         setTitleColor
*/
const QColor& QCusFluentMessageTipView::titleColor() const { return m_impl->titleColor; }

/*!
    \brief      设置时间颜色 \a color.
    \sa         timeColor()
*/
void QCusFluentMessageTipView::setTimeColor(const QColor& color)
{
    m_impl->timeColor = color;
    if (m_impl->model)
        m_impl->model->setProperty("timecolor", color);
}

/*!
    \brief      返回时间颜色.
    \sa         setTimeColor
*/
const QColor& QCusFluentMessageTipView::timeColor() const { return m_impl->timeColor; }

/*!
    \brief      设置关闭按钮图标 \a icon.
    \sa         closeButtonIcon()
*/
void QCusFluentMessageTipView::setCloseButtonIcon(const QPixmap& icon) { m_impl->delegate->setCloseButtonIcon(icon); }

/*!
    \brief      返回关闭按钮图标.
    \sa         setCloseButtonIcon
*/
const QPixmap& QCusFluentMessageTipView::closeButtonIcon() const { return m_impl->delegate->closeButtonIcon(); }

/*!
    \brief      设置为触控外观，即一直显示标题超链接样式和关闭按钮，默认为 false \a flag.
    \sa         touchControlStyle
*/
void QCusFluentMessageTipView::setTouchControlStyle(bool flag)
{
    m_impl->delegate->setTouchControlStyle(flag);
    m_impl->view->viewport()->update();
}

/*!
    \brief      返回是否为触控外观.
    \sa         setTouchControlStyle
*/
bool QCusFluentMessageTipView::touchControlStyle() const { return m_impl->delegate->touchControlStyle(); }

/*!
    \brief      清空所有消息.
*/
void QCusFluentMessageTipView::clearAll()
{
    if (emit aboutToClearAllMessages())
    {
        m_impl->model->clear();
    }
}
/*!
    \reimp
*/
void QCusFluentMessageTipView::resizeEvent(QResizeEvent* e)
{
    QCusFluentAbstractMessageTipView::resizeEvent(e);
    m_impl->view->reset();
}

/*!
    \reimp
*/
void QCusFluentMessageTipView::showEvent(QShowEvent* e) { QCusFluentAbstractMessageTipView::showEvent(e); }

/*!
    \reimp
*/
bool QCusFluentMessageTipView::eventFilter(QObject* o, QEvent* e)
{
    if (o == m_impl->view->verticalScrollBar())
    {
        if (e->type() == QEvent::Show || e->type() == QEvent::Hide)
            m_impl->view->reset();
    }
    return QCusFluentNavigationSidePane::eventFilter(o, e);
}

/*!
    \brief      关闭 \a index 位置的消息.
*/
void QCusFluentMessageTipView::onCloseButtonClicked(const QModelIndex& index)
{
    if (!index.isValid())
        return;
    if (m_impl->model && emit aboutToCloseMessage(std::dynamic_pointer_cast<QCusFluentMessageTipData>(m_impl->model->message(index.row()))))
    {
        m_impl->model->removeMessage(m_impl->model->message(index.row()));
        m_impl->view->viewport()->update();
    }
}

/*!
    \brief      响应 \a index 位置的消息标题栏点击.
*/
void QCusFluentMessageTipView::onTitleClicked(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto msg = m_impl->model->message(index.row());
        emit messageClicked(std::dynamic_pointer_cast<QCusFluentMessageTipData>(msg));
    }
}