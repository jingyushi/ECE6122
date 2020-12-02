/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Implementation of mobile class, parent of Ghost and Pacman
*/
#include "ECE_Mobile.h"

ECE_Mobile::ECE_Mobile()
{
	x = 0.0;
	y = 0.0;
	dir = 0;
	color = 0;
}

ECE_Mobile::~ECE_Mobile()
{

}

float ECE_Mobile::getX()
{
	return this->x;
}
float ECE_Mobile::getY()
{
	return this->y;
}

int ECE_Mobile::getDir()
{
	return this->dir;
}
int ECE_Mobile::getColor()
{
	return this->color;
}
void ECE_Mobile::setX(float value)
{
	this->x = value;
}
void ECE_Mobile::setY(float value)
{
	this->y = value;
}
void ECE_Mobile::setDir(int value)
{
	this->dir = value;
}
void ECE_Mobile::setColor(int value)
{
	this->color = value;
}