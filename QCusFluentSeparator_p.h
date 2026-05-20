#pragma once

#include <QStyleOption>
#include <QWidget>

class QCusFluentSeparator : public QWidget
{
    Q_OBJECT

public:
    QCusFluentSeparator(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QCusFluentSeparator();

    Qt::Orientation orientation() const;

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent*) override;
    void initStyleOption(QStyleOption* opt) const;

public slots:
    void setOrientation(Qt::Orientation orientation);

private:
    Qt::Orientation m_orientation;
};
