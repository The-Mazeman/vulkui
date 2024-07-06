#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"
#include "parserWav.hpp"
#include "button.hpp"
#include "titleBarWindow.hpp"
#include "rulerWindow.hpp"
#include "trackHeaderAreaWindow.hpp"
#include "clipAreaWindow.hpp"
#include "globalState.hpp"
#include "audioClip.hpp"

START_SCOPE(mainWindow)

struct State
{
    uint backgroundEntityTag;
    uint foregroundEntityTag;
    uint rulerWindowTag;
    uint focusedWindowTag;
    uint contextMenuWindowTag;
    uint titleBarWindowTag;
    uint trackHeaderAreaWindowTag;
    uint clipAreaWindowTag;
    GlobalState globalState;
};

void create(Window* window);

END_SCOPE

