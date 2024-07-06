#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "globalState.hpp"
#include "renderer.hpp"

START_SCOPE(rulerWindow)

struct State
{
    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint textEntityTag;
    uint pad;
};

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
