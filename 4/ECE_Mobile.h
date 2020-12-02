/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Definition of mobile class, parent of Ghost and Pacman
*/
#pragma once

class ECE_Mobile
{
public:
	float x;
	float y;
	int dir;
	int color;

	ECE_Mobile();
	~ECE_Mobile();
	float getX();
	float getY();
	int getDir();
	int getColor();
	void setX(float value);
	void setY(float value);
	void setDir(int value);
	void setColor(int value);
};
		