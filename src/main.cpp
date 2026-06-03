#include <windows.h>
#include <gl/freeglut.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

#include "Log.h"
#include "NamingKey.h"
#include "ResourceResolver.h"
#include "ViewerConfig.h"
#include "Model.h"

ASPModel model;
float angle = 0.0f;
long startTime;

bool mode = 0;
bool bones = 0;
bool my_rotate = 0;
bool pause = 0;

namespace
{
	constexpr const char* kConfigPath = "config/viewer.properties";

	ViewerConfig g_config{};

	ResourceResolver BuildResourceResolver(const ViewerConfig& config)
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
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	g_config = ViewerConfig::LoadFromFile(kConfigPath);

	Log::SetDebugEnabled(g_config.enableDebugLogging);
	mode = g_config.startWireframe;
	pause = g_config.startPaused;
	my_rotate = g_config.startRotating;

	if (mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Log::Info() << "Using config file: " << kConfigPath << std::endl;
	Log::Info() << "Configured model: " << g_config.modelName << std::endl;
	Log::Info() << "Configured animation: " << g_config.animationName << std::endl;

	model.Initialize(BuildResourceResolver(g_config));

	if (!model.Load(g_config.modelName.c_str()))
	{
		exit(0);
	}

	if (!g_config.animationName.empty())
	{
		model.LoadAnimation(g_config.animationName.c_str());
	}

	startTime = GetTickCount();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0.0, -3.0, -10.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	//camera.PositionViewer();

	model.Render(GL_TRIANGLES);

	glFlush();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, w / (float)h, 1.0, 100000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'm' || key == 'M')
		mode = !mode;
	if (key == 'b' || key == 'B')
		bones = !bones;
	if (key == 'p' || key == 'P')
		pause = !pause;
	if (key == 'r' || key == 'R')
		my_rotate = !my_rotate;
	if (key == 'l' || key == 'L')
	{
		Log::SetDebugEnabled(!Log::IsDebugEnabled());
		Log::Info() << "Debug logging " << (Log::IsDebugEnabled() ? "enabled" : "disabled") << std::endl;
	}
	if (key == 'd' || key == 'D')
	{
		const bool wasDebugEnabled = Log::IsDebugEnabled();
		Log::SetDebugEnabled(true);
		model.PrintBoneInfo();
		Log::SetDebugEnabled(wasDebugEnabled);
	}

	if (mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void idle()
{
	if (my_rotate)
	{
		angle += 0.05f;
		if (angle > 360.0)
			angle -= 360.0;
	}
	long diff = GetTickCount() - startTime;
	if (!pause)
		model.Update(diff);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Dungeon Siege ASP Model Viewer");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
