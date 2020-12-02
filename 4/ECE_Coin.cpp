/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Implementation of coin class
*/
#include "ECE_Coin.h"

ECE_Coin::ECE_Coin()
{
	x = 0.0;
	y = 0.0;
	bIsPowerUp = false;
}

ECE_Coin::~ECE_Coin()
{

}

float ECE_Coin::getX()
{
	return this->x;
}
float ECE_Coin::getY()
{
	return this->y;
}
bool ECE_Coin::isPowerUp()
{
	return this->bIsPowerUp;
}
void ECE_Coin::setX(float value)
{
	this->x = value;
}
void ECE_Coin::setY(float value)
{
	this->y = value;
}
void ECE_Coin::setType(bool value)
{
	this->bIsPowerUp = value;
}