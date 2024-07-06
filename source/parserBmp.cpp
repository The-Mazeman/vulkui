#include "include.hpp"
#include "parserBmp.hpp"

START_SCOPE(parserBmp)

void parse(void* file, void** pixel)
{
    uint8* bmpFile = (uint8*)file;
    FileHeader* header = (FileHeader*)bmpFile;
    BitmapInfo* bitmapInfo = (BitmapInfo*)(bmpFile + 14); 

    *pixel = (void*)(bmpFile + header->pixelArrayOffset);
}

END_SCOPE
