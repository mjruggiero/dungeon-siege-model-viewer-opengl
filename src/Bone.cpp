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
	glPushMatrix();

	// Match AspModel::Interpolate() transform order.
	glTranslatef(localTranslation.x, localTranslation.y, localTranslation.z);

	quaternion_t axisAngle = ConvertToAxisAngle(localRotation);
	if (axisAngle.w != 0.0f)
	{
		glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z);
	}

	// Draw local bone axes at this joint.
	glBegin(GL_LINES);

	// X axis - red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.10f, 0.0f, 0.0f);

	// Y axis - green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.10f, 0.0f);

	// Z axis - blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.10f);

	glEnd();

	// Draw connection lines from this bone to each direct child.
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 0.0f);

	for (Bone* child = m_pChild; child != nullptr; child = child->m_pSibling)
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(
			child->localTranslation.x,
			child->localTranslation.y,
			child->localTranslation.z);
	}

	glEnd();

	// Children must be rendered while this bone's transform is still active.
	if (m_pChild != nullptr)
	{
		m_pChild->Render();
	}

	glPopMatrix();

	// Siblings render after restoring this bone's parent transform.
	if (m_pSibling != nullptr)
	{
		m_pSibling->Render();
	}
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
