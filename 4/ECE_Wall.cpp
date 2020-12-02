/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Implementation of wall class
*/
#include "ECE_Wall.h"

ECE_Wall::ECE_Wall()
{
	x = 0.0;
	y = 0.0;
	type = 0;
}

ECE_Wall::~ECE_Wall()
{

}

float ECE_Wall::getX()
{
	return this->x;
}
float ECE_Wall::getY()
{
	return this->y;
}
int ECE_Wall::getType()
{
	return this->type;
}
void ECE_Wall::setX(float value)
{
	this->x = value;
}
void ECE_Wall::setY(float value)
{
	this->y = value;
}
void ECE_Wall::setType(int value)
{
	this->type = value;
}