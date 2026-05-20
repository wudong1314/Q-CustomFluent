#pragma once

#include "ToolTip.h"

#include "FluentGlobal.h"

class NavigationToolTipFilter : public ToolTipFilter
{
public:
    using ToolTipFilter::ToolTipFilter;

protected:
    bool canShowToolTip() const override;
};
