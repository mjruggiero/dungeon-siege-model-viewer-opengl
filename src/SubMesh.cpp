#include "SubMesh.h"

#include "Log.h"

#include <GL/freeglut.h>

SubMesh::SubMesh()
{
	m_Material.materials = nullptr;
	m_Vertex.vertices = nullptr;
	m_Corner.corners = nullptr;
	m_Vertices = nullptr;
	m_CornerExtended.corners = nullptr;
	m_VertexMapping.vertexMaps = nullptr;
	m_Face.materials = nullptr;
	m_Face.faces = nullptr;
	m_VertexWeight.bones = nullptr;
	m_Stitches.stitches = nullptr;
}

SubMesh::~SubMesh()
{
	if (m_Material.materials)
		delete[] m_Material.materials;

	if (m_Vertex.vertices)
		delete[] m_Vertex.vertices;

	if (m_Corner.corners)
		delete[] m_Corner.corners;

	if (m_Vertices)
		delete[]m_Vertices;

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

bool SubMesh::Read(FILE* pFile, int numBones)
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
			m_VertexWeight.bones[i].vertices = nullptr;
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

void SubMesh::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	int i;
	Log::Debug() << "HEADER" << std::endl;
	Log::Debug() << "m_header.id=" << m_header.id << std::endl;
	Log::Debug() << "m_header.numMaterials=" << m_header.numMaterials << std::endl;
	Log::Debug() << "m_header.numVertices=" << m_header.numVertices << std::endl;
	Log::Debug() << "m_header.numCorners=" << m_header.numCorners << std::endl;
	Log::Debug() << "m_header.numFaces=" << m_header.numFaces << std::endl;

	Log::Debug() << "MATERIALS" << std::endl;
	Log::Debug() << "m_Material.numMaterials=" << m_Material.numMaterials << std::endl;
	for (i = 0; i < m_Material.numMaterials; ++i)
	{
		Log::Debug() << "m_Material.materials[" << i << "].index=" << m_Material.materials[i].index << std::endl;
		Log::Debug() << "m_Material.materials[" << i << "].numFaces=" << m_Material.materials[i].numFaces << std::endl;
	}

	Log::Debug() << "VERTEX" << std::endl;
	Log::Debug() << "m_Vertex.numVertices=" << m_Vertex.numVertices << std::endl;
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		Log::Debug() << "m_Vertex.vertices[" << i << "].=(" << m_Vertex.vertices[i].x << ","
			<< m_Vertex.vertices[i].y << ","
			<< m_Vertex.vertices[i].z << ")" << std::endl;
	}

	Log::Debug() << "CORNER" << std::endl;
	Log::Debug() << "m_Corner.numCorners=" << m_Corner.numCorners << std::endl;
	for (i = 0; i < m_Corner.numCorners; ++i)
	{
		Log::Debug() << "m_Corner.corners[" << i << "].vertexIndex=" << m_Corner.corners[i].vertexIndex << std::endl;
		Log::Debug() << "m_Corner.corners[" << i << "].normal=(" << m_Corner.corners[i].normal.x << ","
			<< m_Corner.corners[i].normal.x << ","
			<< m_Corner.corners[i].normal.x << ")" << std::endl;
		Log::Debug() << "m_Corner.corners[" << i << "].unknown1=" << m_Corner.corners[i].unknown1 << std::endl;
		Log::Debug() << "m_Corner.corners[" << i << "].unknown2=" << m_Corner.corners[i].unknown2 << std::endl;
		Log::Debug() << "m_Corner.corners[" << i << "].u=" << m_Corner.corners[i].u << std::endl;
		Log::Debug() << "m_Corner.corners[" << i << "].v=" << m_Corner.corners[i].v << std::endl;
	}

	Log::Debug() << "EXTENDED CORNER" << std::endl;
	Log::Debug() << "m_CornerExtended.numCorners=" << m_CornerExtended.numCorners << std::endl;
	for (i = 0; i < m_CornerExtended.numCorners; ++i)
	{
		Log::Debug() << "m_CornerExtended.corners[" << i << "].position=(" << m_CornerExtended.corners[i].position.x << ","
			<< m_CornerExtended.corners[i].position.y << ","
			<< m_CornerExtended.corners[i].position.z << ")" << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight0=" << m_CornerExtended.corners[i].boneWeight0 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight1=" << m_CornerExtended.corners[i].boneWeight1 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight2=" << m_CornerExtended.corners[i].boneWeight2 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneWeight3=" << m_CornerExtended.corners[i].boneWeight3 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex0=" << (short)m_CornerExtended.corners[i].boneIndex0 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex1=" << (short)m_CornerExtended.corners[i].boneIndex1 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex2=" << (short)m_CornerExtended.corners[i].boneIndex2 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].boneIndex3=" << (short)m_CornerExtended.corners[i].boneIndex3 << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].normal=(" << m_CornerExtended.corners[i].normal.x << ","
			<< m_CornerExtended.corners[i].normal.y << ","
			<< m_CornerExtended.corners[i].normal.z << ")" << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].unknown=" << m_CornerExtended.corners[i].unknown << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].u=" << m_CornerExtended.corners[i].u << std::endl;
		Log::Debug() << "m_CornerExtended.corners[" << i << "].v=" << m_CornerExtended.corners[i].v << std::endl;
	}

	Log::Debug() << "VERTEX MAPPING" << std::endl;
	for (i = 0; i < m_Vertex.numVertices; ++i)
	{
		Log::Debug() << "m_VertexMapping.vertexMaps[" << i << "].numCorners=" << m_VertexMapping.vertexMaps[i].numCorners << std::endl;
		Log::Debug() << "m_VertexMapping.vertexMaps[" << i << "].cornerIndices=";
		for (int j = 0; j < m_VertexMapping.vertexMaps[i].numCorners; ++j)
		{
			if (j)
				Log::Debug() << ",";
			Log::Debug() << m_VertexMapping.vertexMaps[i].cornerIndices[j];
		}
		Log::Debug() << std::endl;
	}

	Log::Debug() << "FACE" << std::endl;
	Log::Debug() << "m_Face.numFaces=" << m_Face.numFaces << std::endl;
	for (i = 0; i < m_Material.numMaterials; ++i)
	{
		Log::Debug() << "m_Faces.materials[" << i << "].index=" << m_Face.materials[i].index << std::endl;
		Log::Debug() << "m_Faces.materials[" << i << "].numFaces=" << m_Face.materials[i].numFaces << std::endl;
	}
	for (i = 0; i < m_Face.numFaces; ++i)
	{
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerA=" << m_Face.faces[i].indexCornerA << std::endl;
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerB=" << m_Face.faces[i].indexCornerB << std::endl;
		Log::Debug() << "m_Faces.faces[" << i << "].indexCornerC=" << m_Face.faces[i].indexCornerC << std::endl;
	}

	Log::Debug() << "VERTEX WEIGHTS" << std::endl;
	for (i = 0; i < m_numBones; ++i)
	{
		Log::Debug() << "m_VertexWeight.bones[" << i << "].numVertices=" << m_VertexWeight.bones[i].numVertices << std::endl;
		for (int j = 0; j < m_VertexWeight.bones[i].numVertices; ++j)
		{
			Log::Debug() << "m_VertexWeight.bones[" << i << "].vertices[" << j << "].index="
				<< m_VertexWeight.bones[i].vertices[j].index << std::endl;
			Log::Debug() << "m_VertexWeight.bones[" << i << "].vertices[" << j << "].weighting="
				<< m_VertexWeight.bones[i].vertices[j].weighting << std::endl;
		}
	}

	Log::Debug() << "STITCHES" << std::endl;
	//stch_t m_Stitches;
	Log::Debug() << "m_Stitches.x=" << m_Stitches.numStitches << std::endl;
	for (i = 0; i < m_Stitches.numStitches; ++i)
	{
		Log::Debug() << "m_Stitches.stitches[" << i << "].token=" << m_Stitches.stitches[i].token << std::endl;
		Log::Debug() << "m_Stitches.stitches[" << i << "].numVertices=" << m_Stitches.stitches[i].numVertices << std::endl;
		Log::Debug() << "m_Stitches.stitches[" << i << "].vertices=";
		for (int j = 0; j < m_Stitches.stitches[i].numVertices; ++j)
		{
			if (j)
				Log::Debug() << ",";
			Log::Debug() << m_Stitches.stitches[i].vertices[j];
		}
		Log::Debug() << std::endl;
	}
}

void SubMesh::Render(int type)
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

void SubMesh::Update()
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
