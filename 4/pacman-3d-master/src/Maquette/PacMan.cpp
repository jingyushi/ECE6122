#include "PacMan.h"

PacMan::PacMan()
{
	color = new float[3];
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 0.0;
	mode = NORMAL;
	dir = PAC_WEST;
	nextDir = -1;
	mouthOpen = false;
	radius = PACMAN_RADIUS;
	posX = 5;
	posY = 3;
	delta = 0;
	dying = false;
}

PacMan::~PacMan() 
{
	delete[] color;
}


int PacMan::getMode()
{
	return mode;
}

int PacMan::getNextDir()
{
	return nextDir;
}

bool PacMan::isMouthOpen()
{
	return mouthOpen;
}

int PacMan::getDying()
{
	return dying;
}

void PacMan::setNextDir(int newNextDir) 
{
	nextDir = newNextDir;
}

void PacMan::switchMouth()
{
	mouthOpen = !mouthOpen;
}

void PacMan::animSuperMode(bool value)
{
	mode = value;
	if (!value && color[1] != 1.0)
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2]  = 0.0;
	}
}

void PacMan::couleurAleatoire() 
{
	if(color[1] == 1.0) 
	{
		color[0] = 1.0;
		color[1] = 0.0;
		color[2] = 0.0;
	}
	else 
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 0.0;
	}
}

void PacMan::setDying(int value)
{
	dying = value;
}