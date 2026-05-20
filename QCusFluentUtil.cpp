
#include "QCusFluentWidgetItem_p.h"
#include "QCusFluentUtil_p.h"

QCusFluentWidgetItemPtr CusFluentUtil::find(QAction* action, const QList<QCusFluentWidgetItemPtr>& items)
{
    for (auto& item : items)
    {
        if (item->action() == action)
            return item;
    }
    return nullptr;
}

QCusFluentWidgetItemPtr CusFluentUtil::find(QWidget* widget, const QList<QCusFluentWidgetItemPtr>& items)
{
    for (auto& item : items)
    {
        if (item->widget() == widget)
            return item;
    }
    return nullptr;
}
