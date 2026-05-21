#include "mainwindow.h"
#include <QHBoxLayout>
#include "NavigationWidget.h"
#include "HomeInterface.h"
#include "IconInterface.h"
#include "Router.h"

using FIT = Fluent::IconType;
using NIP = NavigationPanel::ItemPosition;

MainWindow::MainWindow()
    : QCusFluentWindow()
{
    setWindowTitle("");
    setWindowIcon(QPixmap(":/res/example.png"));
    resize(1024, 768);
    
    //setWindowButtonHints(WindowButtonHint::WindowIcon | WindowButtonHint::Title |
    //    WindowButtonHint::Minimize | WindowButtonHint::Maximize |
    //    WindowButtonHint::Close | WindowButtonHint::ThemeToggle |
    //    WindowButtonHint::RouteBack);

    QHBoxLayout* layout = new QHBoxLayout(centralWidget());
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    _navPanel = new NavigationPanel(this);
    _stacked = new StackedWidget(this);

    layout->addWidget(_navPanel, 0);
    layout->addWidget(_stacked, 1);

    _navPanel->setExpandWidth(240);
    setContentsMargins(0, 0, 0, 0);
    initWidget();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWidget()
{
    auto userCard = _navPanel->addUserCard("userCard", ":/res/Shizuka.png", "Shizuka", "shizuka@gmail.com",
        nullptr, NIP::TOP, false);
    userCard->setTitleFontSize(12);
    userCard->setSubtitleFontSize(10);

    QString iconPath = QString(":/res/icons/%1_{color}.svg");
    addSubInterface("1", Fluent::icon(FIT::HOME), "主页", new HomeInterface(this), true, NIP::TOP, "主页");
    addSubInterface("2", Fluent::icon(FIT::EMOJI_TAB_SYMBOLS), "图标", new IconInterface(this), true, NIP::TOP, "图标");
    _navPanel->addSeparator();
   // addSubInterface("3", Fluent::icon(FIT::CHECKBOX), "基本输入", new BasicInputInterface(this), true, NIP::SCROLL, "基本输入");
   /* addSubInterface("4", Fluent::icon(FIT::DATE_TIME), "日期和时间", new DateTimeInputInterface(this), true, NIP::SCROLL, "日期和时间");
    addSubInterface("5", Fluent::icon(FIT::MESSAGE), "对话框", new DialogInputInterface(this), true, NIP::SCROLL, "对话框");
    addSubInterface("6", Fluent::icon(FIT::LAYOUT), "布局", new LayoutInterface(this), true, NIP::SCROLL, "布局");
    addSubInterface("7", Fluent::icon(iconPath.arg("Menu")), "菜单", new MenuInterface(this), true, NIP::SCROLL, "菜单");
    addSubInterface("8", Fluent::icon(FIT::MENU), "导航", new NavigationViewInterface(this), true, NIP::SCROLL, "导航");
    addSubInterface("9", Fluent::icon(FIT::APPLICATION), "窗口", new WindowInterface(this), true, NIP::SCROLL, "窗口");
    addSubInterface("10", Fluent::icon(FIT::PALETTE), "材料", new MaterialInterface(this), true, NIP::SCROLL, "材料");
    addSubInterface("11", Fluent::icon(FIT::CHAT), "状态", new StatusInfoInterface(this), true, NIP::SCROLL, "状态");
    addSubInterface("12", Fluent::icon(iconPath.arg("Text")), "文本", new TextInterface(this), true, NIP::SCROLL, "文本");
    addSubInterface("13", Fluent::icon(iconPath.arg("Grid")), "视图", new ViewInterface(this), true, NIP::SCROLL, "视图");*/

    _navPanel->addSeparator(NIP::BOTTOM);
   // addSubInterface("14", Fluent::icon(FIT::SETTING), "设置", new SettingInterface(this), true, NIP::BOTTOM, "设置");

    Router::instance()->setDefaultRouteKey(_stacked, "homeInterface");

    _navPanel->setCurrentItem("1");

    connect(userCard, &NavigationUserCard::clicked, this, [this]() 
        {
       
        });
}

void MainWindow::addSubInterface(const QString& routeKey, const QIcon& icon, const QString& text,
    QWidget* widget, bool selectable,
    NavigationPanel::ItemPosition position, const QString& tooltip,
    const QString& parentRouteKey)
{
    _navPanel->addItem(routeKey, icon, text, [=]() {Router::instance()->push(_stacked, widget->objectName()); }, selectable, position, tooltip, parentRouteKey);
    _stacked->addWidget(widget);
}

void MainWindow::setCurrentInterface(const QString& routeKey, int index)
{
    Router::instance()->push(_stacked, routeKey);
    _navPanel->setCurrentItem(QString::number(index));
}
