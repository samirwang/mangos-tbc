#pragma once

#include "Common.h"
#include "Singleton.h"

class Custom : public CSingleton<Custom>
{
public:
    Custom();
    ~Custom();

    void LoadFlightPaths();

    uint32 GetAutoItemPrice(uint32 itemid);
};

#define sCustom Custom::Instance()
