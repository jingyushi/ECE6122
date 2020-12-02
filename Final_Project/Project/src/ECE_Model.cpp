/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Model class
*/
#include "ECE_Model.h"
#include "ECE_Display.h"

ECE_Model *ECE_Model::instance = 0;

ECE_Model::ECE_Model()
{
	bIsStarted=false;
	bIsWon = false;
	pacman = new ECE_PacMan();
	nLifes = 3;
	
	posCamX = 35;
	posCamY = -50;
	posCamZ = 30;
	zoom = 50;
	int n;
	char buffer[25];
	n=sprintf_s(buffer, "pacman.map");
	world = new ECE_World(buffer);
	ghosts = new ECE_Ghost*[world->getNumECE_Ghosts()];
	initECE_Ghosts();
	initECE_PacMan();
}

ECE_Model::~ECE_Model()
{
	delete ghosts;
	delete pacman;
	delete world;
}

ECE_Model *ECE_Model::getInstance()
{
	if (instance == 0)
		instance = new ECE_Model();

	return instance;
}

bool ECE_Model::hasBegun(void)
{
	return bIsStarted;
}

ECE_World* ECE_Model::getECE_World()
{
	return world;
}

ECE_PacMan* ECE_Model::getECE_PacMan()
{
	return pacman;
}

int ECE_Model::getLifes()
{
	return nLifes;
}

ECE_Ghost* ECE_Model::getECE_Ghost(int i)
{
	return ghosts[i];
}

int ECE_Model::getCamPosX()
{
	return posCamX;
}

int ECE_Model::getCamPosY()
{
	return posCamY;
}

int ECE_Model::getCamPosZ()
{
	return posCamZ;
}

int ECE_Model::getZoom()
{
	return zoom;
}

bool ECE_Model::isStarted()
{
	return bIsStarted;
}

bool ECE_Model::isWon()
{
	return bIsWon;
}

void ECE_Model::startGame(void) 
{
	bIsStarted=true;
}

void ECE_Model::stopGame(void)
{
	bIsStarted=false;
}

void ECE_Model::gameWon(void)
{
	bIsWon = true;
}

void ECE_Model::reinitAttributes(char* filepath) {
	bIsStarted=false;
	pacman = new ECE_PacMan();
	ghosts = new ECE_Ghost*[world->getNumECE_Ghosts()];
	world = new ECE_World(filepath);
	initECE_Ghosts();
	initECE_PacMan();
}

void ECE_Model::reset(void) {
	delete pacman;
	delete world;
	for(int i = 0; i < world->getNumECE_Ghosts(); i++)
		delete ghosts[i];
	delete[] ghosts;

	nLifes = 3;
	
	char buffer[25];
	int n=sprintf_s(buffer, "pacman.map");
	reinitZoom();
	reinitPosCam();
	reinitAttributes(buffer);
}

void ECE_Model::pacManDie()
{
	initECE_PacMan();
	pacman->setDelta(0);
	pacman->setRadius(PACMAN_RADIUS);
	nLifes--;
	pacman->setRunning(false);
}

void ECE_Model::initECE_Ghosts()
{
	int nCount = 0;

	for(int i = 0; i < world->getMaxRows(); i++)
	{
		for(int j = 0; j < world->getMaxCols(); j++)
		{
			if(world->getMap()[i][j]->getType() == GHOST)
			{
				ghosts[nCount] = new ECE_Ghost();
				ghosts[nCount]->setPosX(j);
				ghosts[nCount]->setInitPosX(j);
				ghosts[nCount]->setPosY(i);
				ghosts[nCount]->setInitPosY(i);
				nCount++;
			}
		}
	}
}

void ECE_Model::initECE_PacMan()
{
	bool found = false;

	for(int i = 0; i < world->getMaxRows(); i++)
	{
		for(int j = 0; j < world->getMaxCols(); j++)
		{
			if((world->getMap()[i][j]->getType() & PACMAN) == PACMAN)
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

void ECE_Model::incCamPosX(int step) {
	posCamX += step;
}

void ECE_Model::incCamPosY(int step) {
	posCamY += step;
}

void ECE_Model::incCamPosZ(int step) {
	posCamZ += step;
}

void ECE_Model::zoomInOut(int step) {
	zoom += step;
}

void ECE_Model::reinitPosCam(void) {
	posCamX = 35;
	posCamY = -50;
	posCamZ = 30;
}

void ECE_Model::reinitZoom(void) {
	zoom = 50;
}