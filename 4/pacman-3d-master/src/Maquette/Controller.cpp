#include "Controller.h"

Controller *Controller::instance=0;

void animateDisplayCallBack(int val)
{
	Controller::getInstance()->animateDisplay();
}

Controller::Controller() 
{
	model = Model::getInstance();
	
	View::getInstance()->setInstance(new View());
	view = View::getInstance();

	timeGhostInPrison = new clock_t[model->getWorld()->getNbGhosts()];

	glutTimerFunc(TEMPSBASE,animateDisplayCallBack,false); // Lance le "controleur" de l'animation
	stepOK = 0;
}

Controller::~Controller()
{
	delete model;
	delete view;
}

Controller *Controller::getInstance()
{
	if (instance == 0)
		instance = new Controller();

	return instance;
}

//void Controller::togglePacManViewMode() {
//	model->setPacManViewMode(!model->isInPacManViewMode());
//}

void Controller::changeDirGhost(Ghost* g, int dir, int nbAvailables, int* availables) {
	int nextDir;
	switch(dir) {
	case PAC_SOUTH:
		nextDir = PAC_NORTH;
		break;
	case PAC_NORTH:
		nextDir = PAC_SOUTH;
		break;
	case PAC_EAST:
		nextDir = PAC_WEST;
		break;
	case PAC_WEST:
		nextDir = PAC_EAST;
		break;
	}
	bool dontChange = false; 
	if(g->getDir() == nextDir)
	{
		for(int k = 0; k < nbAvailables; k++)
		{
			if(availables[k] == nextDir)
			{
				dontChange = true;
				break;
			}
		}
	}

	if(!dontChange)
		g->setDir(dir);
}

void Controller::handleCollisions(int numGhost, Ghost* g) {
	PacMan * p = model->getPacMan();
			
	if(g->getPosX() == p->getPosX() && g->getPosY() == p->getPosY()) 
	{
		if(model->getPacMan()->getMode())
		{
			// returning to origin
			g->setRunning(false);
			g->setDelta(SCALE);
			g->setPosX(g->getInitPosX());
			g->setPosY(g->getInitPosY());

			timeGhostInPrison[numGhost] = clock();
		}
		else if(!(p->getPosX() == p->getInitPosX() && p->getPosY() == p->getInitPosY()))
		{
			model->pacManDie();

			// resetting ghosts after pacman dies
			for(int i=0; i < model->getWorld()->getNbGhosts(); i++) 
			{
				Ghost* ghost = model->getGhost(i);
				ghost->setPosX(ghost->getInitPosX());
				ghost->setPosY(ghost->getInitPosY());
				ghost->setRunning(false);
				timeGhostInPrison[i] = clock();
				ghost->setDelta(SCALE);
			}
		}
	}
}

void Controller::animateGhosts()
{				
	if(model->getWorld()->getNbGhosts() == 0) 
		return;

	for(int i = 0; i < model->getWorld()->getNbGhosts(); i++)
	{
		Ghost * g = model->getGhost(i);

		if(((clock()-timeGhostInPrison[i]) / CLOCKS_PER_SEC) > GHOSTS_PRISON_TIME)
		{
			g->setRunning(true);
		}

		if(g->isRunning())
		{
			// Smooth
			g->setDelta(g->getDelta() + 1);

			// do not exceed SCALE
			if(g->getDelta() >= SCALE)
				g->setDelta(0);

			if(g->getDelta() == 0)
			{
				int x = g->getPosX();
				int y = g->getPosY();

				int available = 0;

				int * availables = new int[4];
				int nbAvailables = 0;


				// right
				if(x + 1 < model->getWorld()->getMaxCols() && (model->getWorld()->getMap()[y][x+1]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_EAST;
				}
				//left
				if(x - 1 > 0 && (model->getWorld()->getMap()[y][x-1]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_WEST;
				}
				//up
				if(y - 1 > 0 && (model->getWorld()->getMap()[y-1][x]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_NORTH;
				}
				//down
				if(y + 1 < model->getWorld()->getMaxRows() && (model->getWorld()->getMap()[y+1][x]->getType() & EMPTY) == EMPTY)
				{
					availables[nbAvailables++] = PAC_SOUTH;
				}

				
				int newDir = availables[rand() % nbAvailables];

				this->changeDirGhost(g, newDir, nbAvailables, availables);
				switch(g->getDir())
				{
				case PAC_NORTH:
					g->setPosY(y-1);
					break;
				case PAC_EAST:
					g->setPosX(x+1);
					break;
				case PAC_SOUTH:
					g->setPosY(y+1);
					break;
				case PAC_WEST:
					g->setPosX(x-1);
					break;
				}
			}
			this->handleCollisions(i, g);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
void Controller::handleStargate(int iX, int iY, int maxRow, int maxCol, int* direction)
{
	stargate = false;

	if(!stargate && (iX == 0 || iY == 0)) 
	{
		model->getPacMan()->setRunning(false);
		*direction=PAC_EAST;
	}
	if(!stargate && (iX == maxCol-1 || iY == maxRow-1)) 
	{
		model->getPacMan()->setRunning(false);
		*direction=PAC_WEST; 
	}

	if(stargate && (iX == 1 || iY == 1)) 
	{
		stargate = false;
	}
	if(stargate && (iX == maxCol-2 || iY == maxRow-2)) 
	{
		stargate = false;
	}
}

void Controller::changeDirPacMan(int nextDir, bool changeDir)
{
	int iX = model->getPacMan()->getPosX();
	int iY = model->getPacMan()->getPosY();
	Cell*** map = model->getWorld()->getMap();

	switch(nextDir) 
	{
		case PAC_NORTH: 
			if((map[iY-1][iX]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_EAST:
			if((map[iY][iX+1]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_WEST:
			if((map[iY][iX-1]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
		case PAC_SOUTH:
			if((map[iY+1][iX]->getType() & EMPTY) == EMPTY)
				changeDir = true;
			break;
	}

	if(changeDir) 
	{
		model->getPacMan()->setDir(nextDir);
		model->getPacMan()->setNextDir(-1);
	}
}

void Controller::handleChangeDirPacMan()
{
	float step = 1; 
	int iX = model->getPacMan()->getPosX();
	int iY = model->getPacMan()->getPosY();

	int direction = model->getPacMan()->getDir();
	int nextDir = model->getPacMan()->getNextDir();

	int maxRow = model->getWorld()->getMaxRows();
	int maxCol = model->getWorld()->getMaxCols();

	this->handleStargate(iX, iY, maxRow, maxCol, &direction);

	Cell*** map = model->getWorld()->getMap();

	if(model->getPacMan()->getDelta() == 0)
	{
		switch(direction)
		{
		case PAC_NORTH:
			if((map[iY-1][iX]->getType() & EMPTY) == EMPTY)
			{
				if((iY-1) == 0) { 
					if((map[maxRow-1][iX]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosY(maxRow-1);
						stargate = true;
					}
				}
				model->getPacMan()->setPosY(model->getPacMan()->getPosY() - step);
			} else
				model->getPacMan()->setRunning(false);
			break;
		case PAC_EAST:
			if((map[iY][iX+1]->getType() & EMPTY) == EMPTY) 
			{
				if((iX+2) == maxCol)
				{ 
					if((map[iY][0]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosX(0);
						stargate = true;
					}
				}
				model->getPacMan()->setPosX(model->getPacMan()->getPosX() + step);
			}
			else 
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		case PAC_SOUTH:
			if((map[iY+1][iX]->getType() & EMPTY) == EMPTY) 
			{
				if((iY+2) == maxRow)
				{
					if((map[0][iX]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosY(0);
						stargate = true;
					}
				}
				model->getPacMan()->setPosY(model->getPacMan()->getPosY() + step);
			}
			else 
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		case PAC_WEST:
			if((map[iY][iX-1]->getType() & EMPTY) == EMPTY) 
			{
				if((iX-1) == 0)
				{
					if((map[iY][maxCol-1]->getType() & EMPTY) == EMPTY) 
					{
						model->getPacMan()->setPosX(maxCol-1);
						stargate = true;
					}
				}

				model->getPacMan()->setPosX(model->getPacMan()->getPosX() - step);
			}
			else
			{
				model->getPacMan()->setRunning(false);
			}
			break;
		}
	}
	
	bool changeDir = false;
	if(model->getPacMan()->getDelta() == (SCALE - 1) && nextDir != -1 && nextDir != direction) 
	{
		this->changeDirPacMan(nextDir, changeDir);
	}
}

void Controller::switchSuperMode()
{
	if(model->getPacMan()->getMode() == SUPER)
	{
		model->getPacMan()->couleurAleatoire();
		if(((clock()-timeModeSuper) / CLOCKS_PER_SEC) > SUPER_TIME)
			model->getPacMan()->animSuperMode(NORMAL);
	}
}

void Controller::handlePills() 
{
	int nextX = model->getPacMan()->getPosX();
	int nextY = model->getPacMan()->getPosY();
	Cell*** map = model->getWorld()->getMap();

	if(model->getPacMan()->isRunning() && ((map[nextY][nextX]->getType() & PILL) == PILL || (map[nextY][nextX]->getType() & SUPER_PILL) == SUPER_PILL)) 
	{
		if(map[nextY][nextX]->getType() == SUPER_PILL) 
		{
			int nbSuperPills = model->getWorld()->getNbSuperPills();
			model->getWorld()->setNbSuperPills(nbSuperPills - 1);
			timeModeSuper = clock();
			model->getPacMan()->animSuperMode(SUPER);
		}
		else 
		{
			int nbPills = model->getWorld()->getNbPills();
			model->getWorld()->setNbPills(nbPills - 1);
		}
		map[nextY][nextX]->setType(EMPTY);
	}
///////////////////////////////////////////////////////////////////////////////////////////
	int nbPills = model->getWorld()->getNbPills();
	int nbSuperPills = model->getWorld()->getNbSuperPills();
	if(nbPills == 0 && nbSuperPills == 0) 
	{
		model->getPacMan()->setRunning(false);
		model->stopGame();
		model->gameWon();
	}
}

void Controller::handlePacManSmooth()
{
	if(model->getPacMan()->isRunning()) 
	{
		model->getPacMan()->setDelta(model->getPacMan()->getDelta() + 1);
		if(model->getPacMan()->getDelta() >= SCALE)
			model->getPacMan()->setDelta(0);
	} 
	else 
	{
		model->getPacMan()->setDelta(0);
	}
}

void Controller::animateDisplay() 
{
	if(model->isStarted())
	{
		animateGhosts();
	}
	switchSuperMode();

	if(model->getPacMan()->isRunning() && model->isStarted()) 
	{
		bool changeDir = false;
		this->handleChangeDirPacMan();
		this->handlePills();
		this->handlePacManSmooth();
	}
	View::getInstance()->display(); 
	glutTimerFunc(TEMPSBASE,animateDisplayCallBack,false);
}

void Controller::handleNewDir(int newDir) {
	if(newDir != -1)
	{
		if(model->getLifes() >= 0) {
			model->startGame();	
		}

		if(!model->getPacMan()->isRunning()) 
		{
			model->getPacMan()->setRunning(true);
			model->getPacMan()->setDir(newDir);
		}
		model->getPacMan()->setNextDir(newDir);
	}
}

void Controller::special(int key, int x, int y)
{
	int newDir = -1;

	switch(key) 
	{
	case GLUT_KEY_DOWN:
		newDir = PAC_SOUTH;
		break;
	case GLUT_KEY_UP: 
		newDir = PAC_NORTH;
		break;
	case GLUT_KEY_LEFT:
		newDir = PAC_WEST;
		break;
	case GLUT_KEY_RIGHT:
		newDir = PAC_EAST;
		break;
	case GLUT_KEY_F1:
		model->incCamPosX(SCALE);
		break;
	case GLUT_KEY_F2:
		model->incCamPosX(-SCALE);
		break;
	case GLUT_KEY_F3:
		model->zoomInOut(ZOOM_DEFAULT);
		break;
	case GLUT_KEY_F4:
		model->zoomInOut(-ZOOM_DEFAULT);
		break;
	case GLUT_KEY_HOME:
		model->incCamPosY(SCALE);
		break;
	case GLUT_KEY_END:
		model->incCamPosY(-SCALE);
		break;
	case GLUT_KEY_PAGE_UP:
		model->incCamPosZ(SCALE);
		break;
	case GLUT_KEY_PAGE_DOWN:
		model->incCamPosZ(-SCALE);
		break;
	case GLUT_KEY_INSERT:
		model->reinitZoom();
		model->reinitPosCam();
		break;
	case GLUT_KEY_F5:
		model->getPacMan()->setRunning(false);
		model->reset();
	}
	this->handleNewDir(newDir);
}
