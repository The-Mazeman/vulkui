#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"


#pragma pack(1)
struct MetricInfo
{
    sint16 fontSize;
    uint8 bitField;
    uint8 charSet;
    uint16 strechHeight;
    uint8 aliasing;
    uint8 paddingUp;
    uint8 paddingRight;
    uint8 paddingDown;
    uint8 paddingLeft;
    uint8 horizontalSpace;
    uint8 verticalSpace;
    uint8 outline;
    char* fontName;
};

#pragma pack(1)
struct FontInfo
{
    uint16 lineHeight;
    uint16 baseLine;
    uint16 scaleWidth;
    uint16 scaleHeight;
    uint16 pages;
    uint8 bitField;
    uint8 alpha;
    uint8 red;
    uint8 green;
    uint8 blue;

};

#pragma pack(1)
struct PageInfo
{
    uint8* pageNames;
};

#pragma pack(1)
struct GlyphInfo
{
    uint32 id;
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    sint16 offsetX;
    sint16 offsetY;
    sint16 advanceX;
    uint8 page;
    uint8 channel;
};
#pragma pack(1)
struct KerningInfo
{
    uint32 firstId;
    uint32 secondId;
    sint16 offsetX;
};

START_SCOPE(parserFontMetrics)

void parse(void* file, GlyphInfo** glyphInfo, KerningInfo** kerningInfo);

END_SCOPE
