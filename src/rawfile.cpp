#include "rawfile.h"

#include "Log.h"

#include <stdio.h>

using namespace std;

char* LoadRawFile(const char* filename, int& width, int& height)
{
	FILE* pFile;

	char path[256];
	//strcpy(path, "data/");
	strcpy(path, filename);
	pFile = fopen(path, "rb");
	if (!pFile)
	{
		Log::Error() << "Couldn't find file: " << filename << endl;
		return NULL;
	}

	rawheader_t header;
	fread(&header, sizeof(rawheader_t), 1, pFile);

	width = header.width;
	height = header.height;

	if (strncmp(header.magic, "ipaR", 4) != 0)
		return NULL;
	if (header.format != '8888')
		return NULL;

	char* buffer = new char[header.width * header.height * 4];

	fread(buffer, sizeof(char), header.width * header.height * 4, pFile);

	fclose(pFile);

	return buffer;
}
