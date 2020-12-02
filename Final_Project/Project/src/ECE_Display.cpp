/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Display class
*/
#include "ECE_Display.h"

ECE_Display *ECE_Display::instance = 0;

ECE_Display::ECE_Display()
{
	myECE_Model = ECE_Model::getInstance();
	glClearColor(0.0,0.0,0.0,0);
	initDisplayLists();
}

ECE_Display::~ECE_Display()
{
	delete instance;
	delete myECE_Model;
}

ECE_Display *ECE_Display::getInstance()
{
	if(ECE_Display::instance == 0) {
		delete instance;
		instance = new ECE_Display();
	}
	return instance;
}

void ECE_Display::setInstance(ECE_Display * newECE_Display)
{
	instance = newECE_Display;
}

void ECE_Display::viewportECE_World()
{
	int heightInfos = h_global * Y_VIEWPORT;
	int widthControls = w_global * X_VIEWPORT_CONTROL;
	glViewport(0, heightInfos, w_global - widthControls, h_global - heightInfos);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int zoom = myECE_Model->getZoom();

	if (w_global == 0)
		w_global = 1;
	if (h_global == 0)
		h_global = 1;

	if (w_global <= h_global)
	{
		d1ECE_World = -zoom;
		delta1ECE_World = zoom - d1ECE_World;
		d2ECE_World = -zoom * h_global / w_global;
		delta2ECE_World = zoom * h_global / w_global - d2ECE_World;
	}
	else
	{
		d1ECE_World = -zoom * w_global / h_global;
		delta1ECE_World = zoom * w_global / h_global - d1ECE_World;
		d2ECE_World = -zoom;
		delta2ECE_World = zoom - d2ECE_World;
	}

	glOrtho(d1ECE_World, d1ECE_World + delta1ECE_World, d2ECE_World, d2ECE_World + delta2ECE_World, 1, 251);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double posXECE_PacMan = (((myECE_Model->getECE_PacMan()->getPosX() - (myECE_Model->getECE_World()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posYECE_PacMan = (((myECE_Model->getECE_PacMan()->getPosY() - (myECE_Model->getECE_World()->getMaxRows() / 2))) * SCALE) + 2.5;
	double radius = myECE_Model->getECE_PacMan()->getRadius();

	gluLookAt(myECE_Model->getCamPosX(), myECE_Model->getCamPosY(), myECE_Model->getCamPosZ(), 0, 0, 0, 0, 0, 1);
}

void ECE_Display::viewportInfos()
{
	GLdouble vp1 = w_global*X_VIEWPORT;
	GLdouble vp2 = 0;
	GLdouble w = w_global - vp1;
	GLdouble h = h_global*Y_VIEWPORT;

	GLdouble d1 = 0;
	GLdouble delta1 = 2500 - d1;
	GLdouble d2 = 0;
	GLdouble delta2 = 100 - d2;

	glViewport (vp1, vp2, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if((w/h) > (delta1/delta2)) 
	{
		gluOrtho2D(d1 - 0.5*((w * delta2/h) - delta1), d1+delta1+0.5*((w*delta2/h)-delta1), d2, d2 + delta2);
	}
	else 
	{ 
		gluOrtho2D(d1, d1 + delta1, d2 - 0.5*((h*delta1 / w) - delta2), d2 + delta2 + 0.5*((h*delta1 / w) - delta2));
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ECE_Display::viewportControls() 
{
	int heightInfos = h_global*Y_VIEWPORT;
	GLdouble vp1 = w_global - (w_global*X_VIEWPORT_CONTROL);
	GLdouble vp2 = heightInfos;
	GLdouble w = w_global*X_VIEWPORT_CONTROL;
	GLdouble h = h_global - heightInfos;
	GLdouble d1 = 0;
	GLdouble d2 = -50;
	GLdouble delta1 = 200 - d1;
	GLdouble delta2 = 200 - d2;

	glViewport(vp1, vp2, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if((w/h) > (delta1/delta2)) 
	{
		gluOrtho2D(d1 - 0.5*((w * delta2/h) - delta1), d1+delta1+0.5*((w*delta2/h)-delta1), d2, d2 + delta2);
		d1_real = d1 - 0.5*((w * delta2/h) - delta1);
		d2_real = d2;
		delta1_reel = d1+delta1+0.5*((w*delta2/h)-delta1) - d1_real;
		delta2_reel = d2 + delta2 - d2_real;
	}
	else 
	{ 
		gluOrtho2D(d1, d1 + delta1, d2 - 0.5*((h*delta1 / w) - delta2), d2 + delta2 + 0.5*((h*delta1 / w) - delta2));
		d1_real = d1;
		d2_real = d2 - 0.5*((h*delta1 / w) - delta2);
		delta1_reel = d1 + delta1 - d1_real;
		delta2_reel = d2 + delta2 + 0.5*((h*delta1 / w) - delta2) - d2_real;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ECE_Display::initECE_PacMan() {
	idList_ECE_PacMan=glGenLists(10);
	glNewList(idList_ECE_PacMan,GL_COMPILE);
	float colorYellow[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLfloat shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
	glColor4f(colorYellow[0], colorYellow[1], colorYellow[2], colorYellow[3]);
	glPushMatrix();
	glutSolidSphere(myECE_Model->getECE_PacMan()->getRadius(), 25, 15);
	glPopMatrix();
	glEndList();
}

void ECE_Display::initCoins()
{
	idList_Coin = idList_Mouse_Controls + 1;
	glNewList(idList_Coin, GL_COMPILE);
	float colorSilver[4] = { 0.75f, 0.75f, 0.75f,1.0f };
	GLfloat shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorSilver);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorSilver);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
	glColor4f(colorSilver[0], colorSilver[1], colorSilver[2], colorSilver[3]);
	glutSolidSphere(COIN_RADIUS, 20, 20);
	glEndList();
}

void ECE_Display::initSuperCoins() 
{
	idList_SuperCoin = idList_Coin + 1;
	glNewList(idList_SuperCoin, GL_COMPILE);
	float colorGolden[4] = { 1.0f, 0.85f, 0.0f,1.0f };
	GLfloat shininess[] = { 128 };
	GLUquadricObj* quadratic = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGolden);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorGolden);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorGolden);
	glColor4f(colorGolden[0], colorGolden[1], colorGolden[2], colorGolden[3]);

	glPushMatrix();
	glTranslatef(0,0, -POWERUP_HEIGHT / 2.0);
	gluCylinder(quadratic, POWERUP_RADIUS, POWERUP_RADIUS, POWERUP_HEIGHT, 20, 20);
	gluDisk(quadratic, 0.0, POWERUP_RADIUS, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0, POWERUP_HEIGHT / 2.0);
	gluDisk(quadratic, 0.0, POWERUP_RADIUS, 20, 20);
	glPopMatrix();
	gluDeleteQuadric(quadratic);
	glEndList();
}


void ECE_Display::initDisplayLists() 
{
	this->initECE_PacMan();
	this->initCoins();
	this->initSuperCoins();
}

void ECE_Display::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	viewportInfos();
	if(myECE_Model->getLifes() <= 0||myECE_Model->isWon())
	{
		myECE_Model->stopGame();
		gameOver();
	}
	else drawLifes();
	viewportControls();
	glEnable(GL_DEPTH_TEST);
	initLights();
	viewportECE_World(); 
	drawECE_World();
	drawCoins();
	drawECE_PacMan();
	drawECE_Ghosts();

	glutSwapBuffers();
}


void ECE_Display::drawLifes()
{
	glPushMatrix();
	stringstream txtLife;
	txtLife << "Lives Remaining : " << myECE_Model->getLifes();
	glColor3f(1, 1, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	for (unsigned int i = 0; i < txtLife.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtLife.str()[i]);
	}
	glPopMatrix();
}

void ECE_Display::reshape(int w, int h)
{	
	w_global=w;
	h_global=h;
}

void ECE_Display::gameOver() {
	glPushMatrix();
	stringstream txtGameOver;
	txtGameOver << "GAME OVER";
	glColor3f(0, 1, 1); 

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	for(unsigned int i = 0; i < txtGameOver.str().size(); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, txtGameOver.str()[i]);
	}
	glPopMatrix();
}

void ECE_Display::drawCoins()
{
	int x, y;
	int nMaxRows = myECE_Model->getECE_World()->getMaxRows();
	int nMaxCols = myECE_Model->getECE_World()->getMaxCols();
	ECE_Cell cell;
	float colorSilver[4] = { 0.75f, 0.75f, 0.75f,1.0f };
	float colorGolden[4] = { 1.0f, 0.85f, 0.0f,1.0f };
	GLfloat shininess[] = { 128 };
	GLUquadricObj* quadratic = gluNewQuadric();
	for (int i = 0; i < nMaxRows; i++)
	{
		for (int j = 0; j < nMaxCols; j++)
		{
			if (i == 0 || i == (nMaxRows - 1) || j == 0 || j == (nMaxCols - 1))
				continue;

			cell = *myECE_Model->getECE_World()->getMap()[i][j];
			x = j * SCALE;
			y = i * SCALE;
			x -= (myECE_Model->getECE_World()->getMaxCols() / 2) * SCALE;	// 4.5
			y -= (myECE_Model->getECE_World()->getMaxRows() / 2) * SCALE;	// 2.5
			x += 0.5 * SCALE;
			y += 0.5 * SCALE;
			if ((cell.getType() == COIN))
			{
				glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, colorSilver);
				glMaterialfv(GL_FRONT, GL_AMBIENT, colorSilver);
				glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
				glColor4f(colorSilver[0], colorSilver[1], colorSilver[2], colorSilver[3]);
				glPushMatrix();
				glTranslatef(x, -y, 0.0f);
				glutSolidSphere(COIN_RADIUS, 20, 20);
				glCallList(idList_Coin);
				glPopMatrix();
			}
			else if ((cell.getType()== POWER_UP))
			{
				glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGolden);
				glMaterialfv(GL_FRONT, GL_AMBIENT, colorGolden);
				glMaterialfv(GL_FRONT, GL_SPECULAR, colorGolden);
				glColor4f(colorGolden[0], colorGolden[1], colorGolden[2], colorGolden[3]);
				glPushMatrix();
				glTranslatef(x, -y,  -POWERUP_HEIGHT / 2.0);
				gluCylinder(quadratic, POWERUP_RADIUS, POWERUP_RADIUS, POWERUP_HEIGHT, 20, 20);
				gluDisk(quadratic, 0.0, POWERUP_RADIUS, 20, 20);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(x, -y, POWERUP_HEIGHT / 2.0);
				gluDisk(quadratic, 0.0, POWERUP_RADIUS, 20, 20);
				glCallList(idList_SuperCoin);
				glPopMatrix();
			}
		}
	}
	gluDeleteQuadric(quadratic);

}

void ECE_Display::drawECE_PacMan()
{
	int delta = myECE_Model->getECE_PacMan()->getDelta();

	if (delta != 0)
		delta -= SCALE;
	double posX = (((myECE_Model->getECE_PacMan()->getPosX() - (myECE_Model->getECE_World()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posY = (((myECE_Model->getECE_PacMan()->getPosY() - (myECE_Model->getECE_World()->getMaxRows() / 2))) * SCALE) + 2.5;
	posY = -posY;

	switch (myECE_Model->getECE_PacMan()->getDir())
	{
	case UP:
		posY += delta;
		break;
	case RIGHT:
		posX += delta;
		break;
	case DOWN:
		posY -= delta;
		break;
	case LEFT:
		posX -= delta;
		break;
	}
	float colorYellow[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLfloat shininess[] = { 128 };

	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
	glColor3fv(myECE_Model->getECE_PacMan()->getColor());//get the real-time color to tell super-mode or not
	glPushMatrix();
	glTranslatef(posX, posY, 0);
	glutSolidSphere(PACMAN_RADIUS, 20, 20);
	glPopMatrix();
}

void ECE_Display::drawECE_World()
{
	int maxRow = myECE_Model->getECE_World()->getMaxRows();
	int maxCol = myECE_Model->getECE_World()->getMaxCols();
	float colorBlue[4] = { 0.0f,0.75f,1.0f,1.0f };
	GLUquadricObj* quadratic = gluNewQuadric();
	GLfloat shininess[] = { 128 };

	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
	glColor4f(colorBlue[0], colorBlue[1], colorBlue[2], colorBlue[3]);
	glPushMatrix();
	glRotatef(180.0, 0.0, 0.0, 1.0);
	glTranslatef(-0.5 * SCALE, 0, 0);
	for(int i = 0; i < maxRow; i++) 
	{
		for(int j = 0; j < maxCol; j++) 
		{
			drawECE_Cell(*myECE_Model->getECE_World()->getMap()[i][j],i,j, quadratic);
		}
	}
	glPopMatrix();
	gluDeleteQuadric(quadratic);
}

//Note that there are ten types of wall in total
void ECE_Display::drawECE_Cell(ECE_Cell cell, int i, int j,GLUquadricObj* quadratic) 
{
	int x = cell.getX();
	int y = cell.getY();
	x = j * SCALE;
	y = i * SCALE;

	x -= (myECE_Model->getECE_World()->getMaxCols() / 2) * SCALE;
	y -= (myECE_Model->getECE_World()->getMaxRows() / 2) * SCALE;
	switch (cell.getType())
	{
	case 0:
		glPushMatrix();
		glTranslatef(x - 0.5*SCALE, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		break;
		//	|
		//	|
	case 1:
		glPushMatrix();
		glTranslatef(x, y + 0.5*SCALE, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		break;
		//	__
		//	|
	case 2:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__
		//	|
	case 3:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//	|__
		//	  
	case 4:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__|
		//	  
	case 5:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__|__
		//	  
	case 6:
		glPushMatrix();
		glTranslatef(x - 0.5*SCALE, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__ __
		//	|  
	case 7:
		glPushMatrix();
		glTranslatef(x - 0.5*SCALE, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__|
		//	|  
	case 8:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y + 0.5*SCALE, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//	|__
		//	|  
	case 9:
		glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, 0.5*SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y + 0.5*SCALE, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
		//__|__
		//	|  
	case 10:
		glPushMatrix();
		glTranslatef(x - 0.5*SCALE, y, 0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y + 0.5*SCALE, 0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, WALL_DEPTH, WALL_DEPTH, SCALE, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(x, y, 0);
		glutSolidSphere(WALL_DEPTH, 20, 20);
		glPopMatrix();
		break;
	default:
		break;
	}
}

void ECE_Display::drawECE_Ghosts() 
{
	float colorRed[4] = { 1.0f,0.0f,0.0f,1.0f };
	float colorGreen[4] = { 0.0f,1.0f,0.0f,1.0f };
	float colorOrange[4] = { 1.0f, 0.5f, 0.5f,1.0f };
	float colorPink[4] = { 1.0f, 0.75f, 0.75f,1.0f };
	float colorWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess[] = { 128 };
	float ghostColor[4];
	GLUquadricObj* quadratic = gluNewQuadric();
	if (myECE_Model->getECE_World()->getNumECE_Ghosts() == 0)
		return;
	for (int i = 0; i < myECE_Model->getECE_World()->getNumECE_Ghosts(); i++)
	{
		ECE_Ghost* g = myECE_Model->getECE_Ghost(i);

		float posX = (g->getPosX() * SCALE) - ((myECE_Model->getECE_World()->getMaxCols() / 2) * SCALE) + g->getRadius();
		float posY = -((g->getPosY() * SCALE) - ((myECE_Model->getECE_World()->getMaxRows() / 2) * SCALE) + g->getRadius());

		int delta = g->getDelta();

		delta -= SCALE;

		switch (g->getDir())
		{
		case UP:
			posY += delta;
			break;
		case RIGHT:
			posX += delta;
			break;
		case DOWN:
			posY -= delta;
			break;
		case LEFT:
			posX -= delta;
			break;
		}
		switch (i) {
		case 0:
			memcpy(ghostColor, colorRed, sizeof(colorRed));
			break;
		case 1:
			memcpy(ghostColor, colorGreen, sizeof(colorGreen));
			break;
		case 2:
			memcpy(ghostColor, colorOrange, sizeof(colorOrange));
			break;
		case 3:
			memcpy(ghostColor, colorPink, sizeof(colorPink));
			break;
		default:
			break;
		}
		if (!g->isAlive())
		{
			memcpy(ghostColor, colorWhite, sizeof(colorWhite));
		}
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ghostColor);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ghostColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ghostColor);
		glColor4f(ghostColor[0], ghostColor[1], ghostColor[2], ghostColor[3]);

		//head
		glPushMatrix();
		glTranslatef(0, 0, 0.2*SCALE);
		glPushMatrix();
		glTranslatef(posX, posY, 0);
		glutSolidSphere(GHOST_RADIUS, 20, 20);
		glPopMatrix();
		//tail
		glPushMatrix();
		glTranslatef(posX, posY, -GHOST_RADIUS);
		gluCylinder(quadratic, GHOST_RADIUS, GHOST_RADIUS, GHOST_RADIUS, 20, 20); //consider drawing a solid circle at the bottom
		gluDisk(quadratic, 0.0, GHOST_RADIUS, 20, 20);
		glPopMatrix();
		glPopMatrix();
	}
	gluDeleteQuadric(quadratic);
}

void ECE_Display::initLights()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	GLfloat position[] = {-(GLfloat)myECE_Model->getCamPosX(), -(GLfloat)myECE_Model->getCamPosY(), (GLfloat)myECE_Model->getCamPosZ(), 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}
