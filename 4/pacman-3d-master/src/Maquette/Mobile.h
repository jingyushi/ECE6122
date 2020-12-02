#pragma once
#include "Globals.h"

class Mobile
{
protected:
	int initPosX;
	int initPosY;
	int posX;
	int posY;
	float radius;
	int dir;
	float* color;
	int delta;
	bool running;
	Mobile();

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