
#include "QCusFluentNavigationBar.h"
#include "QCusFluentNavigationImageButton_p.h"
#include "QCusFluentUserButton_p.h"
#include "QCusFluentWidgetItem_p.h"
#include "QCusFluentUtil_p.h"
#include "QCusFluentMessageTipButton.h"
#include "QCusFluentNavigationSidePane.h"

#include <QAction>
#include <QActionEvent>
#include <QDesktopServices>
#include <QFrame>
#include <QHBoxLayout>
#include <QMenu>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QToolButton>
#include <QUrl>
#include <QWidgetAction>

static Q_CONSTEXPR const char* ActionPosProperty = "QCusFluent_ActionPosition";

struct QCusFluentNavigationBar::Impl
{
    QList<QCusFluentWidgetItemPtr> leftItems;
    QList<QCusFluentWidgetItemPtr> centerItems;
    QList<QCusFluentWidgetItemPtr> rightItems;

    QHBoxLayout* leftLayout;
    QHBoxLayout* centerLayout;
    QHBoxLayout* rightLayout;

    QSize iconSize{ 16, 16 };

    QCusFluentWidgetItemPtr find(QAction* action)
    {
        auto item = find(action, Left);
        if (item)
            return item;
        item = find(action, Center);
        return item ? item : find(action, Right);
    }

    QCusFluentWidgetItemPtr find(QAction* action, const QList<QCusFluentWidgetItemPtr>& items)
    {
        for (const auto& item : items)
        {
            if (item->action() == action)
                return item;
        }
        return nullptr;
    }

    QCusFluentWidgetItemPtr find(QAction* action, ActionPosition pos)
    {
        switch (pos)
        {
        case Left:
            return find(action, leftItems);
        case Center:
            return find(action, centerItems);
        case Right:
            return find(action, rightItems);
        }
        return nullptr;
    }

    QCusFluentWidgetItemPtr find(QWidget* widget, const QList<QCusFluentWidgetItemPtr>& items)
    {
        for (const auto& item : items)
        {
            if (item->widget() == widget)
                return item;
        }
    }

    QCusFluentWidgetItemPtr find(QWidget* widget, ActionPosition pos)
    {
        switch (pos)
        {
        case Left:
            return find(widget, leftItems);
        case Center:
            return find(widget, centerItems);
        case Right:
            return find(widget, rightItems);
        }
        return nullptr;
    }
};

/*!
    \class      QCusFluentNavigationBar
    \brief      QCusFluentNavigationBar类似于QToolBar，它可以配合QCusFluentWindow和QCusFluentNavigationPane使用.
    \inherits   QWidget
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCusFluentNavigationBar.h

    \b          {The navigation bar screenshot:}
    \image      QCusFluentNavigationBarDetail.png
*/

/*!
    \enum       QCusFluentNavigationBar::ActionPosition
                描述Action的显示位置.
    \value      Left
                靠左显示.
    \value      Center
                居中显示.
    \value      Right
                靠右显示.
*/

/*!
    \fn         void QCusFluentNavigationBar::iconSizeChanged(const QSize& size);
    \brief      当图标大小发生改变时发送该信号 \a size.
    \sa         setIconSize
*/

/*!
    \brief      构造一个父对象为 \a parent 的QCusFluentNavigationBar.
*/
QCusFluentNavigationBar::QCusFluentNavigationBar(QWidget* parent) : QWidget(parent), m_impl(std::make_unique<Impl>())
{
    setFixedHeight(50);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    auto initLayout = [](QHBoxLayout** layout)
    {
        *layout = new QHBoxLayout;
        (*layout)->setContentsMargins(0, 0, 0, 0);
        (*layout)->setSpacing(1);
    };

    initLayout(&m_impl->leftLayout);
    initLayout(&m_impl->centerLayout);
    initLayout(&m_impl->rightLayout);

    layout->addLayout(m_impl->leftLayout);
    layout->addStretch(1);
    layout->addLayout(m_impl->centerLayout);
    layout->addStretch(1);
    layout->addLayout(m_impl->rightLayout);
    setAttribute(Qt::WA_StyledBackground);
}

/*!
    \brief      销毁当前QCusFluentNavigationBar对象.
*/
QCusFluentNavigationBar::~QCusFluentNavigationBar() {}

/*!
    \overload   addAction
                将 \a action 添加到 \a pos 位置.
    \sa         QWidget::addAction
*/
void QCusFluentNavigationBar::addAction(QAction* action, ActionPosition pos)
{
    int index = pos == ActionPosition::Right ? m_impl->rightItems.size()
        : pos == ActionPosition::Center ? m_impl->centerItems.size()
        : m_impl->leftItems.size();
    insertAction(index, action, pos);
}

/*!
    \overload   addAction
                该重载函数创建并返回一个图标为 \a icon, 文本为 \a text 的action，并显示到 \a pos 位置.
    \sa         QCusFluentNavigationBar::addAction
*/
QAction* QCusFluentNavigationBar::addAction(const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto action = new QAction(icon, text, this);
    addAction(action, pos);
    return action;
}

/*!
    \overload   addAction
                该重载函数创建并返回一个文本为 \a text 的action，并显示到 \a pos 位置.
    \sa         QCusFluentNavigationBar::addAction
*/
QAction* QCusFluentNavigationBar::addAction(const QString& text, ActionPosition pos) { return addAction(QIcon(), text, pos); }

/*!
    \overload   insertAction
                该重载函数将 \a action 插入到 \a index 的位置，并显示到 \a pos 位置.
    \sa         QWidget::insertAction
*/
void QCusFluentNavigationBar::insertAction(int index, QAction* action, ActionPosition pos)
{
    auto before = actionAt(index, pos);
    action->setProperty(ActionPosProperty, pos);
    QWidget::insertAction(before, action);
}

/*!
    \overload   insertAction
    \brief      该重载函数创建并返回一个图标为 \a icon, 文本为 \a text 的action，并插入到 \a index 位置，显示到 \a pos 位置.
    \sa         QCusFluentNavigationBar::insertAction
*/
QAction* QCusFluentNavigationBar::insertAction(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
    auto action = new QAction(icon, text, nullptr);
    insertAction(index, action, pos);
    return action;
}

/*!
    \overload   insertAction
                该重载函数创建并返回一个文本为 \a text 的action，并插入到 \a index 位置，显示到 \a pos 位置.
    \sa         QCusFluentNavigationBar::insertAction
*/
QAction* QCusFluentNavigationBar::insertAction(int index, const QString& text, ActionPosition pos)
{
    return insertAction(index, QIcon(), text, pos);
}

/*!
    \brief      在 \a pos 位置添加一个分隔符.
    \sa         insertSeparator
*/
QAction* QCusFluentNavigationBar::addSeparator(ActionPosition pos) { return insertSeparator(count(pos), pos); }

/*!
    \brief      在 \a index, \a pos 位置插入一个分隔符.
    \sa         addSeparator
*/
QAction* QCusFluentNavigationBar::insertSeparator(int index, ActionPosition pos)
{
    auto action = new QAction(nullptr);
    action->setSeparator(true);
    insertAction(index, action, pos);
    return action;
}

/*!
    \brief      在 \a pos 位置添加一个 \a action 并与 \a pane 绑定.
    \sa         insertPane
*/
void QCusFluentNavigationBar::addPane(QAction* action, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    insertPane(count(pos), action, pos, pane);
}

/*!
    \overload   addPane
                添加并返回一个图标为 \a icon, 文本为 \a text 的action，显示到 \a pos 位置，并且与 \a pane 绑定.
    \sa         QCusFluentNavigationBar::addPane
*/
QAction* QCusFluentNavigationBar::addPane(const QIcon& icon, const QString& text, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    auto action = new QAction(icon, text, this);
    addPane(action, pos, pane);
    return action;
}

/*!
    \overload   addPane
                添加并返回一个文本为 \a text 的action，显示到 \a pos 位置，并且与 \a pane 绑定.
    \sa         QCusFluentNavigationBar::addPane
*/
QAction* QCusFluentNavigationBar::addPane(const QString& text, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    return addPane(QIcon(), text, pos, pane);
}

/*!
    \brief      插入一个 \a action 到 \a index 位置，显示到 \a pos 位置，并且与 \a pane 绑定.
    \sa         insertAction
*/
void QCusFluentNavigationBar::insertPane(int index, QAction* action, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    if (!this->actions().contains(action))
        insertAction(index, action, pos);
    action->setCheckable(true);
    if (pane->navigationBar() != this)
    {
        pane->setNavigationBar(this);
    }
    pane->bindAction(action);
    pane->hide();

    connect(action,
        &QAction::toggled,
        this,
        [=](bool checked)
        {
            if (checked)
                pane->show();
            else
                pane->hide();
        });

    connect(
        pane, &QCusFluentNavigationSidePane::paneClosed, this, [=]() { action->setChecked(false); }, Qt::QueuedConnection);
}

/*!
    \overload   insertPane
                插入并返回一个图标为 \a icon, 文本为 \a text 的action，并插入到 \a index 位置，显示到 \a pos 位置，并且与 \a pane 绑定.
    \sa         QCusFluentNavigationBar::insertPane
*/
QAction* QCusFluentNavigationBar::insertPane(int index, const QIcon& icon, const QString& text, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    auto action = insertAction(index, icon, text, pos);
    insertPane(index, action, pos, pane);
    return action;
}

/*!
    \overload   insertPane
                插入并返回一个文本为 \a text 的action，并插入到 \a index 位置，显示到 \a pos 位置，并且与 \a pane 绑定.
    \sa         QCusFluentNavigationBar::insertPane
*/
QAction* QCusFluentNavigationBar::insertPane(int index, const QString& text, ActionPosition pos, QCusFluentNavigationSidePane* pane)
{
    return insertPane(index, QIcon(), text, pos, pane);
}

/*!
    \brief      添加并返回一个图标为 \a icon 的帮助action到 \a pos 位置，点击时打开 \a filePath 的帮助文档.
    \sa         insertHelp
*/
QAction* QCusFluentNavigationBar::addHelp(const QUrl& filePath, const QIcon& icon, ActionPosition pos)
{
    auto index = count(pos);
    return insertHelp(index, filePath, icon, pos);
}

/*!
    \brief      插入并返回一个图标为 \a icon 的帮助action到 \a index, \a pos 位置，点击时打开 \a filePath 的帮助文档.
    \sa         addHelp
*/
QAction* QCusFluentNavigationBar::insertHelp(int index, const QUrl& filePath, const QIcon& icon, ActionPosition pos /*= Right*/)
{
    auto help = new QAction(icon, "", this);
    connect(help, &QAction::triggered, this, [=]() { QDesktopServices::openUrl(filePath); });
    insertAction(index, help, pos);
    return help;
}

/*!
    \brief      添加并返回一个图标为 \a icon, 显示位置为 \a pos 的Logo action.
    \sa         insertLogo
*/
QAction* QCusFluentNavigationBar::addLogo(const QIcon& icon, ActionPosition pos)
{
    auto index = count(pos);
    return insertLogo(index, icon, pos);
}

/*!
    \brief      插入并返回一个图标为 \a icon, 显示位置为 \a index, \a pos 的Logo action.
    \sa         addLogo
*/
QAction* QCusFluentNavigationBar::insertLogo(int index, const QIcon& icon, ActionPosition pos /*= Left*/)
{
    QWidgetAction* action = new QWidgetAction(this);
    action->setIcon(icon);
    QCusFluentNavigationImageButton* imageBtn = new QCusFluentNavigationImageButton(this);
    imageBtn->setIcon(icon);
    imageBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    action->setDefaultWidget(imageBtn);
    imageBtn->setDefaultAction(action);

    QObject::connect(action,
        &QAction::objectNameChanged,
        imageBtn,
        static_cast<void (QObject::*)(const QString&)>(&QCusFluentNavigationImageButton::setObjectName));
    insertAction(index, action, pos);
    return action;
}

/*!
    \brief      添加并返回一个图标为 \a icon, 文本为 \a text, 显示位置为 \a pos 的用户action.
    \sa         insertUser
*/
QAction* QCusFluentNavigationBar::addUser(const QIcon& icon, const QString& text, ActionPosition pos)
{
    return insertUser(count(pos), icon, text, pos);
}

/*!
    \brief      插入并返回一个图标为 \a icon, 文本为 \a text, 显示位置为 \a index, \a pos 的用户action.
    \sa         addUser
*/
QAction* QCusFluentNavigationBar::insertUser(int index, const QIcon& icon, const QString& text, ActionPosition pos)
{
    QWidgetAction* action = new QWidgetAction(this);
    QCusFluentUserButton* btn = new QCusFluentUserButton;
    action->setDefaultWidget(btn);
    btn->setDefaultAction(action);

    action->setText(text);
    action->setIcon(icon);
    btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    connect(action, &QAction::objectNameChanged, btn, static_cast<void (QObject::*)(const QString&)>(&QObject::setObjectName));
    insertAction(index, action, pos);
    return action;
}

/*!
    \brief      返回 \a action 的序号.
*/
int QCusFluentNavigationBar::indexOf(QAction* action) const
{
    auto item = m_impl->find(action, Left);
    if (item)
        return m_impl->leftItems.indexOf(item);
    item = m_impl->find(action, Center);
    if (item)
        return m_impl->centerItems.indexOf(item);
    item = m_impl->find(action, Right);
    if (item)
        return m_impl->rightItems.indexOf(item);
    return -1;
}

/*!
    \brief      返回 \a pos 位置的action数量.
*/
int QCusFluentNavigationBar::count(ActionPosition pos) const
{
    switch (pos)
    {
    case ActionPosition::Left:
        return m_impl->leftItems.size();
    case ActionPosition::Center:
        return m_impl->centerItems.size();
    case ActionPosition::Right:
        return m_impl->rightItems.size();
    }
    return 0;
}

/*!
    \brief      返回 \a action 的区域.
*/
QRect QCusFluentNavigationBar::actionRect(QAction* action)
{
    auto item = m_impl->find(action);
    if (!item)
        return QRect();
    return item->widget()->geometry();
}

/*!
    \brief      返回 \a pos \a index 位置的action.
    \sa         addAction, insertAction
*/
QAction* QCusFluentNavigationBar::actionAt(int index, ActionPosition pos) const
{
    if (index < 0)
        return nullptr;
    auto findAction = [index](const QList<QCusFluentWidgetItemPtr>& items) -> QAction*
    {
        if (items.size() > index)
            return items.at(index)->action();
        else
            return nullptr;
    };
    switch (pos)
    {
    case QCusFluentNavigationBar::Left:
        return findAction(m_impl->leftItems);
    case QCusFluentNavigationBar::Center:
        return findAction(m_impl->centerItems);
    case QCusFluentNavigationBar::Right:
        return findAction(m_impl->rightItems);
    default:
        break;
    }
    return nullptr;
}

/*!
    \brief      设置Action图标的大小 \a size.
    \sa         iconSize
*/
void QCusFluentNavigationBar::setIconSize(const QSize& size)
{
    m_impl->iconSize = size;
    emit iconSizeChanged(m_impl->iconSize);
}

/*!
    \brief      返回Action图标的大小.
    \sa         setIconSize
*/
const QSize& QCusFluentNavigationBar::iconSize() const { return m_impl->iconSize; }

/*!
    \reimp
*/
void QCusFluentNavigationBar::actionEvent(QActionEvent* event)
{
    ActionPosition pos = Left;
    if (event->action()->property(ActionPosProperty).isValid())
    {
        pos = static_cast<ActionPosition>(event->action()->property(ActionPosProperty).toInt());
    }

    if (event->type() == QEvent::ActionAdded)
    {
        auto item = std::make_shared<QCusFluentWidgetItem>(event->action(), Qt::Horizontal, m_impl->iconSize, this);
        connect(this, &QCusFluentNavigationBar::iconSizeChanged, item.get(), &QCusFluentWidgetItem::iconSizeChanged);
        switch (pos)
        {
        case Left:
            CusFluentUtil::addItem(item, m_impl->leftItems, event->before(), m_impl->leftLayout);
            break;
        case Center:
            CusFluentUtil::addItem(item, m_impl->centerItems, event->before(), m_impl->centerLayout);
            break;
        case Right:
            CusFluentUtil::addItem(item, m_impl->rightItems, event->before(), m_impl->rightLayout);
            break;
        }
    }
    else if (event->type() == QEvent::ActionRemoved)
    {
        auto item = m_impl->find(event->action(), pos);
        if (item->isCustomWidget())
        {
            item->widget()->hide();
            item->widget()->setParent(nullptr);
        }
        switch (pos)
        {
        case Left:
            m_impl->leftLayout->takeAt(m_impl->leftLayout->indexOf(item->widget()));
            m_impl->leftItems.removeOne(item);
            break;
        case Center:
            m_impl->centerLayout->takeAt(m_impl->centerLayout->indexOf(item->widget()));
            m_impl->centerItems.removeOne(item);
            break;
        case Right:
            m_impl->rightLayout->takeAt(m_impl->rightLayout->indexOf(item->widget()));
            m_impl->rightItems.removeOne(item);
            break;
        }
    }
}