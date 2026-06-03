#pragma once

#include "Math.h"

class Bone
{
public:
	Bone();
	~Bone();
	void AddChild(Bone* pBone);
	void Render();
	void Print();

	int id;
	char* name;
	Bone* m_pParent;
	Bone* m_pSibling;
	Bone* m_pChild;
	quaternion_t inverseRotation;
	vector_t inverseTranslation;
	quaternion_t localRotation;
	vector_t localTranslation;
	float m_localMatrix[16];
};
