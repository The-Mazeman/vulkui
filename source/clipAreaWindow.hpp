#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "globalState.hpp"
#include "renderer.hpp"
#include "audioClip.hpp"
#include "audioClipWindow.hpp"

START_SCOPE(clipAreaWindow)

struct State
{
    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    uint audioClipWindowTag;
    uint padding;
};

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE
