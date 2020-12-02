/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Mobile class, this is the parent of ECE_Ghost and ECE_PacMan
*/
#pragma once
#include "ECE_Globals.h"

class ECE_Mobile
{
protected:
	int initPosX; //The initial x position of this mobile
	int initPosY; //The initial y position of this mobile
	int posX; // integer x coordinate (not scaled)
	int posY; // integer y coordinate (not scaled)
	float radius; // the radius of this mobile
	int dir; //the direction this mobile is moving towards
	float* color; //the color of this mobile
	int delta; //delta used for motion smoothing
	bool bIsRunning; //whether this mobile is runnning
	ECE_Mobile();

public:
	bool isRunning();
	int getPosX();
	int getPosY();
	float getRadius();
	int getDir();
	float* getColor();
	int getDelta();
	int getInitPosX();
	int getInitPosY();
	void setInitPosX(int);
	void setInitPosY(int);
	void setRunning(bool);
	void setPosX(int);
	void setPosY(int);
	void setRadius(float);
	void setDir(int);
	void setColor(float*);
	void setDelta(int);
};