#pragma once

class OrbitCamera
{
public:
	void Reset();
	void ApplyViewTransform() const;

	void BeginMouseDrag(int button, int x, int y);
	void EndMouseDrag(int button);
	void MouseMove(int x, int y);
	void ZoomSteps(int wheelSteps);

private:
	void ClampValues();

private:
	float m_yawDegrees{};
	float m_pitchDegrees{};
	float m_distance{ 10.0f };
	float m_panX{};
	float m_panY{ -3.0f };

	int m_lastMouseX{};
	int m_lastMouseY{};
	bool m_isOrbiting{};
	bool m_isPanning{};
};
