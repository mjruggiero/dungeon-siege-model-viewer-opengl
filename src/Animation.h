#pragma once

#include "Math.h"
#include "PrsFile.h"

class CASPModel;

class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	bool Load(const char* filename);
	void Print();

	friend class CASPModel;

private:
	anim_t m_Anim;
	note_t m_Notes;
	trcr_t m_Trcr;
	rkey_t m_GlobalKeys;
	klst_t* m_Keys;
};
