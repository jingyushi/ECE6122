/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
main function of the pacman game
*/

#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "ECE_Mobile.h"
#include "ECE_Ghost.h"
#include "ECE_Pacman.h"
#include "ECE_Wall.h"
#include "ECE_Coin.h"
#include <string.h>

using namespace std;

#define ESC 27

//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//----------------------------------------------------------------------
GLUquadricObj* quadratic{nullptr};
float fPacmanRadius = 0.5f;
float fGhostRadius = 0.5f;
float fCoinsRadius = 0.15f;
float fPowerUpHeight = 0.2f;
float fPowerUpRadius = 0.3f;
float fWallRadius = 0.1f;
float colorRed[4] = { 1.0f,0.0f,0.0f,1.0f };
float colorGreen[4] = { 0.0f,1.0f,0.0f,1.0f };
float colorBlue[4] = { 0.0f,0.75f,1.0f,1.0f };
float colorYellow[4] = { 1.0f,1.0f,0.0f,1.0f };
float colorSilver[4] = { 0.75f, 0.75f, 0.75f,1.0f };
float colorGolden[4] = { 1.0f, 0.85f, 0.0f,1.0f };
float colorOrange[4] = { 1.0f, 0.5f, 0.5f,1.0f };
float colorPink[4] = { 1.0f, 0.75f, 0.75f,1.0f };
GLfloat shininess[] = { 128 };


//----------------------------------------------------------------------
// Map
//
// The map is built with a 2d array. Each cell contains an integer from 0 to 18,
// representing 11 types of wall, a coin, a powerup coin, 4 ghosts and a pacman.
//
// Map can be easily load and edit through this representation
//----------------------------------------------------------------------
int map[22][19] =
{
	{2,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,3},
	{1,11,11,11,11,11,11,11,11,1,11,11,11,11,11,11,11,11,1},
	{1,12,2,3,11,2,0,3,11,1,11,2,0,3,11,2,3,12,1},
	{1,11,4,5,11,4,0,5,11,1,11,4,0,5,11,4,5,11,1},
	{1,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1},
	{1,11,0,0,11,1,11,0,0,7,0,0,11,1,11,0,0,11,1},
	{1,11,11,11,11,1,11,11,11,1,11,11,11,1,11,11,11,11,1},
	{4,0,0,3,11,9,0,0,13,1,13,0,0,8,11,2,0,0,5},
	{13,13,13,1,11,1,13,13,13,13,13,13,13,1,11,1,13,13,13},
	{0,0,0,5,11,1,13,2,0,14,0,3,13,1,11,4,0,0,0},
	{13,13,13,13,11,13,13,1,15,16,17,1,13,13,11,13,13,13,13},
	{0,0,0,3,11,1,13,4,0,0,0,5,13,1,11,2,0,0,0},
	{13,13,13,1,11,1,13,13,13,13,13,13,13,1,11,1,13,13,13},
	{2,0,0,5,11,1,13,0,0,7,0,0,13,1,11,4,0,0,3},
	{1,11,11,11,11,11,11,11,11,1,11,11,11,11,11,11,11,11,1},
	{1,11,0,3,11,0,0,0,11,1,11,0,0,0,11,2,0,11,1},
	{1,12,11,1,11,11,11,11,11,18,11,11,11,11,11,1,11,12,1},
	{9,0,11,1,11,1,11,0,0,7,0,0,11,1,11,1,11,0,8},
	{1,11,11,11,11,1,11,11,11,1,11,11,11,1,11,11,11,11,1},
	{1,11,0,0,0,6,0,0,11,1,11,0,0,6,0,0,0,11,1},
	{1,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5}
};



//setting the rotation angle and the playlist index
int sceneRotationAngle = 0;
float dis = 40.0f;
GLfloat position[] = { -80.0f, -80.0f, 80.0f, 1.0f };
GLuint wallInd;

//initializing player and four ghosts
ECE_Pacman player;
ECE_Ghost redGhost(1);
ECE_Ghost greenGhost(2);
ECE_Ghost orangeGhost(3);
ECE_Ghost pinkGhost(4);


//drawing one pacman
void drawPacman(ECE_Pacman pacman)
{
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
	glColor4f(colorYellow[0], colorYellow[1], colorYellow[2], colorYellow[3]);
	glPushMatrix();
	glTranslatef(pacman.x, pacman.y, 0);
	glutSolidSphere(fPacmanRadius, 20, 20);
	glPopMatrix();
}
//drawing one ghost
void drawGhost(ECE_Ghost ghost)
{
	float ghostColor[4];
	switch (ghost.color) {
	case 1:
		memcpy(ghostColor, colorRed, sizeof(colorRed));
		break;
	case 2:
		memcpy(ghostColor, colorGreen, sizeof(colorGreen));
		break;
	case 3:
		memcpy(ghostColor, colorOrange, sizeof(colorOrange));
		break;
	case 4:
		memcpy(ghostColor, colorPink, sizeof(colorPink));
		break;
	default:
		break;
	}
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ghostColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ghostColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ghostColor);
	glColor4f(ghostColor[0], ghostColor[1], ghostColor[2], ghostColor[3]);
	//head

	glPushMatrix();
	glTranslatef(ghost.x, ghost.y, 0);
	glutSolidSphere(fGhostRadius, 20, 20);
	glPopMatrix();
	//tail
	glPushMatrix();
	glTranslatef(ghost.x, ghost.y, -fGhostRadius);
	gluCylinder(quadratic, fGhostRadius, fGhostRadius, fGhostRadius, 20, 20); //consider drawing a solid circle at the bottom
	gluDisk(quadratic, 0.0, fGhostRadius, 20, 20);
	glPopMatrix();
}
//drawing one wall object
void drawWall(ECE_Wall wall)
{
	//There should be 11 types of walls in total
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
	glColor4f(colorBlue[0], colorBlue[1], colorBlue[2], colorBlue[3]);
	switch (wall.type) {

		//____
		//
	case 0:
		glPushMatrix();
		glTranslatef(wall.x - 0.5, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		break;
		//	|
		//	|
	case 1:
		glPushMatrix();
		glTranslatef(wall.x, wall.y + 0.5, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		break;
		//	__
		//	|
	case 2:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__
		//	|
	case 3:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//	|__
		//	  
	case 4:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__|
		//	  
	case 5:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__|__
		//	  
	case 6:
		glPushMatrix();
		glTranslatef(wall.x - 0.5, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__ __
		//	|  
	case 7:
		glPushMatrix();
		glTranslatef(wall.x - 0.5, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__|
		//	|  
	case 8:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y + 0.5, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//	|__
		//	|  
	case 9:
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 0.5, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y + 0.5, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
		//__|__
		//	|  
	case 10:
		glPushMatrix();
		glTranslatef(wall.x - 0.5, wall.y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y + 0.5, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, fWallRadius, fWallRadius, 1.0, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(wall.x, wall.y, 0);
		glutSolidSphere(fWallRadius, 20, 20);
		glPopMatrix();
		break;
	}
}
//drawing a coin
void drawCoin(ECE_Coin coin)
{
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorSilver);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorSilver);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
	glColor4f(colorSilver[0], colorSilver[1], colorSilver[2], colorSilver[3]);
	glPushMatrix();
	glTranslatef(coin.x, coin.y, 0.0f);
	glutSolidSphere(fCoinsRadius, 20, 20);
	glPopMatrix();
}
//drawing a powerup coin
void drawPowerup(ECE_Coin powerup)
{
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGolden);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorGolden);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorGolden);
	glColor4f(colorGolden[0], colorGolden[1], colorGolden[2], colorGolden[3]);
	glPushMatrix();
	glTranslatef(powerup.x, powerup.y, -fPowerUpHeight/2.0);
	gluCylinder(quadratic, fPowerUpRadius, fPowerUpRadius, fPowerUpHeight, 20, 20);
	gluDisk(quadratic, 0.0, fPowerUpRadius, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(powerup.x, powerup.y, fPowerUpHeight / 2.0);
	gluDisk(quadratic, 0.0, fPowerUpRadius, 20, 20);
	glPopMatrix();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	if (!quadratic)
		quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);

	wallInd = glGenLists(1);
	glNewList(wallInd, GL_COMPILE);
	//draw the walls here
	//Since Lab4 only requires static objects,
	//all objects are drawed in this list
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			float currX = float(j) - 9.0;
			float currY = 10.0 - float(i); // setting (10,9) as the center of the map
			ECE_Coin powerup;
			ECE_Wall wall;
			ECE_Coin coin;
			switch (map[i][j])
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				wall.setX(currX);
				wall.setY(currY);
				wall.setType(map[i][j]);
				drawWall(wall);
				break;
			case 11:
				coin.setX(currX);
				coin.setY(currY);
				coin.setType(false);
				drawCoin(coin);
				break;
			case 12:
				powerup.setX(currX);
				powerup.setY(currY);
				powerup.setType(true);
				drawPowerup(powerup);
				break;
			case 13:
				break;
			case 14:
				redGhost.setX(currX);
				redGhost.setY(currY);
				drawGhost(redGhost);
				break;
			case 15:
				greenGhost.setX(currX);
				greenGhost.setY(currY);
				drawGhost(greenGhost);
				break;
			case 16:
				orangeGhost.setX(currX);
				orangeGhost.setY(currY);
				drawGhost(orangeGhost);
				break;
			case 17:
				pinkGhost.setX(currX);
				pinkGhost.setY(currY);
				drawGhost(pinkGhost);
				break;
			case 18:
				player.setX(currX);
				player.setY(currY);
				drawPacman(player);
				break;
			default:
				break;
			}
		}
	}
	glEndList();
}

void display(void)
{
	int i, j;

	// Clear color and depth buffers
	glClearColor(0.0, 0.0, 0.0, 1.0); // black background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		0.85*dis, -dis, 0.75*dis, // camera location
		0.0, 0.0, 0.0,    // where camera is pointed
		0.0, 0.0, 1.0);      // landscape or portrait

	// Rotate the wholescene by an angle around the z-axis
	glRotatef(sceneRotationAngle, 0.0f, 0.0f, 1.0f);

	// Move the light with the scene
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	
	//draw here
	//Since Lab4 only requires static objects,
	//all objects are drawed in this list
	glCallList(wallInd);

	glutSwapBuffers(); // Make it all visible
}


// define simple keyboard callback for rotation and exiting
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
	case 'R':
		//
		sceneRotationAngle = (sceneRotationAngle + 5) % 360;
		glutPostRedisplay();
		break;
	case ESC:
	case 'q':
	case 'Q':
		exit(0);
	default:
		break;
	}
}

//TODO
void update(void)
{
	//
	glutPostRedisplay();
}

//window reshaping callback
void reshape(int w, int h)
{
	float ratio = ((float)w) / ((float)h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(20.0, ratio, 0.1, 100000.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	gluDeleteQuadric(quadratic);
	return 0;
}