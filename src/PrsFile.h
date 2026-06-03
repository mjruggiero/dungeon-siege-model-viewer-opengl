#pragma once

#pragma pack(push,1)

typedef struct
{
	int sizeTextField;
	int numBones;
	float duration;
	vector_t unknownVector;
	quaternion_t unknownQuaternion1;
	quaternion_t unknownQuaternion2;
	float unknown;
	char* textField;
} anim_t;

typedef struct
{
	float time;
	char token[4];
} noteInfo_t;

typedef struct
{
	int numNotes;
	noteInfo_t* notes;
} note_t;

typedef struct
{
	float unknown[12];
} itemInfo_t;

typedef struct
{
	int numItems;
	itemInfo_t* items;
	int unknown;
} trcr_t;

typedef struct
{
	float time;
	quaternion_t rotation;
} rotationKey_t;

typedef struct
{
	float time;
	vector_t position;
} positionKey_t;

typedef struct
{
	int numRotations;
	int numPositions;
	rotationKey_t* rotations;
	positionKey_t* positions;
} keyframes_t;

typedef struct
{
	keyframes_t keyframes;
} rkey_t;

typedef struct
{
	int boneIndex;
	int offsetBoneName;
	keyframes_t keyframes;
} klst_t;

typedef struct
{

} aend_t;

#pragma pack(pop)
