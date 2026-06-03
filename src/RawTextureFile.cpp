#include "RawTextureFile.h"

#include "Log.h"

#include <stdio.h>

char* LoadRawFile(const char* filename, int& width, int& height)
{
	FILE* pFile = fopen(filename, "rb");
	if (!pFile)
	{
		Log::Error() << "Couldn't find file: " << filename << std::endl;
		return nullptr;
	}

	rawheader_t header;
	fread(&header, sizeof(rawheader_t), 1, pFile);

	width = header.width;
	height = header.height;

	if (strncmp(header.magic, "ipaR", 4) != 0)
	{
		fclose(pFile);
		return nullptr;
	}

	if (header.format != '8888')
	{
		fclose(pFile);
		return nullptr;
	}

	char* buffer = new char[header.width * header.height * 4];

	fread(buffer, sizeof(char), header.width * header.height * 4, pFile);

	fclose(pFile);

	return buffer;
}
