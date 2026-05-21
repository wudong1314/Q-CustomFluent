#pragma once

#include <QPushButton>

class QCusFluentMaximumSizeButton : public QPushButton
{
    Q_OBJECT
      //Q_PROPERTY(bool maximumsized READ maximumSized WRITE setMaximumSized)
public:
    QCusFluentMaximumSizeButton(QWidget* parent);
    ~QCusFluentMaximumSizeButton();

    void setMaximumSized(bool max);
    bool maximumSized() const;

private:
    bool m_maxsized;
};
