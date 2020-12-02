/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Definition of coin class
*/

#pragma once

class ECE_Coin
{
public:
	float x;
	float y;
	bool bIsPowerUp;
	ECE_Coin();
	~ECE_Coin();
	float getX();
	float getY();
	bool isPowerUp();
	void setX(float value);
	void setY(float value);
	void setType(bool value);
};