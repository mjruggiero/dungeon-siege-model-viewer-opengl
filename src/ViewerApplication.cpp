#include "ViewerApplication.h"

#include "Log.h"
#include "NamingKey.h"
#include "ResourceResolver.h"

#include <windows.h>
#include <gl/freeglut.h>

#include <cstdlib>
#include <utility>

ResourceResolver ViewerApplication::BuildResourceResolver(const ViewerConfig& config) const
{
	ResourceResolver resolver{};
	resolver.SetContentBasePath(config.contentBasePath);
	resolver.SetArtBasePath(config.artBasePath);
	resolver.SetModelFallbackBasePath(config.modelFallbackBasePath);
	resolver.SetAnimationFallbackBasePath(config.animationFallbackBasePath);
	resolver.SetTextureFallbackBasePath(config.textureFallbackBasePath);

	if (!config.namingKeyPath.empty())
	{
		NamingKey namingKey = NamingKey::LoadFromFile(config.namingKeyPath);

		if (namingKey.IsLoaded())
		{
			Log::Info() << "Loaded NamingKey: " << config.namingKeyPath.string() << std::endl;
			resolver.SetNamingKey(std::move(namingKey));
		}
		else
		{
			Log::Warning() << "Unable to load NamingKey: "
				<< config.namingKeyPath.string() << std::endl;
		}
	}
	else
	{
		Log::Warning() << "No NamingKeyPath configured." << std::endl;
	}

	return resolver;
}

bool ViewerApplication::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	m_config = ViewerConfig::LoadFromFile(kConfigPath);

	Log::SetDebugEnabled(m_config.enableDebugLogging);

	m_wireframe = m_config.startWireframe;
	m_paused = m_config.startPaused;
	m_rotating = m_config.startRotating;

	ApplyPolygonMode();

	Log::Info() << "Using config file: " << kConfigPath << std::endl;
	Log::Info() << "Configured model: " << m_config.modelName << std::endl;
	Log::Info() << "Configured animation: " << m_config.animationName << std::endl;

	m_model.Initialize(BuildResourceResolver(m_config));

	if (!m_model.Load(m_config.modelName.c_str()))
	{
		return false;
	}

	if (!m_config.animationName.empty())
	{
		if (!m_model.LoadAnimation(m_config.animationName.c_str()))
		{
			return false;
		}
	}

	m_startTime = GetTickCount64();

	return true;
}

void ViewerApplication::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0.0f, -3.0f, -10.0f);
	glRotatef(m_angle, 0.0f, 1.0f, 0.0f);

	m_model.Render(GL_TRIANGLES, m_showBones);

	glFlush();
	glutSwapBuffers();
}

void ViewerApplication::Reshape(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, static_cast<float>(width) / static_cast<float>(height), 1.0, 100000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ViewerApplication::Keyboard(unsigned char key, int, int)
{
	switch (key)
	{
	case 'm':
	case 'M':
		m_wireframe = !m_wireframe;
		ApplyPolygonMode();
		break;

	case 'b':
	case 'B':
		m_showBones = !m_showBones;
		break;

	case 'p':
	case 'P':
		m_paused = !m_paused;
		break;

	case 'r':
	case 'R':
		m_rotating = !m_rotating;
		break;

	case 'l':
	case 'L':
		Log::SetDebugEnabled(!Log::IsDebugEnabled());
		Log::Info() << "Debug logging "
			<< (Log::IsDebugEnabled() ? "enabled" : "disabled")
			<< std::endl;
		break;

	case 'd':
	case 'D':
	{
		const bool wasDebugEnabled = Log::IsDebugEnabled();
		Log::SetDebugEnabled(true);
		m_model.PrintBoneInfo();
		Log::SetDebugEnabled(wasDebugEnabled);
		break;
	}

	case 27:
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}

void ViewerApplication::Idle()
{
	if (m_rotating)
	{
		m_angle += 0.05f;

		if (m_angle > 360.0f)
		{
			m_angle -= 360.0f;
		}
	}

	const unsigned long long elapsedTime = GetTickCount64() - m_startTime;

	if (!m_paused)
	{
		m_model.Update(static_cast<long>(elapsedTime));
	}

	glutPostRedisplay();
}

void ViewerApplication::ApplyPolygonMode() const
{
	if (m_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}