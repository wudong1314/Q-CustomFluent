#pragma once

#include "QCusFluentWindow.h"
#include "NavigationPanel.h"
#include "StackedWidget.h"
class MainWindow : public QCusFluentWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    void setCurrentInterface(const QString& routeKey, int index);

    void addSubInterface(const QString& routeKey, const QIcon& icon, const QString& text,
        QWidget* widget, bool selectable = true,
        NavigationPanel::ItemPosition position = NavigationPanel::ItemPosition::TOP,
        const QString& tooltip = QString(), const QString& parentRouteKey = QString());

protected:
    void initWidget();
protected:
    NavigationPanel* _navPanel;
    StackedWidget* _stacked;
};

