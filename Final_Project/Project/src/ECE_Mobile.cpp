/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Mobile class
*/
#include "ECE_Mobile.h"

ECE_Mobile::ECE_Mobile()
{
	bIsRunning = false;
	posX = 0;
	posY = 0;
	initPosX = -1;
	initPosY = -1;
	dir = LEFT;
	color = new float[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	delta = SCALE;
	radius = PACMAN_RADIUS;
}

bool ECE_Mobile::isRunning() {
	return bIsRunning;
}

int ECE_Mobile::getPosX()
{
	return posX;
}

int ECE_Mobile::getPosY()
{
	return posY;
}

float ECE_Mobile::getRadius()
{
	return radius;
}

int ECE_Mobile::getDir()
{
	return dir;
}

float* ECE_Mobile::getColor()
{
	return color;
}

int ECE_Mobile::getDelta()
{
	return delta;
}

int ECE_Mobile::getInitPosX()
{
	return initPosX;
}

int ECE_Mobile::getInitPosY()
{
	return initPosY;
}

void ECE_Mobile::setInitPosX(int x)
{
	initPosX = x;
}

void ECE_Mobile::setInitPosY(int y)
{
	initPosY = y;
}

void ECE_Mobile::setRunning(bool r) {
	bIsRunning = r;
}

void ECE_Mobile::setPosX(int value)
{
	posX = value;
}

void ECE_Mobile::setPosY(int value)
{
	posY = value;
}

void ECE_Mobile::setRadius(float value)
{
	radius = value;
}

void ECE_Mobile::setDir(int value)
{
	dir = value;
}

void ECE_Mobile::setColor(float* value)
{
	color = value;
}

void ECE_Mobile::setDelta(int value)
{
	delta = value;
}