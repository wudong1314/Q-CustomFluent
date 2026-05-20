#include "QCusFluentMessageTipButton.h"
#include "QCusFluentMessageTipHelper_p.h"
#include "QCusFluentAbstractMessageTipModel.h"
#include "QCusFluentMessageTipView.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOption>

struct QCusFluentMessageTipButton::Impl
{
    QCusFluentAbstractMessageTipModel* model { nullptr };
    QCusFluentAbstractMessageTipView* view { nullptr };
    QCusFluentMessageTipHelper* helper { nullptr };
};

/*!
    \class      QCusFluentMessageTipButton
    \brief      消息提示按钮.
    \inherits   QAbstractButton
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCusFluentMessageTipButton.h
*/

/*!
    \property   QCusFluentMessageTipButton::tipColor
    \brief      消息提示数量的颜色.
*/

/*!
    \brief      构造一个父对象为 \a parent 的消息提示按钮.
*/
QCusFluentMessageTipButton::QCusFluentMessageTipButton(QWidget* parent) : QAbstractButton(parent), m_impl(std::make_unique<Impl>())
{
    m_impl->helper = new QCusFluentMessageTipHelper(this);
    connect(this, &QAbstractButton::clicked, this, &QCusFluentMessageTipButton::onClicked);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFocusPolicy(Qt::NoFocus);
    setIcon(QIcon(":/QCustomUi/Resources/messageTip-light.png"));
    setTipColor(0xF54545);
    auto pal = this->palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);
}

/*!
    \brief      销毁该消息提示按钮对象.
*/
QCusFluentMessageTipButton::~QCusFluentMessageTipButton() {}

/*!
    \brief      设置消息提示按钮的数据来源 \a model.
    \sa         model()
*/
void QCusFluentMessageTipButton::setModel(QCusFluentAbstractMessageTipModel* model)
{
    if (m_impl->model == model)
        return;
    if (m_impl->model)
        m_impl->model->deleteLater();
    m_impl->model = model;
    connect(m_impl->model, &QAbstractItemModel::rowsRemoved, this, &QCusFluentMessageTipButton::onModelDataChanged);
    connect(m_impl->model, &QAbstractItemModel::rowsInserted, this, &QCusFluentMessageTipButton::onModelDataChanged);
    connect(m_impl->model, &QAbstractItemModel::modelReset, this, &QCusFluentMessageTipButton::onModelDataChanged);
    connectView();
    if (this->isVisible())
    {
        update();
    }
}

/*!
    \brief      返回消息提示按钮的数据来源 model.
    \sa         setModel
*/
QCusFluentAbstractMessageTipModel* QCusFluentMessageTipButton::model() const { return m_impl->model; }

/*!
    \brief      设置消息提示按钮的拓展列表视图 \a view.
    \sa         view()
*/
void QCusFluentMessageTipButton::setView(QCusFluentAbstractMessageTipView* view)
{
    m_impl->view = view;
    connectView();
}

/*!
    \brief      返回消息提示按钮的拓展列表视图.
    \sa         setView
*/
QCusFluentAbstractMessageTipView* QCusFluentMessageTipButton::view() const { return m_impl->view; }

/*!
    \brief      设置消息提示数量的颜色 \a color.
    \sa         tipColor()
*/
void QCusFluentMessageTipButton::setTipColor(const QColor& color) { m_impl->helper->setTipColor(color); }

/*!
    \brief      返回消息提示数量的颜色.
    \sa         setTipColor
*/
const QColor& QCusFluentMessageTipButton::tipColor() const { return m_impl->helper->tipColor(); }

/*!
    \reimp
*/
void QCusFluentMessageTipButton::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    QStyleOptionButton opt;
    opt.initFrom(this);
    initStyleOption(&opt);

    QPainter p(this);

    this->style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);

    if (m_impl->model)
    {
        m_impl->helper->drawMessageTip(m_impl->model->rowCount(), p, this);
    }
}

/*!
    \reimp
*/
QSize QCusFluentMessageTipButton::sizeHint() const
{
    auto&& size = QAbstractButton::sizeHint();
    size.setWidth(this->iconSize().width() + 20);
    return size;
}

/*!
    \brief      连接 model 和视图.
*/
void QCusFluentMessageTipButton::connectView()
{
    if (m_impl->view && m_impl->model)
        m_impl->view->setModel(m_impl->model);
}

/*!
    \brief      初始化样式选项 \a opt.
*/
void QCusFluentMessageTipButton::initStyleOption(QStyleOptionButton* opt)
{
    opt->icon     = this->icon();
    opt->iconSize = this->iconSize();
    opt->features = QStyleOptionButton::DefaultButton;

    if (this->isDown())
    {
        opt->state = QStyle::State_Raised | QStyle::State_Sunken;
    }
    if (this->isChecked())
        opt->state = QStyle::State_On;
}

/*!
    \brief      响应点击操作.
*/
void QCusFluentMessageTipButton::onClicked(bool)
{
    if (this->view())
    {
        if (this->view()->isVisible())
            this->view()->close();
        else
            this->view()->show();
    }
}

/*!
    \brief      消息变化时更新界面.
*/
void QCusFluentMessageTipButton::onModelDataChanged() { update(); }
