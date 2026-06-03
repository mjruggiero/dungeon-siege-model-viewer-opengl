
#ifndef _RAWFILE_H_
#define _RAWFILE_H_

#include <string.h>

typedef struct
{
	char magic[4];
	int format;
	short flags;
	short surfaceCount;
	short width;
	short height;
} rawheader_t;

char *LoadRawFile(const char *filename, int &width, int &height);

#endif // _RAWFILE_H_