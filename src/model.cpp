#include "model.h"

#include "Log.h"

extern bool bones;

CFileTreeNode::CFileTreeNode()
{
	m_pParent = NULL;
	m_pChild = NULL;
	m_pSibling = NULL;
}

CFileTreeNode::~CFileTreeNode()
{

}

void CFileTreeNode::AddChild(CFileTreeNode* pChild)
{
	CFileTreeNode* pParent = this;
	CFileTreeNode* pIter = pParent;

	if (strlen(pChild->m_pszAcronym) != 1)
	{
		// find node where this belongs
		while (pIter->m_pSibling != NULL)
		{
			if (!strncmp(pIter->m_pszAcronym, pChild->m_pszAcronym, strlen(pIter->m_pszAcronym)))
			{
				pParent = pIter;
			}
			pIter = pIter->m_pSibling;
		}
	}

	if (pParent->m_pChild == NULL)
	{
		pParent->m_pChild = pChild;
	}
	else
	{
		CFileTreeNode* pIter = pParent->m_pChild;
		while (pIter->m_pSibling != NULL)
		{
			pIter = pIter->m_pSibling;
		}
		pIter->m_pSibling = pChild;
	}

	// set the parent to this
	pChild->m_pParent = pParent;
}

CFilePathTree::CFilePathTree()
{
	//m_pRoot = NULL;
}

CFilePathTree::~CFilePathTree()
{
	// delete tree
}

bool CFilePathTree::ParseNNKFile(const char* filename)
{
	FILE* pFile = fopen(filename, "r");

	if (!pFile)
		return false;

	char str[1024];

	CName name;

	while (fgets(str, 1024, pFile))
	{
		// process text
		if (strncmp(str, "TREE =", 6) == 0)
		{
			//int x = 0;
			strcpy(str, &str[7]);
			char* token = NULL;
			token = strtok(str, ",\t ");
			if (token)
				strcpy(name.m_pszAcronym, token);
			token = strtok(NULL, ",");
			if (token)
				strcpy(name.m_pszPath, &token[1]);
			token = strtok(NULL, ",");
			if (token)
				strcpy(name.m_pszName, token);
			m_vNames.push_back(name);
		}
	}

	fclose(pFile);

	//Log::Debug() << "length of names=" << m_vNames.size() << endl;
	//for(std::vector<CName>::iterator it = m_vNames.begin(); it != m_vNames.end(); ++it)
	//{
	//	Log::Debug() << "acronym=" << it->m_pszAcronym << endl;
	//	Log::Debug() << "path=" << it->m_pszPath << endl;
	//	Log::Debug() << "name=" << it->m_pszName << endl;
	//}

	return true;
}

char* CFilePathTree::GetFilePath(const char* filename)
{
	return NULL;
}

//	CFileTreeNode *m_pRoot;

CSubMesh::CSubMesh()
{
	m_Material.materials = NULL;
	m_Vertex.vertices = NULL;
	m_Corner.corners = NULL;
	m_CornerExtended.corners = NULL;
	m_VertexMapping.vertexMaps = NULL;
	m_Face.materials = NULL;
	m_Face.faces = NULL;
	m_VertexWeight.bones = NULL;
	m_Stitches.stitches = NULL;
}

CSubMesh::~CSubMesh()
{
	if (m_Material.materials)
		delete[] m_Material.materials;

	if (m_Vertex.vertices)
		delete[] m_Vertex.vertices;

	if (m_Corner.corners)
		delete[] m_Corner.corners;

	if (m_CornerExtended.corners)
		delete[] m_CornerExtended.corners;

	if (m_VertexMapping.vertexMaps)
	{
		for (int i = 0; i < m_header.numVertices; ++i)
		{
			if (m_VertexMapping.vertexMaps[i].cornerIndices)
				delete[] m_VertexMapping.vertexMaps[i].cornerIndices;
		}
		delete[] m_VertexMapping.vertexMaps;
	}

	if (m_Face.materials)
		delete[] m_Face.materials;

	if (m_Face.faces)
		delete[] m_Face.faces;

	if (m_VertexWeight.bones)
	{
		for (int i = 0; i < m_numBones; ++i)
			delete[] m_VertexWeight.bones[i].vertices;
		delete[] m_VertexWeight.bones;
	}

	if (m_Stitches.stitches)
	{
		for (int i = 0; i < m_Stitches.numStitches; ++i)
		{
			if (m_Stitches.stitches[i].vertices)
				delete[] m_Stitches.stitches[i].vertices;
		}
		delete[] m_Stitches.stitches;
	}
}

bool CSubMesh::Read(FILE* pFile, int numBones)
{
	m_numBones = numBones;
	int i;
	char type[5];
	version_t version;

	// read submesh header
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_header, sizeof(bsub_t), 1, pFile);

	// read material
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_Material.numMaterials, sizeof(int), 1, pFile);
	m_Material.materials = new material_t[m_Material.numMaterials];
	fread(m_Material.materials, sizeof(material_t), m_Material.numMaterials, pFile);

	// read vertex
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_Vertex.numVertices, sizeof(int), 1, pFile);
	m_Vertices = new vector_t[m_header.numCorners];
	m_Vertex.vertices = new vector_t[m_Vertex.numVertices];
	//fread(m_Vertex.vertices, sizeof(vector_t), m_Vertex.numVertices, pFile);
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		fread(&m_Vertex.vertices[i].x, sizeof(float), 1, pFile);
		fread(&m_Vertex.vertices[i].y, sizeof(float), 1, pFile);
		fread(&m_Vertex.vertices[i].z, sizeof(float), 1, pFile);
	}

	// read corner
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_Corner.numCorners, sizeof(int), 1, pFile);
	m_Corner.corners = new corner_t[m_Corner.numCorners];
	fread(m_Corner.corners, sizeof(corner_t), m_Corner.numCorners, pFile);

	// read extended corner
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_CornerExtended.numCorners, sizeof(int), 1, pFile);
	m_CornerExtended.corners = new wcorner_t[m_CornerExtended.numCorners];
	fread(m_CornerExtended.corners, sizeof(wcorner_t), m_CornerExtended.numCorners, pFile);

	// read vertex mapping
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	m_VertexMapping.vertexMaps = new vertexMap_t[m_Vertex.numVertices];
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		fread(&m_VertexMapping.vertexMaps[i].numCorners, sizeof(int), 1, pFile);
		m_VertexMapping.vertexMaps[i].cornerIndices = new int[m_VertexMapping.vertexMaps[i].numCorners];
		fread(m_VertexMapping.vertexMaps[i].cornerIndices, sizeof(int), m_VertexMapping.vertexMaps[i].numCorners, pFile);
	}

	// read face
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_Face.numFaces, sizeof(int), 1, pFile);
	m_Face.materials = new material_t[m_Material.numMaterials];
	fread(m_Face.materials, sizeof(material_t), m_Material.numMaterials, pFile);
	m_Face.faces = new face_t[m_Face.numFaces];
	//fread(m_Face.faces, sizeof(face_t), m_Face.numFaces, pFile);
	for (i = 0; i < m_Face.numFaces; ++i)
	{
		fread(&m_Face.faces[i].indexCornerA, sizeof(int), 1, pFile);
		fread(&m_Face.faces[i].indexCornerB, sizeof(int), 1, pFile);
		fread(&m_Face.faces[i].indexCornerC, sizeof(int), 1, pFile);
	}


	// read vertex weight
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	m_VertexWeight.bones = new bone_t[numBones];
	for (i = 0; i < numBones; ++i)
	{
		fread(&m_VertexWeight.bones[i].numVertices, sizeof(int), 1, pFile);
		if (m_VertexWeight.bones[i].numVertices > 0)
		{
			m_VertexWeight.bones[i].vertices = new affectedVertex_t[m_VertexWeight.bones[i].numVertices];
			fread(m_VertexWeight.bones[i].vertices, sizeof(affectedVertex_t), m_VertexWeight.bones[i].numVertices, pFile);
		}
		else
			m_VertexWeight.bones[i].vertices = NULL;
	}

	// read stitches
	fread(&type, sizeof(char), 4, pFile);
	type[4] = '\0';
	fread(&version, sizeof(version_t), 1, pFile);
	//Log::Debug() << "type=" << type << endl;
	//Log::Debug() << "version.major=" << (short)version.major << endl;
	//Log::Debug() << "version.minor=" << (short)version.minor << endl;
	fread(&m_Stitches.numStitches, sizeof(int), 1, pFile);
	m_Stitches.stitches = new stitch_t[m_Stitches.numStitches];
	for (i = 0; i < m_Stitches.numStitches; ++i)
	{
		fread(&m_Stitches.stitches[i].token, sizeof(char), 4, pFile);
		fread(&m_Stitches.stitches[i].numVertices, sizeof(int), 1, pFile);
		m_Stitches.stitches[i].vertices = new int[m_Stitches.stitches[i].numVertices];
		fread(m_Stitches.stitches[i].vertices, sizeof(int), m_Stitches.stitches[i].numVertices, pFile);
	}
	return true;
}

void CSubMesh::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	int i;
	Log::Debug() << "HEADER" << endl;
	Log::Debug() << "m_header.id=" << m_header.id << endl;
	Log::Debug() << "m_header.numMaterials=" << m_header.numMaterials << endl;
	Log::Debug() << "m_header.numVertices=" << m_header.numVertices << endl;
	Log::Debug() << "m_header.numCorners=" << m_header.numCorners << endl;
	Log::Debug() << "m_header.numFaces=" << m_header.numFaces << endl;

	Log::Debug() << "MATERIALS" << endl;
	Log::Debug() << "m_Material.numMaterials=" << m_Material.numMaterials << endl;
	for (i = 0; i < m_Material.numMaterials; ++i)
	{
		Log::Debug() << "m_Material.materials[" << i << "].index=" << m_Material.materials[i].index << endl;
		Log::Debug() << "m_Material.materials[" << i << "].numFaces=" << m_Material.materials[i].numFaces << endl;
	}

	Log::Debug() << "VERTEX" << endl;
	Log::Debug() << "m_Vertex.numVertices=" << m_Vertex.numVertices << endl;
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		Log::Debug() << "m_Vertex.vertices[" << i << "].=(" << m_Vertex.vertices[i].x << ","
			<< m_Vertex.vertices[i].y << ","
			<< m_Vertex.vertices[i].z << ")" << endl;
	}

	Log::Debug() << "CORNER" << endl;
	Log::Debug() << "m_Corner.numCorners=" << m_Corner.numCorners << endl;
	for (i = 0; i < m_Corner.numCorners; ++i)
	{
		Log::Debug() << "m_Corner.corners[" << i << "].vertexIndex=" << m_Corner.corners[i].vertexIndex << endl;
		Log::Debug() << "m_Corner.corners[" << i << "].normal=(" << m_Corner.corners[i].normal.x << ","
			<< m_Corner.corners[i].normal.x << ","
			<< m_Corner.corners[i].normal.x << ")" << endl;
		Log::Debug() << "m_Corner.corners[" << i << "].unknown1=" << m_Corner.corners[i].unknown1 << endl;
		Log::Debug() << "m_Corner.corners[" << i << "].unknown2=" << m_Corner.corners[i].unknown2 << endl;
		Log::Debug() << "m_Corner.corners[" << i << "].u=" << m_Corner.corners[i].u << endl;
		Log::Debug() << "m_Corner.corners[" << i << "].v=" << m_Corner.corners[i].v << endl;
	}

	Log::Debug() << "EXTENDED CORNER" << endl;
	Log::Debug() << "m_CornerExtended.numCorners=" << m_CornerExtended.numCorners << endl;
	for (i = 0; i < m_CornerExtended.numCorners; ++i)
	{
		Log::Debug() << "m_CornerExtended.corners[" << i << "].position=(" << m_CornerExtended.corners[i].position.x << ","
			<< m_CornerExtended.corners[i].position.y << ","
			<< m_CornerExtended.corners[i].position.z << ")" << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight0=" << m_CornerExtended.corners[i].boneWeight0 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight1=" << m_CornerExtended.corners[i].boneWeight1 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight2=" << m_CornerExtended.corners[i].boneWeight2 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight3=" << m_CornerExtended.corners[i].boneWeight3 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex0=" << (short)m_CornerExtended.corners[i].boneIndex0 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex1=" << (short)m_CornerExtended.corners[i].boneIndex1 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex2=" << (short)m_CornerExtended.corners[i].boneIndex2 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex3=" << (short)m_CornerExtended.corners[i].boneIndex3 << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].normal=(" << m_CornerExtended.corners[i].normal.x << ","
			<< m_CornerExtended.corners[i].normal.y << ","
			<< m_CornerExtended.corners[i].normal.z << ")" << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].unknown=" << m_CornerExtended.corners[i].unknown << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].u=" << m_CornerExtended.corners[i].u << endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].v=" << m_CornerExtended.corners[i].v << endl;
	}

	Log::Debug() << "VERTEX MAPPING" << endl;
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		Log::Debug() << "m_VertexMapping.vertexMaps[" << i << "].numCorners=" << m_VertexMapping.vertexMaps[i].numCorners << endl;
		Log::Debug() << "m_VertexMapping.vertexMaps[" << i << "].cornerIndices=";
		for (int j = 0; j < m_VertexMapping.vertexMaps[i].numCorners; ++j)
		{
			if (j)
				Log::Debug() << ",";
			Log::Debug() << m_VertexMapping.vertexMaps[i].cornerIndices[j];
		}
		Log::Debug() << endl;
	}

	Log::Debug() << "FACE" << endl;
	Log::Debug() << "m_Face.numFaces=" << m_Face.numFaces << endl;
	for (i = 0; i < m_Material.numMaterials; ++i)
	{
		Log::Debug() << "m_Faces.materials[" << i << "].index=" << m_Face.materials[i].index << endl;
		Log::Debug() << "m_Faces.materials[" << i << "].numFaces=" << m_Face.materials[i].numFaces << endl;
	}
	for (i = 0; i < m_Face.numFaces; ++i)
	{
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerA=" << m_Face.faces[i].indexCornerA << endl;
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerB=" << m_Face.faces[i].indexCornerB << endl;
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerC=" << m_Face.faces[i].indexCornerC << endl;
	}

	Log::Debug() << "VERTEX WEIGHTS" << endl;
	for (i = 0; i < m_numBones; ++i)
	{
		Log::Debug() << "m_VertexWeight.bones[" << i << "].numVertices=" << m_VertexWeight.bones[i].numVertices << endl;
		for (int j = 0; j < m_VertexWeight.bones[i].numVertices; ++j)
		{
			Log::Debug() << "m_VertexWeight.bones[" << i << "].vertices[" << j << "].index="
				<< m_VertexWeight.bones[i].vertices[j].index << endl;
			Log::Debug() << "m_VertexWeight.bones[" << i << "].vertices[" << j << "].weighting="
				<< m_VertexWeight.bones[i].vertices[j].weighting << endl;
		}
	}

	Log::Debug() << "STITCHES" << endl;
	//stch_t m_Stitches;
	Log::Debug() << "m_Stitches.x=" << m_Stitches.numStitches << endl;
	for (i = 0; i < m_Stitches.numStitches; ++i)
	{
		Log::Debug() << "m_Stitches.stitches[" << i << "].token=" << m_Stitches.stitches[i].token << endl;
		Log::Debug() << "m_Stitches.stitches[" << i << "].numVertices=" << m_Stitches.stitches[i].numVertices << endl;
		Log::Debug() << "m_Stitches.stitches[" << i << "].vertices=";
		for (int j = 0; j < m_Stitches.stitches[i].numVertices; ++j)
		{
			if (j)
				Log::Debug() << ",";
			Log::Debug() << m_Stitches.stitches[i].vertices[j];
		}
		Log::Debug() << endl;
	}
}

void CSubMesh::Render(int type)
{

	int i;
	int faceIndex = 0;

	// loop through materials and draw each face associated with it
	for (i = 0; i < m_Material.numMaterials; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, tex[m_Material.materials[i].index]);

		glBegin(type);

		int startIndex = m_Face.materials[i].index;
		// loop through all faces for this material
		//faceIndex = m_Face.materials[i].index;
		for (int j = 0; j < m_Material.materials[i].numFaces; ++j)
		{
			/*
			// draw this face
			// first point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.z);
			glVertex3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].position.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].position.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].position.z);

			// second point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.z);
			glVertex3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].position.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].position.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].position.z);

			// third point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.z);
			glVertex3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].position.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].position.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].position.z);

			*/
			// draw this face
			// first point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerA + startIndex].normal.z);
			glVertex3f(m_Vertices[m_Face.faces[faceIndex].indexCornerA + startIndex].x,
				m_Vertices[m_Face.faces[faceIndex].indexCornerA + startIndex].y,
				m_Vertices[m_Face.faces[faceIndex].indexCornerA + startIndex].z);

			// second point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerB + startIndex].normal.z);
			glVertex3f(m_Vertices[m_Face.faces[faceIndex].indexCornerB + startIndex].x,
				m_Vertices[m_Face.faces[faceIndex].indexCornerB + startIndex].y,
				m_Vertices[m_Face.faces[faceIndex].indexCornerB + startIndex].z);

			// third point
			glTexCoord2f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].u,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].v);
			glNormal3f(m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.x,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.y,
				m_CornerExtended.corners[m_Face.faces[faceIndex].indexCornerC + startIndex].normal.z);
			glVertex3f(m_Vertices[m_Face.faces[faceIndex].indexCornerC + startIndex].x,
				m_Vertices[m_Face.faces[faceIndex].indexCornerC + startIndex].y,
				m_Vertices[m_Face.faces[faceIndex].indexCornerC + startIndex].z);

			// increment to next face
			++faceIndex;
		}
		glEnd();

	}
}

void CSubMesh::Update()
{
	for (int i = 0; i < m_header.numCorners; ++i)
	{
		// HACK ALERT
		// JUST COPY OVER STATIC VERTICES FOR NO ANIMATION
		//m_Vertices[i] = m_CornerExtended.corners[i].position;
		//continue;
		// blend vertices;
		if (m_CornerExtended.corners[i].boneIndex0)
		{
			m_Vertices[i] = m_CornerExtended.corners[i].boneWeight0 *
				(m_pBones[m_CornerExtended.corners[i].boneIndex0 - 1].m_localMatrix * m_CornerExtended.corners[i].position);
		}
		if (m_CornerExtended.corners[i].boneIndex1)
		{
			m_Vertices[i] = m_Vertices[i] + m_CornerExtended.corners[i].boneWeight1 *
				(m_pBones[m_CornerExtended.corners[i].boneIndex1 - 1].m_localMatrix * m_CornerExtended.corners[i].position);
		}
		if (m_CornerExtended.corners[i].boneIndex2)
		{
			m_Vertices[i] = m_Vertices[i] + m_CornerExtended.corners[i].boneWeight2 *
				(m_pBones[m_CornerExtended.corners[i].boneIndex2 - 1].m_localMatrix * m_CornerExtended.corners[i].position);
		}
		if (m_CornerExtended.corners[i].boneIndex3)
		{
			m_Vertices[i] = m_Vertices[i] + m_CornerExtended.corners[i].boneWeight3 *
				(m_pBones[m_CornerExtended.corners[i].boneIndex3 - 1].m_localMatrix * m_CornerExtended.corners[i].position);
		}
	}
}

CBone::CBone()
{
	name = NULL;
	m_pParent = NULL;
	m_pSibling = NULL;
	m_pChild = NULL;
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, m_localMatrix);
}

CBone::~CBone()
{
	/* FIX THIS!!
	if(m_pChild)
	{
		delete m_pChild;
		m_pChild = NULL;
	}

	if(m_pSibling)
	{
		delete m_pSibling;
		m_pSibling = NULL;
	}

	m_pParent = NULL;

	if(name)
		delete [] name;
	*/
}

void CBone::AddChild(CBone* pBone)
{
	if (m_pChild == NULL)
	{
		m_pChild = pBone;
	}
	else
	{
		CBone* pIter = m_pChild;
		while (pIter->m_pSibling != NULL)
		{
			pIter = pIter->m_pSibling;
		}
		pIter->m_pSibling = pBone;
	}

	// set the parent to this
	pBone->m_pParent = this;
}

void CBone::Render()
{
	// save original matrix before mucking with it
	glPushMatrix();

	//glLoadIdentity();

	// do rotation
	quaternion_t axisAngle;
	axisAngle = ConvertToAxisAngle(inverseRotation);
	//glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z);

	// do translation
	//glTranslatef(-inverseTranslation.x, -inverseTranslation.y, -inverseTranslation.z);

	axisAngle = ConvertToAxisAngle(localRotation);
	glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z);

	glTranslatef(localTranslation.x, localTranslation.y, localTranslation.z);

	//glMultMatrixf(m_localMatrix);

	// draw this node
	glBegin(GL_LINES);

	// x axis
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.05, 0.0, 0.0);

	// y axis
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.05, 0.0);

	// z axis
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.05);

	glEnd();

	// restore original matrix
	glPopMatrix();

	// draw children, if any
	if (m_pChild)
		m_pChild->Render();


	// draw siblings, if any
	if (m_pSibling)
		m_pSibling->Render();
}

void CBone::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	Log::Debug() << "BONE #" << id << endl;
	Log::Debug() << "name=" << name << endl;
	if (m_pParent)
		Log::Debug() << "m_pParent=" << m_pParent->id << endl;
	if (m_pSibling)
		Log::Debug() << "m_pSibling=" << m_pSibling->id << endl;
	if (m_pChild)
		Log::Debug() << "m_pChild=" << m_pChild->id << endl;
	Log::Debug() << "inverseRotation=(" << inverseRotation.w << "," << inverseRotation.x << "," << inverseRotation.y
		<< "," << inverseRotation.z << ")" << endl;
	Log::Debug() << "inverseTranslation=(" << inverseTranslation.x << "," << inverseTranslation.y
		<< "," << inverseTranslation.z << ")" << endl;
	Log::Debug() << "localRotation=(" << localRotation.w << "," << localRotation.x << "," << localRotation.y
		<< "," << localRotation.z << ")" << endl;
	Log::Debug() << "localTranslation=(" << localTranslation.x << "," << localTranslation.y
		<< "," << localTranslation.z << ")" << endl;
	Log::Debug() << "m_localMatrix=" << m_localMatrix[0] << "," << m_localMatrix[4] << "," << m_localMatrix[8]
		<< "," << m_localMatrix[12] << endl;
	Log::Debug() << "              " << m_localMatrix[1] << "," << m_localMatrix[5] << "," << m_localMatrix[9]
		<< "," << m_localMatrix[13] << endl;
	Log::Debug() << "              " << m_localMatrix[2] << "," << m_localMatrix[6] << "," << m_localMatrix[10]
		<< "," << m_localMatrix[14] << endl;
	Log::Debug() << "              " << m_localMatrix[3] << "," << m_localMatrix[7] << "," << m_localMatrix[11]
		<< "," << m_localMatrix[15] << endl;
}

CASPModel::CASPModel()
{
	m_pBones = nullptr;
	m_pSubMesh = nullptr;
	m_BoneInfo.positions = nullptr;
	m_pathTree.ParseNNKFile("namingkey101.nnk");
}

CASPModel::~CASPModel()
{
	if (m_pBones)
		delete[] m_pBones;

	if (m_pSubMesh)
		delete[] m_pSubMesh;

	if (m_BoneInfo.positions)
		delete[] m_BoneInfo.positions;
}

void CASPModel::Initialize(
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
}

bool CASPModel::Load(const char* filename)
{
	FILE* pFile;
	version_t version;

	//char *fullname = m_pathTree.GetFilePath(filename);

	std::string fullPath = m_strModelPath + filename;

	pFile = fopen(fullPath.c_str(), "rb");

	if (!pFile)
	{
		Log::Error() << "Couldn't open file: " << filename << endl;
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
	m_pSubMesh = new CSubMesh[m_header.numSubmeshes];

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
	m_pBones = new CBone[m_header.numBones];
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
			CBone* pParent = &m_pBones[m_pMeshBones[i].parentIndex];
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

void CASPModel::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	int i;
	Log::Debug() << "HEADER" << endl;
	Log::Debug() << "m_header.sizeTextField=" << m_header.sizeTextField << endl;
	Log::Debug() << "m_header.numBones=" << m_header.numBones << endl;
	Log::Debug() << "m_header.numMaterials=" << m_header.numMaterials << endl;
	Log::Debug() << "m_header.numVertices=" << m_header.numVertices << endl;
	Log::Debug() << "m_header.numSubmeshes=" << m_header.numSubmeshes << endl;
	Log::Debug() << "m_header.textField=";
	//Log::Debug() << "m_header.textField=" << m_header.textField << endl;
	for (i = 0; i < m_header.sizeTextField; ++i)
		Log::Debug() << m_header.textField[i];
	Log::Debug() << endl;

	Log::Debug() << "BONES" << endl;
	for (i = 0; i < m_header.numBones; ++i)
	{
		Log::Debug() << "name=" << &m_header.textField[m_pMeshBones[i].nameOffset] << endl;
		Log::Debug() << "m_pMeshBones[" << i << "].index=" << m_pMeshBones[i].index << endl;
		Log::Debug() << "m_pMeshBones[" << i << "].parentIndex=" << m_pMeshBones[i].parentIndex << endl;
		Log::Debug() << "m_pMeshBones[" << i << "].nameOffset=" << m_pMeshBones[i].nameOffset << endl;
	}

	for (i = 0; i < m_header.numSubmeshes; ++i)
	{
		Log::Debug() << "SUBMESH #" << i << endl;
		m_pSubMesh[i].Print();
	}

	Log::Debug() << "BONE INFO" << endl;
	Log::Debug() << "m_BoneInfo.numBones=" << m_BoneInfo.numBones << endl;
	for (i = 0; i < m_BoneInfo.numBones; ++i)
	{
		Log::Debug() << "m_BoneInfo.positions[" << i << "].inverseRotation=("
			<< m_BoneInfo.positions[i].inverseRotation.w << ","
			<< m_BoneInfo.positions[i].inverseRotation.x << ","
			<< m_BoneInfo.positions[i].inverseRotation.y << ","
			<< m_BoneInfo.positions[i].inverseRotation.z << ")" << endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].inverseTranslation=("
			<< m_BoneInfo.positions[i].inverseTranslation.x << ","
			<< m_BoneInfo.positions[i].inverseTranslation.y << ","
			<< m_BoneInfo.positions[i].inverseTranslation.z << ")" << endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].localRotation=("
			<< m_BoneInfo.positions[i].localRotation.w << ","
			<< m_BoneInfo.positions[i].localRotation.x << ","
			<< m_BoneInfo.positions[i].localRotation.y << ","
			<< m_BoneInfo.positions[i].localRotation.z << ")" << endl;
		Log::Debug() << "m_BoneInfo.positions[" << i << "].localTranslation=("
			<< m_BoneInfo.positions[i].localTranslation.x << ","
			<< m_BoneInfo.positions[i].localTranslation.y << ","
			<< m_BoneInfo.positions[i].localTranslation.z << ")" << endl;
	}

	Log::Debug() << "BOUNDING BOX" << endl;
	Log::Debug() << "m_BoundingBox.unknown=" << m_BoundingBox.unknown << endl;
}

void CASPModel::LoadTextures()
{
	int width, height;
	char filename[256];
	int index = 0;

	// lead each texture for each material
	for (int i = 0; i < m_header.numMaterials; ++i)
	{
		strcpy(filename, &m_header.textField[index]);
		// this increments on dword boundaries
		index += strlen(filename) + (4 - strlen(filename) % 4);
		strcat(filename, ".raw");

		std::string fullPath = m_strTexturePath + filename;

		char* buffer = LoadRawFile(fullPath.c_str(), width, height);

		glGenTextures(1, &tex[i]);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);

		delete[] buffer;
	}
}

void CASPModel::Render(int type)
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

void CASPModel::Update(long deltaTime)
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

void CASPModel::Interpolate(const float delta, CBone* pBone)
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

bool CASPModel::LoadAnimation(const char* filename)
{
	std::string fullPath = m_strAnimationPath + filename;
	return m_Anim.Load(fullPath.c_str());
}

void CASPModel::PrintBoneInfo()
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

