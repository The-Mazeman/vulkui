#pragma once

#include "dataTypes.hpp"
#include "defines.hpp"
#include "dataStructures.hpp"
#include "platformWindows.hpp"
#include "parserWav.hpp"

struct AudioClip
{
    WavFile wavFile;
    String filePath;
    uint64 waveformFrameCount;
    sint8* waveformData;
};

START_SCOPE(audioClip)

void create(String* filePath, sint8* waveformData,  uint64 startFrame, AudioClip* audioClipArray);

END_SCOPE

