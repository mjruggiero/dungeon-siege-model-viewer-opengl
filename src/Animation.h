#pragma once

#include "Math.h"
#include "PrsFile.h"

class ASPModel;

class Animation
{
public:
	Animation();
	~Animation();

	bool Load(const char* filename);
	void Print();

	friend class ASPModel;

private:
	anim_t m_Anim;
	note_t m_Notes;
	trcr_t m_Trcr;
	rkey_t m_GlobalKeys;
	klst_t* m_Keys;
};
