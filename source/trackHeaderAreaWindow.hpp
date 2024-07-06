#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"
#include "contextMenuWindow.hpp"
#include "trackHeaderWindow.hpp"
#include "globalState.hpp"

START_SCOPE(trackHeaderAreaWindow)

struct State
{
    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint offsetY;
    uint contextMenuWindowTag;
    uint contextMenuVisible;
    uint firstTrackHeader;
    uint lastTrackHeader;
};

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
