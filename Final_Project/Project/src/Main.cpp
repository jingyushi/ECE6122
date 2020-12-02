/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
The main function of the PacMan Game
*/
#include "ECE_Controller.h"

void displayCallBack()
{
	ECE_Display::getInstance()->display();
}

void reshapeCallBack(int width, int height)
{
	ECE_Display::getInstance()->reshape(width, height);
}

void specialCallBack(int key, int x, int y)
{
	ECE_Controller::getInstance()->keyBoardInput(key, x, y);
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
	ECE_Controller *controller = ECE_Controller::getInstance();
	glutMainLoop();
	return 0;
}