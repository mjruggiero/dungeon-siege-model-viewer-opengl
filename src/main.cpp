#include "Log.h"
#include "ViewerApplication.h"

#include <gl/freeglut.h>

#include <cstdlib>
#include <memory>

namespace
{
	std::unique_ptr<ViewerApplication> g_application;

	void DisplayCallback()
	{
		g_application->Display();
	}

	void ReshapeCallback(int width, int height)
	{
		g_application->Reshape(width, height);
	}

	void KeyboardCallback(unsigned char key, int x, int y)
	{
		g_application->Keyboard(key, x, y);
	}

	void IdleCallback()
	{
		g_application->Idle();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Dungeon Siege ASP Model Viewer");

	g_application = std::make_unique<ViewerApplication>();

	if (!g_application->Initialize())
	{
		Log::Error() << "Failed to initialize viewer application." << std::endl;
		return EXIT_FAILURE;
	}

	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(IdleCallback);
	glutKeyboardFunc(KeyboardCallback);

	glutMainLoop();

	return EXIT_SUCCESS;
}
