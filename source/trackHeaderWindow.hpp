#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"

START_SCOPE(trackHeaderWindow)

struct State
{
    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint offsetY;
    uint contextMenuTag;
};

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
