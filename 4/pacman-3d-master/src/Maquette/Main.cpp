#include "Controller.h"


void displayCallBack()
{
	View::getInstance()->display();
}

void reshapeCallBack(int width, int height)
{
	View::getInstance()->reshape(width, height);
}

void specialCallBack(int key, int x, int y)
{
	Controller::getInstance()->special(key, x, y);
}


int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600,900);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(displayCallBack);
	glutReshapeFunc(reshapeCallBack);
	glutSpecialFunc(specialCallBack);
	Controller *controller = Controller::getInstance();
	glutMainLoop();

	return 0;
}