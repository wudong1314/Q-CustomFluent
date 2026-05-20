
#pragma once

#include "QCusFluentNavigationSidePane.h"


class QCusFluentAbstractMessageTipModel;
class QCusFluentNavigationBar;

class QCusFluentAbstractMessageTipView : public QCusFluentNavigationSidePane
{
    Q_OBJECT

public:
    QCusFluentAbstractMessageTipView(QCusFluentNavigationBar* parent);
    ~QCusFluentAbstractMessageTipView();
    virtual void setModel(QCusFluentAbstractMessageTipModel* model) = 0;
    virtual QCusFluentAbstractMessageTipModel* model() const        = 0;
};
