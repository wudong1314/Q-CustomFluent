#pragma once

#include <QAction>

#include <memory>

class QCusFluentWidgetItem;
typedef std::shared_ptr<QCusFluentWidgetItem> QCusFluentWidgetItemPtr;

namespace CusFluentUtil
{
QCusFluentWidgetItemPtr find(QAction* action, const QList<QCusFluentWidgetItemPtr>& items);
QCusFluentWidgetItemPtr find(QWidget* widget, const QList<QCusFluentWidgetItemPtr>& items);

template<class Layout, class Item>
void addItem(Item item, QList<Item>& items, QAction* before, Layout* layout)
{
    auto it    = std::find_if(items.begin(), items.end(), [=](Item item) { return item->action() == before; });
    it         = items.insert(it, item);
    auto index = items.indexOf(*it);
    layout->insertWidget(index, item->widget());
    if (item->isCustomWidget())
    {
        item->widget()->setVisible(item->action()->isVisible());
    }
};

template<class Layout, class Item>
void removeItem(QAction* action, QList<Item>& items, Layout* layout)
{
    auto item = CusFluentUtil::find(action, items);
    items.removeOne(item);
    layout->takeAt(layout->indexOf(item->widget()));
}
} // namespace 
