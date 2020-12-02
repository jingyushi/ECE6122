/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Controller class
*/
#include "ECE_Controller.h"

ECE_Controller *ECE_Controller::instance=0;
void animateECE_DisplayCallBack(int val)
{
	ECE_Controller::getInstance()->display();
}

ECE_Controller::ECE_Controller() 
{
	myECE_Model = ECE_Model::getInstance();
	
	ECE_Display::getInstance()->setInstance(new ECE_Display());
	myECE_Display = ECE_Display::getInstance();

	timeECE_GhostInPrison = new clock_t[myECE_Model->getECE_World()->getNumECE_Ghosts()];
	lastTimeECE_GhostRelease = clock() - CLOCKS_PER_SEC * GHOSTS_SPAWN_SLOT;
	glutTimerFunc(TIMESTEP,animateECE_DisplayCallBack,false);
}

ECE_Controller::~ECE_Controller()
{
	delete myECE_Model;
	delete myECE_Display;
}

ECE_Controller *ECE_Controller::getInstance()
{
	if (instance == 0) instance = new ECE_Controller();
	return instance;
}

void ECE_Controller::ghostCollisions(int numECE_Ghost, ECE_Ghost* g) {
	ECE_PacMan * p = myECE_Model->getECE_PacMan();
			
	if(g->getPosX() == p->getPosX() && g->getPosY() == p->getPosY()) 
	{
		if(myECE_Model->getECE_PacMan()->getMode())
		{
			// returning to origin
			g->setRunning(false);
			g->setAlive(false);
			g->setDelta(SCALE);
			g->setPosX(g->getInitPosX());
			g->setPosY(g->getInitPosY());

			timeECE_GhostInPrison[numECE_Ghost] = clock();
		}
		else if(!(p->getPosX() == p->getInitPosX() && p->getPosY() == p->getInitPosY()))
		{
			myECE_Model->pacManDie();

			// resetting ghosts after pacman dies
			for(int i=0; i < myECE_Model->getECE_World()->getNumECE_Ghosts(); i++) 
			{
				ECE_Ghost* ghost = myECE_Model->getECE_Ghost(i);
				ghost->setPosX(ghost->getInitPosX());
				ghost->setPosY(ghost->getInitPosY());
				ghost->setRunning(false);
				ghost->setAlive(true);
				timeECE_GhostInPrison[i] = clock();
				ghost->setDelta(SCALE);
			}
		}
	}
}
//This is the BFS algorithm used for the ghosts to navigate along the shortest path towards the PacMan
//A queue is created to store all the neighbor points and a map is created to store the visited points, until the shortest path is found
ECE_Cell* ECE_Controller::BFS(ECE_Cell*** currentMap, int sourceX, int sourceY, int targetX, int targetY,int nMaxCols,int nMaxRows)
{
	queue<ECE_Cell*> Q;
	ECE_Cell* source = currentMap[sourceY][sourceX];
	ECE_Cell* target = currentMap[targetY][targetX];
	Q.push(source);
	map<ECE_Cell*, ECE_Cell*> V;
	V[source]= nullptr;
	while (!Q.empty())
	{
		ECE_Cell* curr = Q.front();
		Q.pop();
		///////////////////////////////
		if (curr == target)
		///////////////////
		{
			return getShortestDir(V, curr, sourceX, sourceY);
		}
		int x = curr->getPosX();
		int y = curr->getPosY();
		if (x + 1 < nMaxCols && currentMap[y][x + 1]->getType() > 10)
		{
			if (!V.count(currentMap[y][x + 1]))
			{
				V[currentMap[y][x + 1]] = curr;
				Q.push(currentMap[y][x + 1]);
			}
		}
		if (x - 1 > 0 && currentMap[y][x - 1]->getType() > 10)
		{
			if (!V.count(currentMap[y][x - 1]))
			{
				V[currentMap[y][x - 1]] = curr;
				Q.push(currentMap[y][x - 1]);
			}
		}
		if (y - 1 > 0 && currentMap[y - 1][x]->getType() > 10)
		{
			if (!V.count(currentMap[y-1][x]))
			{
				V[currentMap[y-1][x]] = curr;
				Q.push(currentMap[y-1][x]);
			}
		}
		if (y + 1 < nMaxRows && currentMap[y + 1][x]->getType() > 10)
		{
			if (!V.count(currentMap[y+1][x]))
			{
				V[currentMap[y+1][x]] = curr;
				Q.push(currentMap[y+1][x]);
			}
		}
	}
	return nullptr;
}
ECE_Cell* ECE_Controller::getShortestDir(map<ECE_Cell*, ECE_Cell*> myMap, ECE_Cell* target, int sourceX, int sourceY)
{
	list<ECE_Cell*> path;
	ECE_Cell* curr = target;
	while (curr != nullptr)
	{
		path.push_back(curr);
		curr = myMap[curr];
	}
	path.pop_back();
	if (path.empty()) return nullptr;
	return path.back();
}

void ECE_Controller::moveECE_Ghosts()
{				
	if(myECE_Model->getECE_World()->getNumECE_Ghosts() == 0) 
		return;
	ECE_PacMan* p = myECE_Model->getECE_PacMan();
	int nPacX = p->getPosX();
	int nPacY = p->getPosY();
	for(int i = 0; i < myECE_Model->getECE_World()->getNumECE_Ghosts(); i++)
	{
		ECE_Ghost * g = myECE_Model->getECE_Ghost(i);
		clock_t currentTime = clock();
		if((((currentTime -timeECE_GhostInPrison[i]) / CLOCKS_PER_SEC) > GHOSTS_FREEZE_TIME)
			&&((currentTime -lastTimeECE_GhostRelease)/ CLOCKS_PER_SEC)>=GHOSTS_SPAWN_SLOT)
		{
			if(!g->isRunning())
				lastTimeECE_GhostRelease = clock();
			g->setRunning(true);
			g->setAlive(true);
		}

		if(g->isRunning())
		{
			g->setDelta(g->getDelta() + 1);
			if(g->getDelta() >= SCALE)
				g->setDelta(0);
			if(g->getDelta() == 0)
			{
				int x = g->getPosX();
				int y = g->getPosY();
				int newDir;
				int nMaxCols = myECE_Model->getECE_World()->getMaxCols();
				int nMaxRows = myECE_Model->getECE_World()->getMaxRows();
				ECE_Cell*** currentMap = myECE_Model->getECE_World()->getMap();
				ECE_Cell* source = currentMap[y][x];
				ECE_Cell* target = currentMap[nPacY][nPacX];

				ECE_Cell* nextECE_Cell = BFS(currentMap, x, y, nPacX, nPacY, nMaxCols, nMaxRows);
				if (nextECE_Cell != nullptr)
				{
					int nextX = nextECE_Cell->getPosX();
					int nextY = nextECE_Cell->getPosY();
					if (!p->getMode())
					{
						if (nextX > x&& y == nextY) newDir = RIGHT;
						else if (nextX < x && y == nextY) newDir = LEFT;
						else if (nextX == x && nextY < y) newDir = UP;
						else if (nextX == x && nextY > y) newDir = DOWN;
						else newDir = LEFT;
					}
					else//GHOST escaping using a random direction other than the direction towards the super mode player
					{
						int* pnMoves = new int[4];
						int nCount = 0;
						if (x + 1 != nextX && x + 1 < nMaxCols && currentMap[y][x + 1]->getType() > 10)
						{
							pnMoves[nCount++] = RIGHT;
						}
						if (x - 1 != nextX && x - 1 > 0 && currentMap[y][x - 1]->getType() > 10)
						{
							pnMoves[nCount++] = LEFT;
						}
						if (y + 1 != nextY && x + 1 < nMaxRows && currentMap[y + 1][x]->getType() > 10)
						{
							pnMoves[nCount++] = DOWN;
						}
						if (y - 1 != nextX && y - 1 > 0 && currentMap[y - 1][x]->getType() > 10)
						{
							pnMoves[nCount++] = UP;
						}
						if (nCount != 0)
						{
							newDir = pnMoves[(int)clock() % nCount]; //fake random
						}
						else
						{
							newDir = LEFT;
						}
					}
				}
				else
				{
					newDir = LEFT;
				}


				g->setDir(newDir);
				switch(g->getDir())
				{
				case UP:
					g->setPosY(y-1);
					break;
				case RIGHT:
					g->setPosX(x+1);
					break;
				case DOWN:
					g->setPosY(y+1);
					break;
				case LEFT:
					g->setPosX(x-1);
					break;
				}
			}
			this->ghostCollisions(i, g);
		}
	}
}

//This is the handle for the pacman to teleport from one boarder without wall to its corresponding one
void ECE_Controller::handleTeleport(int iX, int iY, int maxRow, int maxCol, int* direction)
{
	bTeleport = false;

	if(!bTeleport && (iX == 0 || iY == 0)) 
	{
		myECE_Model->getECE_PacMan()->setRunning(false);
		*direction=RIGHT;
	}
	if(!bTeleport && (iX == maxCol-1 || iY == maxRow-1)) 
	{
		myECE_Model->getECE_PacMan()->setRunning(false);
		*direction=LEFT; 
	}

	if(bTeleport && (iX == 1 || iY == 1)) 
	{
		bTeleport = false;
	}
	if(bTeleport && (iX == maxCol-2 || iY == maxRow-2)) 
	{
		bTeleport = false;
	}
}

void ECE_Controller::changeDirECE_PacMan(int nextDir, bool changeDir)
{
	int iX = myECE_Model->getECE_PacMan()->getPosX();
	int iY = myECE_Model->getECE_PacMan()->getPosY();
	ECE_Cell*** cMap = myECE_Model->getECE_World()->getMap();

	switch(nextDir) 
	{
		case UP: 
			if((cMap[iY-1][iX]->getType()>10))
				changeDir = true;
			break;
		case RIGHT:
			if((cMap[iY][iX+1]->getType() >10))
				changeDir = true;
			break;
		case LEFT:
			if((cMap[iY][iX-1]->getType() > 10))
				changeDir = true;
			break;
		case DOWN:
			if((cMap[iY+1][iX]->getType() > 10))
				changeDir = true;
			break;
	}

	if(changeDir) 
	{
		myECE_Model->getECE_PacMan()->setDir(nextDir);
		myECE_Model->getECE_PacMan()->changeDir(-1);
	}
}

void ECE_Controller::changePacmanDir()
{
	float step = 1; 
	int iX = myECE_Model->getECE_PacMan()->getPosX();
	int iY = myECE_Model->getECE_PacMan()->getPosY();

	int direction = myECE_Model->getECE_PacMan()->getDir();
	int nextDir = myECE_Model->getECE_PacMan()->getNextDir();

	int maxRow = myECE_Model->getECE_World()->getMaxRows();
	int maxCol = myECE_Model->getECE_World()->getMaxCols();

	this->handleTeleport(iX, iY, maxRow, maxCol, &direction);

	ECE_Cell*** cMap = myECE_Model->getECE_World()->getMap();

	if(myECE_Model->getECE_PacMan()->getDelta() == 0)
	{
		switch(direction)
		{
		case UP:
			if((cMap[iY-1][iX]->getType() >10))
			{
				if((iY-1) == 0) { 
					if((cMap[maxRow-1][iX]->getType() > 10))
					{
						myECE_Model->getECE_PacMan()->setPosY(maxRow-1);
						bTeleport = true;
					}
				}
				myECE_Model->getECE_PacMan()->setPosY(myECE_Model->getECE_PacMan()->getPosY() - step);
			} else
				myECE_Model->getECE_PacMan()->setRunning(false);
			break;
		case RIGHT:
			if((cMap[iY][iX+1]->getType() > 10))
			{
				if((iX+2) == maxCol)
				{ 
					if((cMap[iY][0]->getType() > 10))
					{
						myECE_Model->getECE_PacMan()->setPosX(0);
						bTeleport = true;
					}
				}
				myECE_Model->getECE_PacMan()->setPosX(myECE_Model->getECE_PacMan()->getPosX() + step);
			}
			else 
			{
				myECE_Model->getECE_PacMan()->setRunning(false);
			}
			break;
		case DOWN:
			if((cMap[iY+1][iX]->getType() > 10))
			{
				if((iY+2) == maxRow)
				{
					if((cMap[0][iX]->getType() > 10))
					{
						myECE_Model->getECE_PacMan()->setPosY(0);
						bTeleport = true;
					}
				}
				myECE_Model->getECE_PacMan()->setPosY(myECE_Model->getECE_PacMan()->getPosY() + step);
			}
			else 
			{
				myECE_Model->getECE_PacMan()->setRunning(false);
			}
			break;
		case LEFT:
			if((cMap[iY][iX-1]->getType() > 10))
			{
				if((iX-1) == 0)
				{
					if((cMap[iY][maxCol-1]->getType() > 10))
					{
						myECE_Model->getECE_PacMan()->setPosX(maxCol-1);
						bTeleport = true;
					}
				}

				myECE_Model->getECE_PacMan()->setPosX(myECE_Model->getECE_PacMan()->getPosX() - step);
			}
			else
			{
				myECE_Model->getECE_PacMan()->setRunning(false);
			}
			break;
		}
	}
	
	bool changeDir = false;
	if(myECE_Model->getECE_PacMan()->getDelta() == (SCALE - 1) && nextDir != -1 && nextDir != direction) 
	{
		this->changeDirECE_PacMan(nextDir, changeDir);
	}
}

void ECE_Controller::powerUp()
{
	if(myECE_Model->getECE_PacMan()->getMode() == SUPER)
	{
		myECE_Model->getECE_PacMan()->changeColor();
		if(((clock()-timeModeSuper) / CLOCKS_PER_SEC) > SUPER_TIME)
			myECE_Model->getECE_PacMan()->changeMode(NORMAL);
	}
}

void ECE_Controller::pickCoins() 
{
	int nextX = myECE_Model->getECE_PacMan()->getPosX();
	int nextY = myECE_Model->getECE_PacMan()->getPosY();
	ECE_Cell*** cMap = myECE_Model->getECE_World()->getMap();

	if(myECE_Model->getECE_PacMan()->isRunning() && (cMap[nextY][nextX]->getType() == COIN || cMap[nextY][nextX]->getType() == POWER_UP))
	{
		if(cMap[nextY][nextX]->getType() == POWER_UP)
		{
			int nNumPowerUps = myECE_Model->getECE_World()->getNumPowerUps();
			myECE_Model->getECE_World()->setNumPowerUps(nNumPowerUps - 1);
			timeModeSuper = clock();
			myECE_Model->getECE_PacMan()->changeMode(SUPER);
		}
		else 
		{
			int nNumCoins = myECE_Model->getECE_World()->getNumCoins();
			myECE_Model->getECE_World()->setNumCoins(nNumCoins - 1);
		}
		cMap[nextY][nextX]->setType(EMPTY);
	}
	int nNumCoins = myECE_Model->getECE_World()->getNumCoins();
	int nNumPowerUps = myECE_Model->getECE_World()->getNumPowerUps();
	if(nNumCoins == 0 && nNumPowerUps == 0) 
	{
		myECE_Model->getECE_PacMan()->setRunning(false);
		myECE_Model->stopGame();
		myECE_Model->gameWon();
	}
}

void ECE_Controller::smoothPacman()
{
	if(myECE_Model->getECE_PacMan()->isRunning()) 
	{
		myECE_Model->getECE_PacMan()->setDelta(myECE_Model->getECE_PacMan()->getDelta() + 1);
		if(myECE_Model->getECE_PacMan()->getDelta() >= SCALE)
			myECE_Model->getECE_PacMan()->setDelta(0);
	} 
	else 
	{
		myECE_Model->getECE_PacMan()->setDelta(0);
	}
}

void ECE_Controller::display() 
{
	if(myECE_Model->isStarted())
	{
		moveECE_Ghosts();
	}
	powerUp();

	if(myECE_Model->getECE_PacMan()->isRunning() && myECE_Model->isStarted()) 
	{
		bool changeDir = false;
		this->changePacmanDir();
		this->pickCoins();
		this->smoothPacman();
	}
	ECE_Display::getInstance()->display(); 
	glutTimerFunc(TIMESTEP,animateECE_DisplayCallBack,false);
}

void ECE_Controller::handleNewDir(int newDir) {
	if(newDir != -1)
	{
		if(myECE_Model->getLifes() >= 0) {
			myECE_Model->startGame();	
		}

		if(!myECE_Model->getECE_PacMan()->isRunning()) 
		{
			myECE_Model->getECE_PacMan()->setRunning(true);
			myECE_Model->getECE_PacMan()->setDir(newDir);
		}
		myECE_Model->getECE_PacMan()->changeDir(newDir);
	}
}

void ECE_Controller::keyBoardInput(int key, int x, int y)
{
	int newDir = -1;

	switch(key) 
	{
	case GLUT_KEY_DOWN:
		newDir = DOWN;
		break;
	case GLUT_KEY_UP: 
		newDir = UP;
		break;
	case GLUT_KEY_LEFT:
		newDir = LEFT;
		break;
	case GLUT_KEY_RIGHT:
		newDir = RIGHT;
		break;
	case GLUT_KEY_F1:
		myECE_Model->incCamPosX(SCALE);
		break;
	case GLUT_KEY_F2:
		myECE_Model->incCamPosX(-SCALE);
		break;
	case GLUT_KEY_F3:
		myECE_Model->zoomInOut(ZOOM_DEFAULT);
		break;
	case GLUT_KEY_F4:
		myECE_Model->zoomInOut(-ZOOM_DEFAULT);
		break;
	case GLUT_KEY_HOME:
		myECE_Model->incCamPosY(SCALE);
		break;
	case GLUT_KEY_END:
		myECE_Model->incCamPosY(-SCALE);
		break;
	case GLUT_KEY_PAGE_UP:
		myECE_Model->incCamPosZ(SCALE);
		break;
	case GLUT_KEY_PAGE_DOWN:
		myECE_Model->incCamPosZ(-SCALE);
		break;
	case GLUT_KEY_INSERT:
		myECE_Model->reinitZoom();
		myECE_Model->reinitPosCam();
		break;
	case GLUT_KEY_F5:
		myECE_Model->getECE_PacMan()->setRunning(false);
		myECE_Model->reset();
	}
	this->handleNewDir(newDir);
}
