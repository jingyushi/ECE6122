/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Ghost class
*/
#include "ECE_Ghost.h"

ECE_Ghost::ECE_Ghost()
{
	posX = 0;
	posY = 0;
	dir = LEFT;
	color = new float[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	bIsAlive = true;
	delta = SCALE;
	radius = GHOST_RADIUS;
}

ECE_Ghost::~ECE_Ghost()
{
	delete[] color;
}

bool ECE_Ghost::isAlive()
{
	return this->bIsAlive;
}

void ECE_Ghost::setAlive(bool isAlive)
{
	this->bIsAlive = isAlive;
}