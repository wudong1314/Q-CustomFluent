#pragma once

#include <memory>

#include "QCusFluentToolButton_p.h"

class QStyleOptionToolButton;

class QCusFluentUserButton : public QCusFluentToolButton
{
    Q_OBJECT
public:
    QCusFluentUserButton(QWidget* parent = nullptr);
    ~QCusFluentUserButton();

protected:
    void paintEvent(QPaintEvent*) override;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QRect doTextRect(const QStyleOptionToolButton& opt) const;
    QRect doIconRect(const QStyleOptionToolButton& opt) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
