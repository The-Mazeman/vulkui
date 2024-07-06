#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"

struct Header
{
	uint16 type;
	uint16 channelCount;
	uint32 sampleRate;
	uint32 byteRate;
	uint16 blockAlign;
	uint16 bitDepth;
};
struct WavFile
{
	Header header;
	uint64 frameCount;
	void* sampleChunk;
};

START_SCOPE(parserWav)

void parse(void* wavFilePointer, WavFile* wavFile);

END_SCOPE
