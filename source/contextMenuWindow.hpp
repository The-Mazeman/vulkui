#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"

START_SCOPE(contextMenuWindow)

struct State
{   
    String* menuOptions;
    uint optionCount;

    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint visibility;
};

void create(void* renderer, String* menuOptions, uint optionCount, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
