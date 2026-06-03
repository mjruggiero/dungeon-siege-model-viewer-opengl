#include "Animation.h"
#include "Log.h"

#include "AspFile.h"
#include "PrsFile.h"

#include <cstdio>
#include <cstring>

CAnimation::CAnimation()
{
	m_Keys = nullptr;
}

CAnimation::~CAnimation()
{
	if (m_Anim.textField)
		delete[] m_Anim.textField;

	if (m_Notes.notes)
		delete[] m_Notes.notes;

	if (m_Keys)
		delete[] m_Keys;
}

bool CAnimation::Load(const char* filename)
{
	FILE* pFile;
	int i;
	version_t version;

	pFile = fopen(filename, "rb");

	if (!pFile)
	{
		Log::Error() << "Couldn't load file: " << filename << std::endl;
		return false;
	}

	// read header
	char type[5];
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	Log::Debug() << "type=" << type << std::endl;
	fread(&version, sizeof(version_t), 1, pFile);
	Log::Debug() << "version.major=" << (short)version.major << std::endl;
	Log::Debug() << "version.minor=" << (short)version.minor << std::endl;
	fread(&m_Anim.sizeTextField, sizeof(int), 1, pFile);
	fread(&m_Anim.numBones, sizeof(int), 1, pFile);
	fread(&m_Anim.duration, sizeof(float), 1, pFile);
	fread(&m_Anim.unknownVector, sizeof(vector_t), 1, pFile);
	fread(&m_Anim.unknownQuaternion1, sizeof(quaternion_t), 1, pFile);
	fread(&m_Anim.unknownQuaternion2, sizeof(quaternion_t), 1, pFile);
	fread(&m_Anim.unknown, sizeof(float), 1, pFile);
	m_Anim.textField = new char[m_Anim.sizeTextField];
	fread(m_Anim.textField, sizeof(char), m_Anim.sizeTextField, pFile);
	Log::Debug() << "textField=" << m_Anim.textField << std::endl;

	// read notes
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	Log::Debug() << "type=" << type << std::endl;
	fread(&version, sizeof(version_t), 1, pFile);
	Log::Debug() << "version.major=" << (short)version.major << std::endl;
	Log::Debug() << "version.minor=" << (short)version.minor << std::endl;
	fread(&m_Notes.numNotes, sizeof(int), 1, pFile);
	m_Notes.notes = new noteInfo_t[m_Notes.numNotes];
	fread(m_Notes.notes, sizeof(noteInfo_t), m_Notes.numNotes, pFile);

	// read trcr
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	if (strcmp(type, "TRCR") == 0)
	{
		Log::Debug() << "type=" << type << std::endl;
		fread(&version, sizeof(version_t), 1, pFile);
		Log::Debug() << "version.major=" << (short)version.major << std::endl;
		Log::Debug() << "version.minor=" << (short)version.minor << std::endl;
		fread(&m_Trcr.numItems, sizeof(int), 1, pFile);
		m_Trcr.items = new itemInfo_t[m_Trcr.numItems];
		fread(m_Trcr.items, sizeof(itemInfo_t), m_Trcr.numItems, pFile);
		fseek(pFile, 8, SEEK_CUR);
		fread(&type, sizeof(char), 4, pFile);
		Log::Debug() << "type=" << type << std::endl;
	}

	// read rkey
	fread(&version, sizeof(version_t), 1, pFile);
	Log::Debug() << "version.major=" << (short)version.major << std::endl;
	Log::Debug() << "version.minor=" << (short)version.minor << std::endl;
	fread(&m_GlobalKeys.keyframes.numRotations, sizeof(int), 1, pFile);
	fread(&m_GlobalKeys.keyframes.numPositions, sizeof(int), 1, pFile);
	m_GlobalKeys.keyframes.rotations = new rotationKey_t[m_GlobalKeys.keyframes.numRotations];
	fread(m_GlobalKeys.keyframes.rotations, sizeof(rotationKey_t), m_GlobalKeys.keyframes.numRotations, pFile);
	m_GlobalKeys.keyframes.positions = new positionKey_t[m_GlobalKeys.keyframes.numPositions];
	fread(m_GlobalKeys.keyframes.positions, sizeof(positionKey_t), m_GlobalKeys.keyframes.numPositions, pFile);

	// read klst
	m_Keys = new klst_t[m_Anim.numBones];
	for (i = 0; i < m_Anim.numBones; ++i)
	{
		fread(&type, sizeof(char), 4, pFile);
		type[4] = '\0';
		//Log::Debug() << "type=" << type << endl;
		fread(&version, sizeof(version_t), 1, pFile);
		//Log::Debug() << "version.major=" << (short)version.major << endl;
		//Log::Debug() << "version.minor=" << (short)version.minor << endl;
		fread(&m_Keys[i].boneIndex, sizeof(int), 1, pFile);
		fread(&m_Keys[i].offsetBoneName, sizeof(int), 1, pFile);
		fread(&m_Keys[i].keyframes.numRotations, sizeof(int), 1, pFile);
		fread(&m_Keys[i].keyframes.numPositions, sizeof(int), 1, pFile);
		m_Keys[i].keyframes.rotations = new rotationKey_t[m_Keys[i].keyframes.numRotations];
		fread(m_Keys[i].keyframes.rotations, sizeof(rotationKey_t), m_Keys[i].keyframes.numRotations, pFile);
		m_Keys[i].keyframes.positions = new positionKey_t[m_Keys[i].keyframes.numPositions];
		fread(m_Keys[i].keyframes.positions, sizeof(positionKey_t), m_Keys[i].keyframes.numPositions, pFile);
	}

	return true;
}

void CAnimation::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	int i;

	Log::Debug() << "ANIM" << std::endl;
	Log::Debug() << "m_Anim.sizeTextField=" << m_Anim.sizeTextField << std::endl;
	Log::Debug() << "m_Anim.numBones=" << m_Anim.numBones << std::endl;
	Log::Debug() << "m_Anim.duration=" << m_Anim.duration << std::endl;
	Log::Debug() << "m_Anim.unknownVector=(" << m_Anim.unknownVector.x << ","
		<< m_Anim.unknownVector.x << ","
		<< m_Anim.unknownVector.z << ")" << std::endl;
	Log::Debug() << "m_Anim.unknownQuaternion1=(" << m_Anim.unknownQuaternion1.x << ","
		<< m_Anim.unknownQuaternion1.y << ","
		<< m_Anim.unknownQuaternion1.z << ","
		<< m_Anim.unknownQuaternion1.w << ")" << std::endl;
	Log::Debug() << "m_Anim.unknownQuaternion2=(" << m_Anim.unknownQuaternion2.x << ","
		<< m_Anim.unknownQuaternion2.y << ","
		<< m_Anim.unknownQuaternion2.z << ","
		<< m_Anim.unknownQuaternion2.w << ")" << std::endl;
	Log::Debug() << "m_Anim.unknown=" << m_Anim.unknown << std::endl;
	Log::Debug() << "m_Anim.textField=" << m_Anim.textField << std::endl;
	Log::Debug() << std::endl;

	Log::Debug() << "NOTE" << std::endl;
	Log::Debug() << "m_Notes.numNotes=" << m_Notes.numNotes << std::endl;
	for (i = 0; i < m_Notes.numNotes; ++i)
	{
		Log::Debug() << "m_Notes.notes[" << i << "].time=" << m_Notes.notes[i].time << std::endl;
		Log::Debug() << "m_Notes.notes[" << i << "].token=" << m_Notes.notes[i].token << std::endl;
	}
	Log::Debug() << std::endl;

	Log::Debug() << "TRCR" << std::endl;
	Log::Debug() << "m_Trcr.numItems=" << m_Trcr.numItems << std::endl;
	for (i = 0; i < m_Trcr.numItems; ++i)
	{
		Log::Debug() << "m_Trcr.items[" << i << "].unknown=";
		for (int j = 0; j < 12; ++j)
		{
			if (j)
				Log::Debug() << ",";
			Log::Debug() << m_Trcr.items[i].unknown[j];
		}
		Log::Debug() << std::endl;
	}
	Log::Debug() << std::endl;

	Log::Debug() << "RKEY" << std::endl;
	for (i = 0; i < m_Anim.numBones; ++i)
	{
		Log::Debug() << "m_GlobalKeys.keyframes.numRotations=" << m_GlobalKeys.keyframes.numRotations << std::endl;
		Log::Debug() << "m_GlobalKeys.keyframes.numPositions=" << m_GlobalKeys.keyframes.numPositions << std::endl;
		int j;
		for (j = 0; j < m_GlobalKeys.keyframes.numRotations; ++j)
		{
			Log::Debug() << "m_GlobalKeys.keyframes.rotations[" << j << "].time="
				<< m_GlobalKeys.keyframes.rotations[j].time << std::endl;
			Log::Debug() << "m_GlobalKeys.keyframes.rotations[" << j << "].rotation=("
				<< m_GlobalKeys.keyframes.rotations[j].rotation.x << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.y << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.z << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.w << ")" << std::endl;
		}
		for (j = 0; j < m_GlobalKeys.keyframes.numPositions; ++j)
		{
			Log::Debug() << "m_GlobalKeys.keyframes.positions[" << j << "].time="
				<< m_GlobalKeys.keyframes.positions[j].time << std::endl;
			Log::Debug() << "m_GlobalKeys.keyframes.positions[" << j << "].positions=("
				<< m_GlobalKeys.keyframes.positions[j].position.x << ","
				<< m_GlobalKeys.keyframes.positions[j].position.y << ","
				<< m_GlobalKeys.keyframes.positions[j].position.z << ")" << std::endl;
		}
	}
	Log::Debug() << std::endl;

	Log::Debug() << "KLST" << std::endl;
	for (i = 0; i < m_Anim.numBones; ++i)
	{
		Log::Debug() << "name=" << &m_Anim.textField[m_Keys[i].offsetBoneName] << std::endl;
		Log::Debug() << "m_Keys[" << i << "].boneIndex=" << m_Keys[i].boneIndex << std::endl;
		Log::Debug() << "m_Keys[" << i << "].offsetBoneName=" << m_Keys[i].offsetBoneName << std::endl;
		Log::Debug() << "m_Keys[" << i << "].keyframes.numRotations=" << m_Keys[i].keyframes.numRotations << std::endl;
		Log::Debug() << "m_Keys[" << i << "].keyframes.numPositions=" << m_Keys[i].keyframes.numPositions << std::endl;
		int j;
		for (j = 0; j < m_Keys[i].keyframes.numRotations; ++j)
		{
			Log::Debug() << "m_Keys[" << i << "].keyframes.rotations[" << j << "].time="
				<< m_Keys[i].keyframes.rotations[j].time << std::endl;
			Log::Debug() << "m_Keys[" << i << "].keyframes.rotations[" << j << "].rotation=("
				<< m_Keys[i].keyframes.rotations[j].rotation.x << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.y << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.z << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.w << ")" << std::endl;
		}
		for (j = 0; j < m_Keys[i].keyframes.numPositions; ++j)
		{
			Log::Debug() << "m_Keys[" << i << "].keyframes.positions[" << j << "].time="
				<< m_Keys[i].keyframes.positions[j].time << std::endl;
			Log::Debug() << "m_Keys[" << i << "].keyframes.positions[" << j << "].positions=("
				<< m_Keys[i].keyframes.positions[j].position.x << ","
				<< m_Keys[i].keyframes.positions[j].position.y << ","
				<< m_Keys[i].keyframes.positions[j].position.z << ")" << std::endl;
		}
	}
	Log::Debug() << std::endl;
}
