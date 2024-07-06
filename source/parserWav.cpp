#include "include.hpp"
#include "parserWav.hpp"

START_SCOPE(parserWav)

void parse(void* wavFilePointer, WavFile* wavFile)
{
	while (1)
	{
		uint tagValue;
		memcpy(&tagValue, wavFilePointer, 4);
		switch (tagValue)
		{
			case 1179011410: // Riff tag value 
			{
				wavFilePointer = (char*)wavFilePointer + 8;
				break;
			}
			case 1163280727: // Wave tag value
			{
				wavFilePointer = (char*)wavFilePointer + 4;
				break;
			}
			case 544501094: // Format tag value
			{
				uint formatChunkSize;
				memcpy(&formatChunkSize, (char*)wavFilePointer + 4, 4);
				memcpy(wavFile, (char*)wavFilePointer + 8, 16);
				wavFilePointer = (char*)wavFilePointer + formatChunkSize + 8ull;
				break;
			}
			case 1635017060: // Data tag value
			{
				uint dataChunkSize;
				memcpy(&dataChunkSize, (char*)wavFilePointer + 4, 4);
				wavFile->sampleChunk = (float*)((char*)wavFilePointer + 8);
				wavFile->frameCount = dataChunkSize / wavFile->header.blockAlign;
				return;
			}
			default:
			{
				uint unknownChunkSize;
				memcpy(&unknownChunkSize, (char*)wavFilePointer + 4, 4);
				wavFilePointer = (char*)wavFilePointer + unknownChunkSize + 8ull;
			}
		}
	}
}
END_SCOPE
