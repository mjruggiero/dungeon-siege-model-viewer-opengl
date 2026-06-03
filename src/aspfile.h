
#ifndef _ASPFILE_H_
#define _ASPFILE_H_

#include "math.h"
#include "rawfile.h"

#pragma pack(push,1)

typedef struct
{
	char major;
	char minor;
	char null[2];
} version_t;

typedef struct
{
	int sizeTextField;
	int numBones;
	int numMaterials;
	int numVertices;
	int numSubmeshes;
	int unknown;
	char *textField;
} bmsh_t;

typedef struct
{
	int index;
	int parentIndex;
	int nameOffset;
} bonh_t;

typedef struct
{
	int id;
	int numMaterials;
	int numVertices;
	int numCorners;
	int numFaces;
} bsub_t;

typedef struct
{
	int index;
	int numFaces;
} material_t;

typedef struct
{
	int numMaterials;
	material_t *materials;
} bsmm_t;

typedef struct
{
	int numVertices;
	vector_t *vertices;
} bvtx_t;

typedef struct
{
	int vertexIndex;
	vector_t normal;
	int unknown1;
	int unknown2;
	float u, v;
} corner_t;

typedef struct
{
	int numCorners;
	corner_t *corners;
} bcrn_t;

typedef struct
{
	vector_t position;
	float boneWeight0;
	float boneWeight1;
	float boneWeight2;
	float boneWeight3;
	unsigned char boneIndex0;
	unsigned char boneIndex1;
	unsigned char boneIndex2;
	unsigned char boneIndex3;
	vector_t normal;
	int unknown;
	float u, v;
} wcorner_t;

typedef struct
{
	int numCorners;
	wcorner_t *corners;
} wcrn_t;

typedef struct
{
	int numCorners;
	int *cornerIndices;
} vertexMap_t;

typedef struct
{
	vertexMap_t *vertexMaps;
} bvmp_t;

typedef struct
{
	int indexCornerA;
	int indexCornerB;
	int indexCornerC;
} face_t;

typedef struct
{
	int numFaces;
	material_t *materials;
	face_t *faces;
} btri_t;

typedef struct
{
	int index;
	float weighting;
} affectedVertex_t;

typedef struct
{
	int numVertices;
	affectedVertex_t *vertices;
} bone_t;

typedef struct
{
	bone_t *bones;
} bvwl_t;

typedef struct
{
	char token[4];
	int numVertices;
	int *vertices;
} stitch_t;

typedef struct
{
	int numStitches;
	stitch_t *stitches;
} stch_t;

typedef struct
{
	quaternion_t inverseRotation;
	vector_t inverseTranslation; 
	quaternion_t localRotation;
	vector_t localTranslation; 
} position_t;

typedef struct
{
	int numBones;
	position_t *positions;
} rpos_t;

typedef struct
{
	int unknown;
} bbox_t;

typedef struct
{
	
} bend_t;

typedef struct
{
	int numInfos;
	char *info;
} info_t;

#pragma pack(pop)

#endif // _ASPFILE_H_
