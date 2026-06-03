// model.h

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <gl/freeglut.h>
#include "aspfile.h"
#include "prsfile.h"
using namespace std;

class CFileTreeNode
{
public:
	CFileTreeNode();
	~CFileTreeNode();
	void AddChild(CFileTreeNode* pChild);

	CFileTreeNode* m_pParent;
	CFileTreeNode* m_pChild;
	CFileTreeNode* m_pSibling;
	char m_pszAcronym[16];
	char m_pszPath[128];
	char m_pszName[128];
};

class CName
{
public:
	char m_pszAcronym[16];
	char m_pszPath[128];
	char m_pszName[128];
};

class CFilePathTree
{
public:
	CFilePathTree();
	~CFilePathTree();
	bool ParseNNKFile(const char* filename);
	char* GetFilePath(const char* filename);

private:
	std::vector<CName> m_vNames;

};

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

class CSubMesh
{
public:
	CSubMesh();
	~CSubMesh();
	bool Read(FILE* pFile, int numBones);
	void Print();
	void Render(int type);
	void Update();

	unsigned int* tex;
	CBone* m_pBones;
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

class CASPModel
{
public:
	CASPModel();
	~CASPModel();

	void Initialize(
		const char* basePath,
		const char* modelPath,
		const char* animationPath,
		const char* texturePath);
	bool Load(const char* filename);
	bool LoadAnimation(const char* filename);
	void Print();
	void LoadTextures();
	void Render(int type);
	void Update(long deltaTime);
	void Interpolate(const float delta, CBone* pBone);
	void PrintBoneInfo();

	// friends
	friend class CSubMesh;

	CAnimation m_Anim;
	CFilePathTree m_pathTree;

private:
	unsigned int tex[8];
	bmsh_t m_header;
	bonh_t* m_pMeshBones;
	CSubMesh* m_pSubMesh;
	rpos_t m_BoneInfo;
	bbox_t m_BoundingBox;
	CBone* m_pBones;

	std::string m_strBasePath;
	std::string m_strModelPath;
	std::string m_strAnimationPath;
	std::string m_strTexturePath;

};

#endif // _MODEL_H_
