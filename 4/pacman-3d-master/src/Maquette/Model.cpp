#include "Model.h"
#include "View.h"

Model *Model::instance = 0;

Model::Model()
{
	spinningMode = false;
	//pacmanViewMode = false;
	started=false;
	won = false;
	pacman = new PacMan();
	level = 1;
	lifes = 3;
	scoreBearing = 0;
	
	posCamX = -50;
	posCamY = -50;
	posCamZ = 100;
	
	projectionMode = PARALLEL_PROJECTION;
	zoom = 50;
	int n;
	char buffer[25];
	n=sprintf_s(buffer, "world1.map");
	world = new World(buffer);
	ghosts = new Ghost*[world->getNbGhosts()];
	initGhosts();
	initPacMan();
}

Model::~Model()
{
	delete pacman;
	delete world;
}

Model *Model::getInstance()
{
	if (instance == 0)
		instance = new Model();

	return instance;
}

bool Model::hasBegun(void)
{
	return started;
}

World* Model::getWorld()
{
	return world;
}

PacMan* Model::getPacMan()
{
	return pacman;
}

int Model::getLifes()
{
	return lifes;
}

int Model::getLevel()
{
	return level;
}

Ghost* Model::getGhost(int i)
{
	return ghosts[i];
}

int Model::getCamPosX()
{
	return posCamX;
}

int Model::getCamPosY()
{
	return posCamY;
}

int Model::getCamPosZ()
{
	return posCamZ;
}

int Model::getZoom()
{
	return zoom;
}

int Model::getProjectionMode() 
{
	return projectionMode;
}

bool Model::isStarted()
{
	return started;
}

bool Model::isWon()
{
	return won;
}

//bool Model::isInPacManViewMode()
//{
//	return pacmanViewMode;
//}

bool Model::isSpinning()
{
	return spinningMode;
}

void Model::setProjectionMode(int newProjectionMode) {
	projectionMode = newProjectionMode;
}

void Model::incLifes(int step) 
{
	lifes += step;
	if(lifes < 0) {
		stopGame();
	}
}

void Model::startGame(void) 
{
	started=true;
}

void Model::stopGame(void)
{
	started=false;
}

void Model::gameWon(void)
{
	won = true;
}

void Model::nextLevel(void)
{
	level++;
	char buffer[25];
	int n=sprintf_s(buffer, "world%d.map", level);
	reinitAttributes(buffer);
}

void Model::reinitAttributes(char* buffer) {
	spinningMode = false;
	//pacmanViewMode = false;
	started=false;
	pacman = new PacMan();
	ghosts = new Ghost*[world->getNbGhosts()];
	world = new World(buffer);
	initGhosts();
	initPacMan();
}

void Model::reset(void) {
	delete pacman;
	delete world;
	for(int i = 0; i < world->getNbGhosts(); i++)
		delete ghosts[i];
	delete[] ghosts;

	level = 1;
	lifes = 1;
	
	char buffer[25];
	int n=sprintf_s(buffer, "world%d.map", level);
	View::getInstance()->resetSpin();
	reinitZoom();
	reinitPosCam();
	reinitAttributes(buffer);
}

void Model::pacManDie()
{
	initPacMan();
	pacman->setDelta(0);
	pacman->setRadius(PACMAN_RADIUS);
	pacman->setDying(0);
	lifes--;
	pacman->setRunning(false);
}

void Model::initGhosts()
{
	int cmpt = 0;

	for(int i = 0; i < world->getMaxRows(); i++)
	{
		for(int j = 0; j < world->getMaxCols(); j++)
		{
			if(world->getMap()[i][j]->getType() == GHOST)
			{
				ghosts[cmpt] = new Ghost();
				ghosts[cmpt]->setPosX(j);
				ghosts[cmpt]->setInitPosX(j);
				ghosts[cmpt]->setPosY(i);
				ghosts[cmpt]->setInitPosY(i);
				cmpt++;
			}
		}
	}
}

void Model::initPacMan()
{
	int cmpt = 0;
	bool found = false;

	for(int i = 0; i < world->getMaxRows(); i++)
	{
		for(int j = 0; j < world->getMaxCols(); j++)
		{
			if((world->getMap()[i][j]->getType() & PACMAN_POS) == PACMAN_POS)
			{
				pacman->setPosX(j);
				pacman->setPosY(i);
				found = true;
				break;
			}
		}
		if(found) break;
	}
}

void Model::incCamPosX(int step) {
	posCamX += step;
}

void Model::incCamPosY(int step) {
	posCamY += step;
}

void Model::incCamPosZ(int step) {
	posCamZ += step;
}

void Model::zoomInOut(int step) {
	zoom += step;
}

void Model::reinitPosCam(void) {
	posCamX = -50;
	posCamY = -50;
	posCamZ = 100;
}

void Model::reinitZoom(void) {
	zoom = 50;
}

void Model::toggleSpinning(void) {
	spinningMode = !spinningMode;
}

//void Model::setPacManViewMode(bool newViewMode) {
//	pacmanViewMode = newViewMode;
//}