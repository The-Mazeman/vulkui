#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"

START_SCOPE(button)

struct State
{
    uint windowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint textEntityTag;
    uint parentWindowTag;
    uint pwd;
};

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
