
#pragma once

#include <QAbstractTableModel>

#include <memory>

using QCusFluentAbstractMessageTipDataPtr = std::shared_ptr<class QCusFluentAbstractMessageTipData>;

class QCusFluentAbstractMessageTipModel : public QAbstractTableModel
{
    Q_OBJECT
        Q_PROPERTY(bool reversedOrder READ reversedOrder WRITE setReversedOrder)
public:
    QCusFluentAbstractMessageTipModel(QObject* parent);
    ~QCusFluentAbstractMessageTipModel();

    void addMessage(QCusFluentAbstractMessageTipDataPtr msg);
    void insertMessage(int index, QCusFluentAbstractMessageTipDataPtr msg);
    void removeMessage(QCusFluentAbstractMessageTipDataPtr msg);
    QCusFluentAbstractMessageTipDataPtr message(int row) const;
    void clear();
    void setMaximumCount(int count);
    int maximumCount() const;
    void setReversedOrder(bool re);
    bool reversedOrder() const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData([[maybe_unused]] const QModelIndex& index,
        [[maybe_unused]] const QVariant& value,
        [[maybe_unused]] int role = Qt::EditRole) override
    {
        return false;
    }
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
