#pragma once

#include "AspModel.h"
#include "ViewerConfig.h"

#include <string>

class ViewerApplication
{
public:
	bool Initialize();

	void Display();
	void Reshape(int width, int height);
	void Keyboard(unsigned char key, int x, int y);
	void Idle();

private:
	static constexpr const char* kConfigPath = "config/viewer.properties";

	ResourceResolver BuildResourceResolver(const ViewerConfig& config) const;
	void ApplyPolygonMode() const;

private:
	ViewerConfig m_config{};
	AspModel m_model{};

	float m_angle{};
	unsigned long long m_startTime{};

	bool m_wireframe{};
	bool m_showBones{};
	bool m_rotating{};
	bool m_paused{};
};
