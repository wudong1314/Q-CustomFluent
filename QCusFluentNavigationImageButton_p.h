
#pragma once

#include "QCusFluentToolButton_p.h"

#include <memory>

class QCusFluentNavigationImageButton : public QCusFluentToolButton
{
    Q_OBJECT

public:
    QCusFluentNavigationImageButton(QWidget* parent);
    ~QCusFluentNavigationImageButton();

protected:
    void paintEvent(QPaintEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;
    QSize sizeHint() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
