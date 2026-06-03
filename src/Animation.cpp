#include "Animation.h"

#include "aspfile.h"
#include "prsfile.h"

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

CAnimation::CAnimation()
{
	m_Keys = NULL;
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
		cerr << "Couldn't load file: " << filename << endl;
		return false;
	}

	// read header
	char type[5];
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	cout << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	cout << "version.major=" << (short)version.major << endl;
	cout << "version.minor=" << (short)version.minor << endl;
	fread(&m_Anim.sizeTextField, sizeof(int), 1, pFile);
	fread(&m_Anim.numBones, sizeof(int), 1, pFile);
	fread(&m_Anim.duration, sizeof(float), 1, pFile);
	fread(&m_Anim.unknownVector, sizeof(vector_t), 1, pFile);
	fread(&m_Anim.unknownQuaternion1, sizeof(quaternion_t), 1, pFile);
	fread(&m_Anim.unknownQuaternion2, sizeof(quaternion_t), 1, pFile);
	fread(&m_Anim.unknown, sizeof(float), 1, pFile);
	m_Anim.textField = new char[m_Anim.sizeTextField];
	fread(m_Anim.textField, sizeof(char), m_Anim.sizeTextField, pFile);
	cout << "textField=" << m_Anim.textField << endl;

	// read notes
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	cout << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	cout << "version.major=" << (short)version.major << endl;
	cout << "version.minor=" << (short)version.minor << endl;
	fread(&m_Notes.numNotes, sizeof(int), 1, pFile);
	m_Notes.notes = new noteInfo_t[m_Notes.numNotes];
	fread(m_Notes.notes, sizeof(noteInfo_t), m_Notes.numNotes, pFile);

	// read trcr
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	if (strcmp(type, "TRCR") == 0)
	{
		cout << "type=" << type << endl;
		fread(&version, sizeof(version_t), 1, pFile);
		cout << "version.major=" << (short)version.major << endl;
		cout << "version.minor=" << (short)version.minor << endl;
		fread(&m_Trcr.numItems, sizeof(int), 1, pFile);
		m_Trcr.items = new itemInfo_t[m_Trcr.numItems];
		fread(m_Trcr.items, sizeof(itemInfo_t), m_Trcr.numItems, pFile);
		fseek(pFile, 8, SEEK_CUR);
		fread(&type, sizeof(char), 4, pFile);
		cout << "type=" << type << endl;
	}

	// read rkey
	fread(&version, sizeof(version_t), 1, pFile);
	cout << "version.major=" << (short)version.major << endl;
	cout << "version.minor=" << (short)version.minor << endl;
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
		//cout << "type=" << type << endl;
		fread(&version, sizeof(version_t), 1, pFile);
		//cout << "version.major=" << (short)version.major << endl;
		//cout << "version.minor=" << (short)version.minor << endl;
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
	int i;

	cout << "ANIM" << endl;
	cout << "m_Anim.sizeTextField=" << m_Anim.sizeTextField << endl;
	cout << "m_Anim.numBones=" << m_Anim.numBones << endl;
	cout << "m_Anim.duration=" << m_Anim.duration << endl;
	cout << "m_Anim.unknownVector=(" << m_Anim.unknownVector.x << ","
		<< m_Anim.unknownVector.x << ","
		<< m_Anim.unknownVector.z << ")" << endl;
	cout << "m_Anim.unknownQuaternion1=(" << m_Anim.unknownQuaternion1.x << ","
		<< m_Anim.unknownQuaternion1.y << ","
		<< m_Anim.unknownQuaternion1.z << ","
		<< m_Anim.unknownQuaternion1.w << ")" << endl;
	cout << "m_Anim.unknownQuaternion2=(" << m_Anim.unknownQuaternion2.x << ","
		<< m_Anim.unknownQuaternion2.y << ","
		<< m_Anim.unknownQuaternion2.z << ","
		<< m_Anim.unknownQuaternion2.w << ")" << endl;
	cout << "m_Anim.unknown=" << m_Anim.unknown << endl;
	cout << "m_Anim.textField=" << m_Anim.textField << endl;
	cout << endl;

	cout << "NOTE" << endl;
	cout << "m_Notes.numNotes=" << m_Notes.numNotes << endl;
	for (i = 0; i < m_Notes.numNotes; ++i)
	{
		cout << "m_Notes.notes[" << i << "].time=" << m_Notes.notes[i].time << endl;
		cout << "m_Notes.notes[" << i << "].token=" << m_Notes.notes[i].token << endl;
	}
	cout << endl;

	cout << "TRCR" << endl;
	cout << "m_Trcr.numItems=" << m_Trcr.numItems << endl;
	for (i = 0; i < m_Trcr.numItems; ++i)
	{
		cout << "m_Trcr.items[" << i << "].unknown=";
		for (int j = 0; j < 12; ++j)
		{
			if (j)
				cout << ",";
			cout << m_Trcr.items[i].unknown[j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "RKEY" << endl;
	for (i = 0; i < m_Anim.numBones; ++i)
	{
		cout << "m_GlobalKeys.keyframes.numRotations=" << m_GlobalKeys.keyframes.numRotations << endl;
		cout << "m_GlobalKeys.keyframes.numPositions=" << m_GlobalKeys.keyframes.numPositions << endl;
		int j;
		for (j = 0; j < m_GlobalKeys.keyframes.numRotations; ++j)
		{
			cout << "m_GlobalKeys.keyframes.rotations[" << j << "].time="
				<< m_GlobalKeys.keyframes.rotations[j].time << endl;
			cout << "m_GlobalKeys.keyframes.rotations[" << j << "].rotation=("
				<< m_GlobalKeys.keyframes.rotations[j].rotation.x << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.y << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.z << ","
				<< m_GlobalKeys.keyframes.rotations[j].rotation.w << ")" << endl;
		}
		for (j = 0; j < m_GlobalKeys.keyframes.numPositions; ++j)
		{
			cout << "m_GlobalKeys.keyframes.positions[" << j << "].time="
				<< m_GlobalKeys.keyframes.positions[j].time << endl;
			cout << "m_GlobalKeys.keyframes.positions[" << j << "].positions=("
				<< m_GlobalKeys.keyframes.positions[j].position.x << ","
				<< m_GlobalKeys.keyframes.positions[j].position.y << ","
				<< m_GlobalKeys.keyframes.positions[j].position.z << ")" << endl;
		}
	}
	cout << endl;

	cout << "KLST" << endl;
	for (i = 0; i < m_Anim.numBones; ++i)
	{
		cout << "name=" << &m_Anim.textField[m_Keys[i].offsetBoneName] << endl;
		cout << "m_Keys[" << i << "].boneIndex=" << m_Keys[i].boneIndex << endl;
		cout << "m_Keys[" << i << "].offsetBoneName=" << m_Keys[i].offsetBoneName << endl;
		cout << "m_Keys[" << i << "].keyframes.numRotations=" << m_Keys[i].keyframes.numRotations << endl;
		cout << "m_Keys[" << i << "].keyframes.numPositions=" << m_Keys[i].keyframes.numPositions << endl;
		int j;
		for (j = 0; j < m_Keys[i].keyframes.numRotations; ++j)
		{
			cout << "m_Keys[" << i << "].keyframes.rotations[" << j << "].time="
				<< m_Keys[i].keyframes.rotations[j].time << endl;
			cout << "m_Keys[" << i << "].keyframes.rotations[" << j << "].rotation=("
				<< m_Keys[i].keyframes.rotations[j].rotation.x << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.y << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.z << ","
				<< m_Keys[i].keyframes.rotations[j].rotation.w << ")" << endl;
		}
		for (j = 0; j < m_Keys[i].keyframes.numPositions; ++j)
		{
			cout << "m_Keys[" << i << "].keyframes.positions[" << j << "].time="
				<< m_Keys[i].keyframes.positions[j].time << endl;
			cout << "m_Keys[" << i << "].keyframes.positions[" << j << "].positions=("
				<< m_Keys[i].keyframes.positions[j].position.x << ","
				<< m_Keys[i].keyframes.positions[j].position.y << ","
				<< m_Keys[i].keyframes.positions[j].position.z << ")" << endl;
		}
	}
	cout << endl;
}
