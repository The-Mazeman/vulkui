
#include "include.hpp"
#include "parserFontMetrics.hpp"

START_SCOPE(parserFontMetrics)

void parse(void* file, GlyphInfo** glyphInfo, KerningInfo** kerningInfo)
{
    uint8* metricFile = (uint8*)file;
    uint offset = {};
    while(1)
    {
        metricFile += offset;
        switch(*metricFile)
        {
            case 66:
            {
                assert(*(metricFile + 1) == 77);
                assert(*(metricFile + 2) == 70);
                assert(*(metricFile + 3) == 3);
                offset = 4;
                break;
            }
            case 1:
            {
                int* blockSize = (int*)(metricFile + 1);
                MetricInfo* metricInfo = (MetricInfo*)(metricFile + 5);
                offset = (uint)(*blockSize + 5);
                break;

            }
            case 2:
            {
                int* blockSize = (int*)(metricFile + 1);
                FontInfo* fontInfo = (FontInfo*)(metricFile + 5);
                offset = (uint)(*blockSize + 5);
                break;
            }
            case 3:
            {
                int* blockSize = (int*)(metricFile + 1);
                PageInfo* page = (PageInfo*)(metricFile + 5);
                offset = (uint)(*blockSize + 5);
                break;
            }
            case 4:
            {
                int* blockSize = (int*)(metricFile + 1);
                *glyphInfo = (GlyphInfo*)(metricFile + 5);
                offset = (uint)(*blockSize + 5);
                break;

            }
            case 5:
            {
                int* blockSize = (int*)(metricFile + 1);
                *kerningInfo = (KerningInfo*)(metricFile + 5);
                offset = (uint)(*blockSize + 5);
                return;
                break;

            }
        }
    }

}

END_SCOPE
