/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Definition of wall class
*/
#pragma once

class ECE_Wall
{
public:
	float x;
	float y;
	int type;
	ECE_Wall();
	~ECE_Wall();
	float getX();
	float getY();
	int getType();
	void setX(float value);
	void setY(float value);
	void setType(int value);
};