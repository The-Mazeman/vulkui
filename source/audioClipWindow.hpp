#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "renderer.hpp"
#include "parserWav.hpp"
#include "audioClip.hpp"
#include "globalState.hpp"

struct State
{
    uint windowTag;
    uint parentWindowTag;
    uint foregroundEntityTag;
    uint backgroundEntityTag;
    AudioClip audioClip;
    uint userIsDragging;
    uint mouseRelativeX;
};

START_SCOPE(audioClipWindow)

void create(void* renderer, Window* window, AudioClip* audioClip, uint parentWindowTag, uint* selfWindowTag);

END_SCOPE

