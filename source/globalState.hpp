#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"

struct GlobalState
{
    uint framesPerPixel;
    uint sampleRate;
    uint trackCount;
    uint audioEndpointFrameCount;
    uint offsetX;
    uint offsetY;
    uint64 readCursor;
    uint trackHeaderWindowTags[32];
};

START_SCOPE(globalState)

void get(HWND windowHandle, GlobalState** globalState);

END_SCOPE
