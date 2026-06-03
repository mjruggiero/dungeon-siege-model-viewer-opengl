#include "OrbitCamera.h"

#include <algorithm>
#include <gl/freeglut.h>

namespace
{
	constexpr float kOrbitDegreesPerPixel = 0.35f;
	constexpr float kPanUnitsPerPixel = 0.025f;
	constexpr float kMinPitchDegrees = -89.0f;
	constexpr float kMaxPitchDegrees = 89.0f;
	constexpr float kMinDistance = 1.0f;
	constexpr float kMaxDistance = 500.0f;
	constexpr float kZoomInMultiplier = 0.90f;
	constexpr float kZoomOutMultiplier = 1.10f;
}

void OrbitCamera::Reset()
{
	m_yawDegrees = 0.0f;
	m_pitchDegrees = 0.0f;
	m_distance = 10.0f;
	m_panX = 0.0f;
	m_panY = -3.0f;
	m_lastMouseX = 0;
	m_lastMouseY = 0;
	m_isOrbiting = false;
	m_isPanning = false;
}

void OrbitCamera::ApplyViewTransform() const
{
	glTranslatef(m_panX, m_panY, -m_distance);
	glRotatef(m_pitchDegrees, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yawDegrees, 0.0f, 1.0f, 0.0f);
}

void OrbitCamera::BeginMouseDrag(int button, int x, int y)
{
	m_lastMouseX = x;
	m_lastMouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		m_isOrbiting = true;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		m_isPanning = true;
	}
}

void OrbitCamera::EndMouseDrag(int button)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		m_isOrbiting = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		m_isPanning = false;
	}
}

void OrbitCamera::MouseMove(int x, int y)
{
	const int deltaX = x - m_lastMouseX;
	const int deltaY = y - m_lastMouseY;

	m_lastMouseX = x;
	m_lastMouseY = y;

	if (m_isOrbiting)
	{
		m_yawDegrees += static_cast<float>(deltaX) * kOrbitDegreesPerPixel;
		m_pitchDegrees += static_cast<float>(deltaY) * kOrbitDegreesPerPixel;
	}
	else if (m_isPanning)
	{
		m_panX += static_cast<float>(deltaX) * kPanUnitsPerPixel;
		m_panY -= static_cast<float>(deltaY) * kPanUnitsPerPixel;
	}

	ClampValues();
}

void OrbitCamera::ZoomSteps(int wheelSteps)
{
	if (wheelSteps > 0)
	{
		for (int i = 0; i < wheelSteps; ++i)
		{
			m_distance *= kZoomInMultiplier;
		}
	}
	else if (wheelSteps < 0)
	{
		for (int i = 0; i < -wheelSteps; ++i)
		{
			m_distance *= kZoomOutMultiplier;
		}
	}

	ClampValues();
}

void OrbitCamera::ClampValues()
{
	m_pitchDegrees = std::clamp(m_pitchDegrees, kMinPitchDegrees, kMaxPitchDegrees);
	m_distance = std::clamp(m_distance, kMinDistance, kMaxDistance);
}
