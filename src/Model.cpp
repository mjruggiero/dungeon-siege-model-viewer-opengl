#include "Model.h"

#include "Log.h"
#include "RawFile.h"
#include "SubMesh.h"

#include <filesystem>
#include <string>
#include <utility>

extern bool bones;

ASPModel::ASPModel()
{
	m_pBones = nullptr;
	m_pSubMesh = nullptr;
	m_BoneInfo.positions = nullptr;
}

ASPModel::~ASPModel()
{
	if (m_pBones)
		delete[] m_pBones;

	if (m_pSubMesh)
		delete[] m_pSubMesh;

	if (m_BoneInfo.positions)
		delete[] m_BoneInfo.positions;
}

void ASPModel::Initialize(
	const char* basePath,
	const char* modelPath,
	const char* animationPath,
	const char* texturePath)
{
	std::string strBasePath = basePath;
	if (strBasePath[strBasePath.size() - 1] != '/') strBasePath += '/';

	m_strBasePath = strBasePath;
	m_strModelPath = strBasePath + modelPath;
	m_strAnimationPath = strBasePath + animationPath;
	m_strTexturePath = strBasePath + texturePath;
	m_useResourceResolver = false;
}

void ASPModel::Initialize(ResourceResolver resourceResolver)
{
	m_resourceResolver = std::move(resourceResolver);
	m_useResourceResolver = true;
}

bool ASPModel::Load(const char* filename)
{
	FILE* pFile;
	version_t version;

	std::filesystem::path fullPath;

	if (m_useResourceResolver)
	{
		const auto resolvedPath = m_resourceResolver.ResolveAspModelPath(filename);

		if (!resolvedPath)
		{
			Log::Error() << "Couldn't resolve ASP model: " << filename << std::endl;
			return false;
		}

		fullPath = *resolvedPath;
	}
	else
	{
		fullPath = std::filesystem::path{ m_strModelPath } / filename;
	}

	pFile = fopen(fullPath.string().c_str(), "rb");

	if (!pFile)
	{
		Log::Error() << "Couldn't open ASP model: " << fullPath.string() << std::endl;
		return false;
	}

	// read header
	char type[5];
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	//Log::Debug() << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;

	fread(&m_header.sizeTextField, sizeof(int), 1, pFile);
	fread(&m_header.numBones, sizeof(int), 1, pFile);
	fread(&m_header.numMaterials, sizeof(int), 1, pFile);
	fread(&m_header.numVertices, sizeof(int), 1, pFile);
	fread(&m_header.numSubmeshes, sizeof(int), 1, pFile);
	fread(&m_header.unknown, sizeof(int), 1, pFile);
	m_header.textField = new char[m_header.sizeTextField];
	fread(m_header.textField, sizeof(char), m_header.sizeTextField, pFile);

	// read bones
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	//Log::Debug() << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;

	m_pMeshBones = new bonh_t[m_header.numBones];
	for (int i = 0; i < m_header.numBones; ++i)
	{
		fread(&m_pMeshBones[i], sizeof(bonh_t), 1, pFile);
	}

	// read bsub
	m_pSubMesh = new SubMesh[m_header.numSubmeshes];

	for (int i = 0; i < m_header.numSubmeshes; ++i)
	{
		m_pSubMesh[i].tex = tex;
		m_pSubMesh[i].Read(pFile, m_header.numBones);
	}

	// read rpos
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	//Log::Debug() << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_BoneInfo.numBones, sizeof(int), 1, pFile);
	m_BoneInfo.positions = new position_t[m_BoneInfo.numBones];
	fread(m_BoneInfo.positions, sizeof(position_t), m_BoneInfo.numBones, pFile);

	// setup bones
	m_pBones = new Bone[m_header.numBones];
	for (int i = 0; i < m_header.numBones; ++i)
	{
		m_pBones[i].id = i;
		m_pBones[i].name = new char[strlen(&m_header.textField[m_pMeshBones[i].nameOffset]) + 1];
		strcpy(m_pBones[i].name, &m_header.textField[m_pMeshBones[i].nameOffset]);
		m_pBones[i].inverseRotation = m_BoneInfo.positions[i].inverseRotation;
		m_pBones[i].inverseTranslation = m_BoneInfo.positions[i].inverseTranslation;
		m_pBones[i].localRotation = m_BoneInfo.positions[i].localRotation;
		m_pBones[i].localTranslation = m_BoneInfo.positions[i].localTranslation;
		// link
		if (m_pMeshBones[i].index != m_pMeshBones[i].parentIndex)
		{
			Bone* pParent = &m_pBones[m_pMeshBones[i].parentIndex];
			pParent->AddChild(&m_pBones[i]);
		}
	}

	// set bone pointers for sub meshes
	for (int i = 0; i < m_header.numSubmeshes; ++i)
	{
		m_pSubMesh[i].m_pBones = m_pBones;
	}

	// read bbox
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	//Log::Debug() << "type=" << type << endl;
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_BoundingBox.unknown, sizeof(int), 1, pFile);

	// read bend
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	//Log::Debug() << "type=" << type << endl;

	fclose(pFile);

	LoadTextures();

	Print();

	return true;
}

void ASPModel::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	int i;
	Log::Debug() << "HEADER" << std::endl;
	Log::Debug() << "m_header.sizeTextField=" << m_header.sizeTextField << std::endl;
	Log::Debug() << "m_header.numBones=" << m_header.numBones << std::endl;
	Log::Debug() << "m_header.numMaterials=" << m_header.numMaterials << std::endl;
	Log::Debug() << "m_header.numVertices=" << m_header.numVertices << std::endl;
	Log::Debug() << "m_header.numSubmeshes=" << m_header.numSubmeshes << std::endl;
	Log::Debug() << "m_header.textField=";
	//Log::Debug() << "m_header.textField=" << m_header.textField << endl;
	for (i = 0; i < m_header.sizeTextField; ++i)
		Log::Debug() << m_header.textField[i];
	Log::Debug() << std::endl;

	Log::Debug() << "BONES" << std::endl;
	for (i = 0; i < m_header.numBones; ++i)
	{
		Log::Debug() << "name=" << &m_header.textField[m_pMeshBones[i].nameOffset] << std::endl;
		Log::Debug() << "m_pMeshBones[" << i << "].index=" << m_pMeshBones[i].index << std::endl;
		Log::Debug() << "m_pMeshBones[" << i << "].parentIndex=" << m_pMeshBones[i].parentIndex << std::endl;
		Log::Debug() << "m_pMeshBones[" << i << "].nameOffset=" << m_pMeshBones[i].nameOffset << std::endl;
	}

	for (i = 0; i < m_header.numSubmeshes; ++i)
	{
		Log::Debug() << "SUBMESH #" << i << std::endl;
		m_pSubMesh[i].Print();
	}

	Log::Debug() << "BONE INFO" << std::endl;
	Log::Debug() << "m_BoneInfo.numBones=" << m_BoneInfo.numBones << std::endl;
	for (i = 0; i < m_BoneInfo.numBones; ++i)
	{
		Log::Debug() << "m_BoneInfo.positions[" << i << "].inverseRotation=("
			<< m_BoneInfo.positions[i].inverseRotation.w << ","
			<< m_BoneInfo.positions[i].inverseRotation.x << ","
			<< m_BoneInfo.positions[i].inverseRotation.y << ","
			<< m_BoneInfo.positions[i].inverseRotation.z << ")" << std::endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].inverseTranslation=("
			<< m_BoneInfo.positions[i].inverseTranslation.x << ","
			<< m_BoneInfo.positions[i].inverseTranslation.y << ","
			<< m_BoneInfo.positions[i].inverseTranslation.z << ")" << std::endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].localRotation=("
			<< m_BoneInfo.positions[i].localRotation.w << ","
			<< m_BoneInfo.positions[i].localRotation.x << ","
			<< m_BoneInfo.positions[i].localRotation.y << ","
			<< m_BoneInfo.positions[i].localRotation.z << ")" << std::endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].localTranslation=("
			<< m_BoneInfo.positions[i].localTranslation.x << ","
			<< m_BoneInfo.positions[i].localTranslation.y << ","
			<< m_BoneInfo.positions[i].localTranslation.z << ")" << std::endl;
	}

	Log::Debug() << "BOUNDING BOX" << std::endl;
	Log::Debug() << "m_BoundingBox.unknown=" << m_BoundingBox.unknown << std::endl;
}

void ASPModel::LoadTextures()
{
	int width, height;
	int index = 0;

	// Load each texture for each material.
	for (int i = 0; i < m_header.numMaterials; ++i)
	{
		const std::string textureName = &m_header.textField[index];

		// Texture names in the ASP text field are padded to dword boundaries.
		index += static_cast<int>(textureName.length()) +
			(4 - static_cast<int>(textureName.length()) % 4);

		std::filesystem::path fullPath;

		if (m_useResourceResolver)
		{
			const auto resolvedPath = m_resourceResolver.ResolveRawTexturePath(textureName);

			if (!resolvedPath)
			{
				Log::Warning() << "Skipping unresolved RAW texture: " << textureName << std::endl;
				continue;
			}

			fullPath = *resolvedPath;
		}
		else
		{
			fullPath = std::filesystem::path{ m_strTexturePath } / (textureName + ".raw");
		}

		char* buffer = LoadRawFile(fullPath.string().c_str(), width, height);

		if (buffer == nullptr)
		{
			Log::Warning() << "Skipping RAW texture that failed to load: "
				<< fullPath.string() << std::endl;
			continue;
		}

		glGenTextures(1, &tex[i]);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);

		delete[] buffer;
	}
}

void ASPModel::Render(int type)
{
	int i;

	// reader all submeshes
	for (i = 0; i < m_header.numSubmeshes; ++i)
		m_pSubMesh[i].Render(type);

	// render bones
	if (bones)
	{
		glDisable(GL_TEXTURE_2D);
		m_pBones[0].Render();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
	}
}

void ASPModel::Update(long deltaTime)
{
	long duration = m_Anim.m_Anim.duration * 1000;
	float delta = (deltaTime % duration) / (float)duration;
	//Log::Debug() << "delta=" << delta << endl;
	// push modelview matrix onto stack
	glPushMatrix();

	// load identity into model matrix
	glLoadIdentity();

	// call the recursive function for pelvis
	Interpolate(delta, m_pBones);

	// pop modelview matrix off of stack 
	glPopMatrix();

	// now you have the transformation matrices 

	// fix all vertices
	for (int i = 0; i < m_header.numSubmeshes; ++i)
	{
		m_pSubMesh[i].Update();
	}
}

void ASPModel::Interpolate(const float delta, Bone* pBone)
{
	// push modelview matrix onto stack 
	glPushMatrix();

	int i;
	klst_t* pList = &m_Anim.m_Keys[pBone->id];

	// translate by relative vector
	int positionIndex = 0;
	vector_t trans;
	float t;
	if (pList->keyframes.numPositions > 1)
	{
		for (i = 0; i < pList->keyframes.numPositions; ++i)
		{
			if (pList->keyframes.positions[i].time <= delta)
				positionIndex = i;
		}

		if (positionIndex == pList->keyframes.numPositions - 1)
		{
			trans = pList->keyframes.positions[positionIndex].position;
		}
		else
		{
			// interpolate
			t = (delta - pList->keyframes.positions[positionIndex].time) /
				(pList->keyframes.positions[positionIndex + 1].time - pList->keyframes.positions[positionIndex].time);

			trans = (1.0 - t) * pList->keyframes.positions[positionIndex].position +
				t * pList->keyframes.positions[positionIndex + 1].position;
		}
	}
	else
	{
		trans = pList->keyframes.positions[positionIndex].position;
	}

	pBone->localTranslation = trans;

	glTranslatef(pBone->localTranslation.x, pBone->localTranslation.y, pBone->localTranslation.z);

	// rotate by relative quaternion 
	int rotationIndex = 0;
	quaternion_t rot;
	if (pList->keyframes.numRotations > 1)
	{
		for (i = 0; i < pList->keyframes.numRotations; ++i)
		{
			if (pList->keyframes.rotations[i].time <= delta)
				rotationIndex = i;
		}

		if (rotationIndex == pList->keyframes.numRotations - 1)
		{
			rot = pList->keyframes.rotations[rotationIndex].rotation;
		}
		else
		{
			// interpolate
			t = (delta - pList->keyframes.rotations[rotationIndex].time) /
				(pList->keyframes.rotations[rotationIndex + 1].time - pList->keyframes.rotations[rotationIndex].time);

			rot = slerp(pList->keyframes.rotations[rotationIndex].rotation,
				pList->keyframes.rotations[rotationIndex + 1].rotation, t);
		}
	}
	else
	{
		rot = pList->keyframes.rotations[rotationIndex].rotation;
	}

	pBone->localRotation = rot;

	quaternion_t axisAngle;
	axisAngle = ConvertToAxisAngle(pBone->localRotation);
	if (axisAngle.w != 0.0)
		glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z);

	// push modelview matrix onto stack 
	glPushMatrix();

	// translate by absolute inverse vector (in asp) 
	glTranslatef(m_BoneInfo.positions[pBone->id].inverseTranslation.x,
		m_BoneInfo.positions[pBone->id].inverseTranslation.y,
		m_BoneInfo.positions[pBone->id].inverseTranslation.z);

	// rotate by absolute inverse quaternion (in asp) 
	//quaternion_t axisAngle;
	axisAngle = ConvertToAxisAngle(pBone->inverseRotation);
	glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z);

	// copy current modelview matrix somewhere to save 
	glGetFloatv(GL_MODELVIEW_MATRIX, pBone->m_localMatrix);

	// pop modelview matrix off of stack 
	glPopMatrix();

	// recursively call this func for all children
	if (pBone->m_pChild)
		Interpolate(delta, pBone->m_pChild);

	// pop modelview matrix off of stack 
	glPopMatrix();

	if (pBone->m_pSibling)
		Interpolate(delta, pBone->m_pSibling);
}

bool ASPModel::LoadAnimation(const char* filename)
{
	std::filesystem::path fullPath;

	if (m_useResourceResolver)
	{
		const auto resolvedPath = m_resourceResolver.ResolvePrsAnimationPath(filename);

		if (!resolvedPath)
		{
			Log::Error() << "Couldn't resolve PRS animation: " << filename << std::endl;
			return false;
		}

		fullPath = *resolvedPath;
	}
	else
	{
		fullPath = std::filesystem::path{ m_strAnimationPath } / filename;
	}

	return m_Anim.Load(fullPath.string().c_str());
}

void ASPModel::PrintBoneInfo()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	for (int i = 0; i < m_header.numBones; ++i)
	{
		m_pBones[i].Print();
	}
}
