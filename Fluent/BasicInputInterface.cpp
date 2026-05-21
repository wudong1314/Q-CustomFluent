#include "Fluent/BasicInputInterface.h"
#include <QLabel>

BasicInputInterface::BasicInputInterface(QWidget* parent)
    : QWidget(parent)
{
    // placeholder simple UI
    auto *lbl = new QLabel(tr("Basic Input (placeholder)"), this);
    lbl->setAlignment(Qt::AlignCenter);
}
