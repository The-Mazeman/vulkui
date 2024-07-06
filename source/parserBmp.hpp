#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"

START_SCOPE(parserBmp)

#pragma pack(2)
struct FileHeader
{
    ushort tag;
    uint fileSize;
    ushort pad1;
    ushort pad2;
    uint pixelArrayOffset;
    ushort pad3;
};
struct BitmapInfo
{
    uint headerSize;
    uint width;
    uint height;
    ushort colorPlanes;
    ushort bitDepth;
    uint compression;
    uint rawSize;
    uint horizontalSize;
    uint verticalSize;
    uint colorPalatteSize;
    uint pad;
};
struct Pixel
{
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;
};
void parse(void* file, void** pixel);

END_SCOPE
