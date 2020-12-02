#include "View.h"

View *View::instance = 0;

View::View()
{
	model = Model::getInstance();
	glClearColor(0.0,0.0,0.0,0);
	time = 0.0;
	initDisplayLists();
}

View::~View()
{
	delete instance;
	delete model;
}

void View::redraw()
{
	glutPostRedisplay();
}

View *View::getInstance()
{
	if(View::instance == 0) {
		delete instance;
		instance = new View();
	}
	return instance;
}

void View::setInstance(View * newView)
{
	instance = newView;
}

void View::setHGlobal(int newHGlobal) {
	h_global = newHGlobal;
}

void View::setWGlobal(int newWGlobal) {
	w_global = newWGlobal;
}

void View::viewportWorld()
{
	int heightInfos = h_global * TAILLE_Y_VIEWPORT_INFOS;
	int widthControls = w_global * TAILLE_X_VIEWPORT_CONTROLS;
	glViewport(0, heightInfos, w_global - widthControls, h_global - heightInfos); // Taille et positionnement de la viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Initialisation de la matrice de projection
	int zoom = model->getZoom();
	if (model->isSpinning())
	{
		time += 1;
	}
	glRotated(time, 0, 0, 1);

	if (w_global == 0)
		w_global = 1;
	if (h_global == 0)
		h_global = 1;

	if (w_global <= h_global)
	{
		d1World = -zoom;
		delta1World = zoom - d1World;
		d2World = -zoom * h_global / w_global;
		delta2World = zoom * h_global / w_global - d2World;
	}
	else
	{
		d1World = -zoom * w_global / h_global;
		delta1World = zoom * w_global / h_global - d1World;
		d2World = -zoom;
		delta2World = zoom - d2World;
	}


	if (model->getProjectionMode() == PARALLEL_PROJECTION)
	{
		glOrtho(d1World, d1World + delta1World, d2World, d2World + delta2World, 1, 251);
	}
	else if (model->getProjectionMode() == PERSPECTIVE_PROJECTION)
	{
		double fieldView = zoom;
		double aspectRatio = (double)w_global / h_global;
		double near = 0.1;
		double far = 500.0;
		gluPerspective(fieldView, aspectRatio, near, far);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double posXPacMan = (((model->getPacMan()->getPosX() - (model->getWorld()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posYPacMan = (((model->getPacMan()->getPosY() - (model->getWorld()->getMaxRows() / 2))) * SCALE) + 2.5;
	double radius = model->getPacMan()->getRadius();

	int deltaCamX = 0;
	int deltaCamY = 0;

	int deltaPacX = 0;
	int deltaPacY = 0;
	float orientation_x = 0;
	float orientation_y = 0;

	int delta = model->getPacMan()->getDelta();

	if (delta == 0) delta = SCALE;

	switch (model->getPacMan()->getDir())
	{
	case PAC_NORTH:
		deltaCamY = -FPS + delta;
		deltaPacY = -delta;
		orientation_y = 1;
		break;
	case PAC_SOUTH:	// OK
		deltaCamY = FPS - delta;
		deltaPacY = delta;
		orientation_y = -1;
		break;
	case PAC_EAST:
		deltaCamX = -FPS + delta;
		deltaPacX = -delta;
		orientation_x = 1;
		break;
	case PAC_WEST: // OK
		deltaCamX = FPS - delta;
		deltaPacX = delta;
		orientation_x = -1;
		break;
	}
	gluLookAt(model->getCamPosX(), model->getCamPosY(), model->getCamPosZ(), 0, 0, 0, 1, 1, 1);
}

void View::viewportInfos()
{
	GLdouble vp1 = w_global*TAILLE_X_VIEWPORT_INFOS;
	GLdouble vp2 = 0;
	GLdouble w = w_global - vp1;
	GLdouble h = h_global*TAILLE_Y_VIEWPORT_INFOS;

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

void View::viewportControls() 
{
	int heightInfos = h_global*TAILLE_Y_VIEWPORT_INFOS;
	GLdouble vp1 = w_global - (w_global*TAILLE_X_VIEWPORT_CONTROLS);
	GLdouble vp2 = heightInfos;
	GLdouble w = w_global*TAILLE_X_VIEWPORT_CONTROLS;
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
		d1_reel = d1 - 0.5*((w * delta2/h) - delta1);
		d2_reel = d2;
		delta1_reel = d1+delta1+0.5*((w*delta2/h)-delta1) - d1_reel;
		delta2_reel = d2 + delta2 - d2_reel;
	}
	else 
	{ 
		gluOrtho2D(d1, d1 + delta1, d2 - 0.5*((h*delta1 / w) - delta2), d2 + delta2 + 0.5*((h*delta1 / w) - delta2));
		d1_reel = d1;
		d2_reel = d2 - 0.5*((h*delta1 / w) - delta2);
		delta1_reel = d1 + delta1 - d1_reel;
		delta2_reel = d2 + delta2 + 0.5*((h*delta1 / w) - delta2) - d2_reel;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Initialisation de la matrice de modelview
}

void View::initDlPacMan() {
	idList_PacMan=glGenLists(10);
	glNewList(idList_PacMan,GL_COMPILE);
	float colorYellow[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLfloat shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
	glColor4f(colorYellow[0], colorYellow[1], colorYellow[2], colorYellow[3]);
	glPushMatrix();
	glutSolidSphere(model->getPacMan()->getRadius(), 25, 15);
	glPopMatrix();
	glEndList();
}

void View::initDlPills()
{
	idList_Pill = idList_Mouse_Controls + 1;
	glNewList(idList_Pill, GL_COMPILE);
	float colorSilver[4] = { 0.75f, 0.75f, 0.75f,1.0f };
	GLfloat shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorSilver);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorSilver);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
	glColor4f(colorSilver[0], colorSilver[1], colorSilver[2], colorSilver[3]);
	glutSolidSphere(PILL_RADIUS, 20, 20);
	glEndList();
}

void View::initDlSuperPills() 
{
	idList_SuperPill = idList_Pill + 1;
	glNewList(idList_SuperPill, GL_COMPILE);
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


void View::initDisplayLists() 
{
	this->initDlPacMan();
	this->initDlPills();
	this->initDlSuperPills();
}

void View::drawHLine()
{
	GLfloat shininess[] = { 128 };
	float colorBlue[4] = { 0.0f,0.75f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
	glColor4f(colorBlue[0], colorBlue[1], colorBlue[2], colorBlue[3]);
	GLUquadricObj* quadratic = gluNewQuadric();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluCylinder(quadratic, WALL_DEPTH_RATIO*SCALE, WALL_DEPTH_RATIO * SCALE, 1.0*SCALE, 20, 20);
	gluDeleteQuadric(quadratic);
}

void View::drawWLine()
{
	GLfloat shininess[] = { 128 };
	float colorBlue[4] = { 0.0f,0.75f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
	glColor4f(colorBlue[0], colorBlue[1], colorBlue[2], colorBlue[3]);
	GLUquadricObj* quadratic = gluNewQuadric();
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluCylinder(quadratic, WALL_DEPTH_RATIO * SCALE, WALL_DEPTH_RATIO * SCALE, 1.0 * SCALE, 20, 20);
	gluDeleteQuadric(quadratic);
}

void View::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewportInfos();
	if(model->getLifes() <= 0||model->isWon())
	{
		model->stopGame();
		gameOver();
	}
	else drawLifes();

	viewportControls();
	glEnable(GL_DEPTH_TEST);

	initLights();

	viewportWorld(); 
	drawWorld();
	drawPills();
	drawPacMan();
	drawGhosts();

	glutSwapBuffers();
}


void View::drawLifes()
{
	glPushMatrix();
	stringstream txtLife;
	txtLife << "Lives Remaining : " << model->getLifes();
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

void View::reshape(int w, int h)
{	
	w_global=w;
	h_global=h;
}

void View::gameOver() {
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

void View::drawPills()
{
	int x, y;
	int maxRows = model->getWorld()->getMaxRows();
	int maxCols = model->getWorld()->getMaxCols();
	Cell cell;
	float colorSilver[4] = { 0.75f, 0.75f, 0.75f,1.0f };
	float colorGolden[4] = { 1.0f, 0.85f, 0.0f,1.0f };
	GLfloat shininess[] = { 128 };
	GLUquadricObj* quadratic = gluNewQuadric();
	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			if (i == 0 || i == (maxRows - 1) || j == 0 || j == (maxCols - 1))
				continue;

			cell = *model->getWorld()->getMap()[i][j];
			x = j * SCALE;
			y = i * SCALE;
			x -= (model->getWorld()->getMaxCols() / 2) * SCALE;	// 4.5
			y -= (model->getWorld()->getMaxRows() / 2) * SCALE;	// 2.5
			x += 0.5 * SCALE;
			y += 0.5 * SCALE;
			if (((cell.getType() & PILL) == PILL))
			{
				glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, colorSilver);
				glMaterialfv(GL_FRONT, GL_AMBIENT, colorSilver);
				glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
				glColor4f(colorSilver[0], colorSilver[1], colorSilver[2], colorSilver[3]);
				glPushMatrix();
				glTranslatef(x, -y, 0.0f);
				glutSolidSphere(PILL_RADIUS, 20, 20);
				glCallList(idList_Pill);
				glPopMatrix();
			}
			else if (((cell.getType() & SUPER_PILL) == SUPER_PILL))
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
				glCallList(idList_SuperPill);
				glPopMatrix();
			}
		}
	}

	gluDeleteQuadric(quadratic);

}

void View::drawPacMan()
{

	// Smooth du pacman
	int delta = model->getPacMan()->getDelta();

	if (delta != 0)
		delta -= SCALE;

	// on récupère les informations utiles pour dessiner pacman !
	double posX = (((model->getPacMan()->getPosX() - (model->getWorld()->getMaxCols() / 2))) * SCALE) + 2.5;
	double posY = (((model->getPacMan()->getPosY() - (model->getWorld()->getMaxRows() / 2))) * SCALE) + 2.5;
	posY = -posY;
	double start = 45 + (model->getPacMan()->getDir() * 90);
	double sweep = 270;

	int dying = model->getPacMan()->getDying();
	if (model->getPacMan()->getDying() >= 1)
	{
		model->getPacMan()->setRadius(model->getPacMan()->getRadius() - 0.25);
		model->getPacMan()->setDying(dying + 1);

		if (model->getPacMan()->getDying() > 5)
		{
			model->getPacMan()->setDying(10);
		}
	}

	// Gestion du smooth
	static int angleRotPacManX = 90;
	static int angleRotPacManZ = 0;

	switch (model->getPacMan()->getDir())
	{
	case PAC_NORTH:
		posY += delta;
		angleRotPacManZ = 270;
		break;
	case PAC_EAST:
		posX += delta;
		angleRotPacManZ = 180;
		break;
	case PAC_SOUTH:
		posY -= delta;
		angleRotPacManZ = 90;
		break;
	case PAC_WEST:
		posX -= delta;
		angleRotPacManZ = 0;
		break;
	}

	double radius = model->getPacMan()->getRadius();
	glPushMatrix();
	int topMouthAngle = 58;
	int bottomMouthAngle = 116;
	static int longsTop = 40;
	static int longsBottom = 40;

	// Mouvement de la bouche
	if (longsTop < 40) {
		longsTop += 10;
		longsBottom += 10;
	}
	else {
		longsTop -= 10;
		longsBottom -= 10;
	}

	//On trace le haut et le bas du PacMan
	glTranslated(posX, posY, radius);	// déplacement
	glTranslated(0, 0, 0.5);
	glScaled(0.6, 0.6, 0.6);
	glPushMatrix();
	glColor3fv(model->getPacMan()->getColor());
	glRotatef(angleRotPacManZ, 0.0, 0.0, 1.0);
	glRotatef(angleRotPacManX, 1.0, 0.0, 0.0);
	drawTopPacman(2, 40, longsTop);
	drawBottomPacman(2, 40, longsBottom);
	glPopMatrix();
	glPopMatrix();
}

void View::drawWorld()
{
	int maxRow = model->getWorld()->getMaxRows();
	int maxCol = model->getWorld()->getMaxCols();

	glColor3f(0, 0, 1);	
	for(int i = 0; i < maxRow; i++) 
	{
		for(int j = 0; j < maxCol; j++) 
		{
			if(i == 0 || j == 0 || i == maxRow-1 || j == maxCol-1) //border
				continue;

			drawCell(*model->getWorld()->getMap()[i][j],i,j);
		}
	}
}

void View::drawCell(Cell cell, int i, int j) 
{
	int x = cell.getX();
	int y = cell.getY();
	float x1, y1;
	Cell*** map = model->getWorld()->getMap();

	cell = *model->getWorld()->getMap()[i][j];
	x = j * SCALE;
	y = i * SCALE;

	x -= (model->getWorld()->getMaxCols() / 2) * SCALE;
	y -= (model->getWorld()->getMaxRows() / 2) * SCALE;

	if((cell.getType() & EMPTY) == EMPTY)
	{
		// top
		if((map[i-1][j]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawHLine();
			glPopMatrix();
		}

		// bottom
		if((map[i+1][j]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawHLine();
			glPopMatrix();
		}

		// right
		if((map[i][j+1]->getType() & HARD) == HARD)
		{
			x1 = x + SCALE;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawWLine();
			glPopMatrix();
		}

		// left
		if((map[i][j-1]->getType() & HARD) == HARD)
		{
			x1 = x;
			y1 = y + SCALE;
			glPushMatrix();
			glTranslated(x1, -y1, 0);
			drawWLine();
			glPopMatrix();
		}
	}
}

void View::drawGhosts() 
{
	// Si pas de fantômes on quitte la méthode drawGhosts()
	if (model->getWorld()->getNbGhosts() == 0)
		return;

	for (int i = 0; i < model->getWorld()->getNbGhosts(); i++)
	{
		Ghost* g = model->getGhost(i);

		float posX = (g->getPosX() * SCALE) - ((model->getWorld()->getMaxCols() / 2) * SCALE) + g->getRadius();
		float posY = -((g->getPosY() * SCALE) - ((model->getWorld()->getMaxRows() / 2) * SCALE) + g->getRadius());

		int delta = g->getDelta();

		delta -= SCALE;

		// Gestion du smooth
		switch (g->getDir())
		{
		case PAC_NORTH:
			posY += delta;
			break;
		case PAC_EAST:
			posX += delta;
			break;
		case PAC_SOUTH:
			posY -= delta;
			break;
		case PAC_WEST:
			posX -= delta;
			break;
		}
		glPushMatrix();
		GLUquadricObj* cylinder = gluNewQuadric();
		glTranslated(posX - 0.75, posY, g->getRadius());	// déplacement

		glColor3fv(model->getGhost(0)->getColor());
		glRotatef(90, 1.0, 0.0, 0.0);
		glScaled(GHOST_RADIUS, GHOST_RADIUS, GHOST_RADIUS);


		//Draw the base
		glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glutWireCone(0.3, 0.5, 10, 10);
		glutSolidCone(0.3, 0.5, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5, 0.0, 0.5);
		glRotatef(90, 1.0, 0.0, 0.0);
		glutSolidCone(0.3, 0.5, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.5, 0.0, -0.5);
		glRotatef(90, 1.0, 0.0, 0.0);
		glutSolidCone(0.3, 0.5, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.0, 0.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glutWireCone(0.3, 0.5, 10, 10);
		glutSolidCone(0.3, 0.5, 10, 10);
		glPopMatrix();


		//Draw the cylindrical body
		glPushMatrix();
		glTranslatef(0.5, 0.78, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		gluCylinder(cylinder, 0.8, 0.8, 0.8, 20, 20);
		glPopMatrix();

		//Draw the top spherical head
		glPushMatrix();
		glTranslatef(0.5, 0.8, 0.0);
		//glutWireSphere(0.8, 20, 20);
		glutSolidSphere(0.8, 20, 20);
		glPopMatrix();
		gluDeleteQuadric(cylinder);
		glPopMatrix();

	}
}

float * View::getNormal(float * p1, float * p2, float * p3){
	float xVertex[2];
	float yVertex[2];
	float zVertex[2];
	static float normale[3];

	xVertex[0] = p1[0]-p2[0];
	xVertex[1] = p2[0]-p3[0];
	yVertex[0] = p1[1]-p2[1];
	yVertex[1] = p2[1]-p3[1];
	zVertex[0] = p1[2]-p2[2];
	zVertex[1] = p2[2]-p3[2];
	normale[0] = yVertex[0]*zVertex[1] - zVertex[0]*yVertex[1];
	normale[1] = zVertex[0]*xVertex[1] - xVertex[0]*zVertex[1];
	normale[2] = xVertex[0]*yVertex[1] - yVertex[0]*xVertex[1];

	return normale;
}

void View::initLights()
{
	// Lumières 
	/* Pour la avoir un effet de shininess */
	float mat_specular[] = { 1, 1, 1, 1 };
	float mat_shininess[] = { 50 };

	/* Spot sur light1 */
	float light1_specular[] = { 1, 1, 1, 1 };
	float light1_ambient[] = { 0, 0, 0, 1 };
	float light1_diffuse[] = { 1, 1, 1, 1 };	// donner une couleur au spot
	float light1_position[] = { 0, 0, 15, 1 };	// position de la source de lumière, positionnelle
	float light1_direction[] = { 0, 0, 1 };	// direction du spot

	/* Light2 */
	float light2_position[] = { -10, 10, 10, 0 };	// position de la source de lumière, directionelle
	float light2_ambient[] = { 0.2, 0.2, 0.2, 1 };
	float light2_diffuse[] = { 0.7, 0.7, 0.7, 1 };
	float light2_specular[] = { 1, 1, 1, 1 };

	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); //active le test de profondeur
	glEnable(GL_LIGHTING); // on active l'éclairage
	glEnable(GL_LIGHT2); // on allume/active la lumière 2
	glEnable(GL_LIGHT3);
	//glDisable(GL_LIGHT1); //désactive la lumière1
	//glDisable(GL_LIGHT0); //désactive la lumière0

	glEnable(GL_COLOR_MATERIAL); //afficher les couleurs lorsque l'éclairage est activé

	/*On active la shininess(brilliance)*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	/*On initialise le spot*/
	glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular);
	//spot direction
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light1_direction);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 15.0);

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.02);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.0);

	/*Lumière 2*/
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

	/*Modele de lumiere*/
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

GLint View::getHGlobal()
{
	return h_global;
}

GLint View::getWGlobal()
{
	return w_global;
}

GLdouble View::getD1()
{
	return d1_reel;
}

GLdouble View::getD2()
{
	return d2_reel;
}

GLdouble View::getDelta1()
{
	return delta1_reel;
}

GLdouble View::getDelta2()
{
	return delta2_reel;
}

void View::resetSpin() 
{
	time = 0;
}

void View::drawTopPacman(double r, int lats, int longs)
{
	int i, j;
	for (i = 1; i <= lats; i++)
	{
		double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = r * sin(lat0);
		double zr0 = r * cos(lat0);

		double lat1 = PI * (-0.5 + (double)i / lats);
		double z1 = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs - 6; j++)
		{
			double lng = 2 * PI * (double)(j - 1) / longs / 2;
			double x = r * cos(lng);
			double y = r * sin(lng);

			glNormal3f(x * zr0, y * zr0, r * z0);
			glVertex3f(x * zr0, y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, r * z1);
			glVertex3f(x * zr1, y * zr1, r * z1);
		}
		glEnd();
	}
}

void View::drawBottomPacman(double r, int lats, int longs)
{
	int i, j;
	for (i = 1; i <= lats; i++)
	{
		double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = r * sin(lat0);
		double zr0 = r * cos(lat0);

		double lat1 = PI * (-0.5 + (double)i / lats);
		double z1 = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 1; j <= longs - 5; j++)
		{
			double lng = -2 * PI * (double)(j - 1) / longs / 2;
			double x = r * cos(lng);
			double y = r * sin(lng);

			glNormal3f(x * zr0, y * zr0, r * z0);
			glVertex3f(x * zr0, y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, r * z1);
			glVertex3f(x * zr1, y * zr1, r * z1);
		}
		glEnd();
	}
}