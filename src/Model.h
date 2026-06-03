#pragma once

#include "AspFile.h"
#include "Animation.h"
#include "Bone.h"
#include "ResourceResolver.h"

#include <gl/freeglut.h>

#include <stdio.h>
#include <filesystem>
#include <string>
#include <vector>

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
	void Initialize(ResourceResolver resourceResolver);
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

	ResourceResolver m_resourceResolver;
	bool m_useResourceResolver{};
};
