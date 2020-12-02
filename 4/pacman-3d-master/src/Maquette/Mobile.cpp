#include "Mobile.h"

Mobile::Mobile()
{
	running = false;
	posX = 0;
	posY = 0;
	initPosX = -1;
	initPosY = -1;
	dir = PAC_WEST;
	color = new float[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	delta = SCALE;
	radius = PACMAN_RADIUS;
}

bool Mobile::isRunning() {
	return running;
}

int Mobile::getPosX()
{
	return posX;
}

int Mobile::getPosY()
{
	return posY;
}

float Mobile::getRadius()
{
	return radius;
}

int Mobile::getDir()
{
	return dir;
}

float* Mobile::getColor()
{
	return color;
}

int Mobile::getDelta()
{
	return delta;
}

int Mobile::getInitPosX()
{
	return initPosX;
}

int Mobile::getInitPosY()
{
	return initPosY;
}

void Mobile::setInitPosX(int x)
{
	initPosX = x;
}

void Mobile::setInitPosY(int y)
{
	initPosY = y;
}

void Mobile::setRunning(bool r) {
	running = r;
}

void Mobile::setPosX(int value)
{
	posX = value;
}

void Mobile::setPosY(int value)
{
	posY = value;
}

void Mobile::setRadius(float value)
{
	radius = value;
}

void Mobile::setDir(int value)
{
	dir = value;
}

void Mobile::setColor(float* value)
{
	color = value;
}

void Mobile::setDelta(int value)
{
	delta = value;
}