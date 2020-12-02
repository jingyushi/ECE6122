/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of Cell class
*/
#include "ECE_Cell.h"

ECE_Cell::ECE_Cell()
{
	ECE_Cell(0, 0, 0,0,0);
}

ECE_Cell::ECE_Cell(int mx,int my,int t,int px, int py) 
{
	x=mx;
	y=my;
	type=t;
	posX = px;
	posY = py;
}

ECE_Cell::~ECE_Cell() {
}

int ECE_Cell::getType()
{
	return type;
}

int ECE_Cell::getPosX()
{
	return posX;
}

int ECE_Cell::getPosY()
{
	return posY;
}
int ECE_Cell::getX()
{
	return x;
}

int ECE_Cell::getY()
{
	return y;
}

void ECE_Cell::setType(int t) 
{
	type=t;
}
