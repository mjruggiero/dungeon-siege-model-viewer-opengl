#pragma once

#include "Math.h"

class CBone
{
public:
	CBone();
	~CBone();
	void AddChild(CBone* pBone);
	void Render();
	void Print();

	int id;
	char* name;
	CBone* m_pParent;
	CBone* m_pSibling;
	CBone* m_pChild;
	quaternion_t inverseRotation;
	vector_t inverseTranslation;
	quaternion_t localRotation;
	vector_t localTranslation;
	float m_localMatrix[16];
};
