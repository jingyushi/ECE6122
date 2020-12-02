/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of PacMan class
*/
#include "ECE_PacMan.h"

ECE_PacMan::ECE_PacMan()
{
	color = new float[3];
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 0.0;
	mode = NORMAL;
	dir = LEFT;
	nextDir = -1;
	radius = PACMAN_RADIUS;
	posX = 5;
	posY = 3;
	delta = 0;
}

ECE_PacMan::~ECE_PacMan() 
{
	delete[] color;
}


int ECE_PacMan::getMode()
{
	return mode;
}

int ECE_PacMan::getNextDir()
{
	return nextDir;
}

void ECE_PacMan::changeDir(int newNextDir) 
{
	nextDir = newNextDir;
}

void ECE_PacMan::changeMode(bool value)
{
	mode = value;
	if (!value && color[1] != 1.0)
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2]  = 0.0;
	}
}

void ECE_PacMan::changeColor() 
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