#include "mainwindow.h"
#include <QHBoxLayout>
#include "NavigationWidget.h"
#include "HomeInterface.h"
#include "IconInterface.h"
#include "Router.h"
#include "Fluent/BasicInputInterface.h"

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
    addSubInterface("1", Fluent::icon(FIT::HOME), "��ҳ", new HomeInterface(this), true, NIP::TOP, "��ҳ");
    addSubInterface("2", Fluent::icon(FIT::EMOJI_TAB_SYMBOLS), "ͼ��", new IconInterface(this), true, NIP::TOP, "ͼ��");
    _navPanel->addSeparator();
    addSubInterface("3", Fluent::icon(FIT::CHECKBOX), "��������", new BasicInputInterface(this), true, NIP::SCROLL, "��������");
   /* addSubInterface("4", Fluent::icon(FIT::DATE_TIME), "���ں�ʱ��", new DateTimeInputInterface(this), true, NIP::SCROLL, "���ں�ʱ��");
    addSubInterface("5", Fluent::icon(FIT::MESSAGE), "�Ի���", new DialogInputInterface(this), true, NIP::SCROLL, "�Ի���");
    addSubInterface("6", Fluent::icon(FIT::LAYOUT), "����", new LayoutInterface(this), true, NIP::SCROLL, "����");
    addSubInterface("7", Fluent::icon(iconPath.arg("Menu")), "�˵�", new MenuInterface(this), true, NIP::SCROLL, "�˵�");
    addSubInterface("8", Fluent::icon(FIT::MENU), "����", new NavigationViewInterface(this), true, NIP::SCROLL, "����");
    addSubInterface("9", Fluent::icon(FIT::APPLICATION), "����", new WindowInterface(this), true, NIP::SCROLL, "����");
    addSubInterface("10", Fluent::icon(FIT::PALETTE), "����", new MaterialInterface(this), true, NIP::SCROLL, "����");
    addSubInterface("11", Fluent::icon(FIT::CHAT), "״̬", new StatusInfoInterface(this), true, NIP::SCROLL, "״̬");
    addSubInterface("12", Fluent::icon(iconPath.arg("Text")), "�ı�", new TextInterface(this), true, NIP::SCROLL, "�ı�");
    addSubInterface("13", Fluent::icon(iconPath.arg("Grid")), "��ͼ", new ViewInterface(this), true, NIP::SCROLL, "��ͼ");*/

    _navPanel->addSeparator(NIP::BOTTOM);
   // addSubInterface("14", Fluent::icon(FIT::SETTING), "����", new SettingInterface(this), true, NIP::BOTTOM, "����");

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
