#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

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

	void MouseCallback(int button, int state, int x, int y)
	{
		g_application->Mouse(button, state, x, y);
	}

	void MotionCallback(int x, int y)
	{
		g_application->Motion(x, y);
	}

	void MouseWheelCallback(int wheel, int direction, int x, int y)
	{
		g_application->MouseWheel(wheel, direction, x, y);
	}

	void IdleCallback()
	{
		g_application->Idle();
	}
}

int main(int argc, char** argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
#endif

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1600, 900);
	const int windowId = glutCreateWindow("Dungeon Siege ASP Model Viewer");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	g_application = std::make_unique<ViewerApplication>();

	if (!g_application->Initialize())
	{
		Log::Error() << "Failed to initialize viewer application." << std::endl;
		g_application.reset();
		return EXIT_FAILURE;
	}

	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(IdleCallback);
	glutKeyboardFunc(KeyboardCallback);
	glutMouseFunc(MouseCallback);
	glutMotionFunc(MotionCallback);
	glutMouseWheelFunc(MouseWheelCallback);

	glutMainLoop();

	g_application.reset();
	glutDestroyWindow(windowId);

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return EXIT_SUCCESS;
}
