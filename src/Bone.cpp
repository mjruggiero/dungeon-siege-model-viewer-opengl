#include "Bone.h"

#include "Log.h"

#include <gl/freeglut.h>

Bone::Bone()
{
	name = nullptr;
	m_pParent = nullptr;
	m_pSibling = nullptr;
	m_pChild = nullptr;
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, m_localMatrix);
}

Bone::~Bone()
{
	delete[] name;
	name = nullptr;

	m_pParent = nullptr;
	m_pSibling = nullptr;
	m_pChild = nullptr;
}

void Bone::AddChild(Bone* pBone)
{
	if (m_pChild == nullptr)
	{
		m_pChild = pBone;
	}
	else
	{
		Bone* pIter = m_pChild;
		while (pIter->m_pSibling != nullptr)
		{
			pIter = pIter->m_pSibling;
		}
		pIter->m_pSibling = pBone;
	}

	// set the parent to this
	pBone->m_pParent = this;
}

void Bone::Render()
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
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.05f, 0.0f, 0.0f);

	// y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.05f, 0.0f);

	// z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.05f);

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

void Bone::Print()
{
	if (!Log::IsDebugEnabled())
	{
		return;
	}

	Log::Debug() << "BONE #" << id << std::endl;
	Log::Debug() << "name=" << name << std::endl;
	if (m_pParent)
		Log::Debug() << "m_pParent=" << m_pParent->id << std::endl;
	if (m_pSibling)
		Log::Debug() << "m_pSibling=" << m_pSibling->id << std::endl;
	if (m_pChild)
		Log::Debug() << "m_pChild=" << m_pChild->id << std::endl;
	Log::Debug() << "inverseRotation=(" << inverseRotation.w << "," << inverseRotation.x << "," << inverseRotation.y
		<< "," << inverseRotation.z << ")" << std::endl;
	Log::Debug() << "inverseTranslation=(" << inverseTranslation.x << "," << inverseTranslation.y
		<< "," << inverseTranslation.z << ")" << std::endl;
	Log::Debug() << "localRotation=(" << localRotation.w << "," << localRotation.x << "," << localRotation.y
		<< "," << localRotation.z << ")" << std::endl;
	Log::Debug() << "localTranslation=(" << localTranslation.x << "," << localTranslation.y
		<< "," << localTranslation.z << ")" << std::endl;
	Log::Debug() << "m_localMatrix=" << m_localMatrix[0] << "," << m_localMatrix[4] << "," << m_localMatrix[8]
		<< "," << m_localMatrix[12] << std::endl;
	Log::Debug() << "              " << m_localMatrix[1] << "," << m_localMatrix[5] << "," << m_localMatrix[9]
		<< "," << m_localMatrix[13] << std::endl;
	Log::Debug() << "              " << m_localMatrix[2] << "," << m_localMatrix[6] << "," << m_localMatrix[10]
		<< "," << m_localMatrix[14] << std::endl;
	Log::Debug() << "              " << m_localMatrix[3] << "," << m_localMatrix[7] << "," << m_localMatrix[11]
		<< "," << m_localMatrix[15] << std::endl;
}
