#pragma once

#include "AspFormat.h"
#include "Bone.h"

#include <cstdio>

class SubMesh
{
public:
	SubMesh();
	~SubMesh();
	bool Read(FILE* pFile, int numBones);
	void Print();
	void Render(int type);
	void Update();

	unsigned int* tex;
	Bone* m_pBones;
private:
	int m_numBones;
	bsub_t m_header;
	bsmm_t m_Material;
	bvtx_t m_Vertex;
	bcrn_t m_Corner;
	vector_t* m_Vertices;
	wcrn_t m_CornerExtended;
	bvmp_t m_VertexMapping;
	btri_t m_Face;
	bvwl_t m_VertexWeight;
	stch_t m_Stitches;
};
