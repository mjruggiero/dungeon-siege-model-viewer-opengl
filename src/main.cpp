
#include <windows.h>
//#include <gl/glut.h>
#include <gl/freeglut.h>
#include <stdio.h>
#include "model.h"
#include "utilities.h"

CASPModel model;
float angle = 0.0f;
long startTime;

const char* path = "D:/Dungeon Siege/Objects/art/";

bool mode = 0;
bool bones = 0;
bool my_rotate = 0;
bool pause = 0;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	const char* basePath = "D:/data/dungeon-siege/art/";

	const char* modelPath = "meshes/characters/evil_b_bosses/dragon/";
	const char* animationPath = "animations/characters/evil_b_bosses/dragon/fs0/";
	const char* texturePath = "bitmaps/characters/evil_b_bosses/";

	//const char* modelPath = "meshes/characters/evil_d_monsters/drake/";
	//const char* animationPath = "animations/characters/evil_d_monsters/drake/fs0/";
	//const char* texturePath = "bitmaps/characters/evil_d_monsters/";

	model.Initialize(basePath, modelPath, animationPath, texturePath);

	// drake
	if (!model.Load("m_c_ebb_dg_pos_1.asp"))
		exit(0);

	model.LoadAnimation("a_c_ebb_dg_fs0_at.prs"); // attacking with breath
	//model.LoadAnimation("a_c_edm_dk_fs0_fl.prs"); // flying
	//model.LoadAnimation("a_c_edm_dk_fs0_ds.prs"); // standing

	// mucosa
	//model.Load("data/m_c_edm_mu_pos_1.asp");
	//model.LoadAnimation("data/a_c_edm_mu_fs0_dfs.prs");

	// hero male
	//model.Load("data/m_c_gah_fb_pos_a2.asp");
	//model.LoadAnimation("data/a_c_gah_fb_fs0_at.prs"); // punching
	//model.LoadAnimation("data/a_c_gah_fb_fs0_ds.prs"); // standing pose

	// hero female
	//model.Load("data/m_c_gah_fg_pos_a1.asp");
	//model.LoadAnimation("data/a_c_gah_fg_fs0_at.prs");

	// synge
	//model.Load("m_c_edm_sy_pos_1.asp");
	//model.LoadAnimation("a_c_edm_sy_fs0_at.prs");

	//model.m_Anim.Print();

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
	if (key == 'd' || key == 'D')
	{
		model.PrintBoneInfo();
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
