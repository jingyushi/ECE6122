#pragma once
#include "Globals.h"
#include <string>
#include <fstream>
#include <iostream>
#include "GL/glut.h"
#include "GL/gl.h"


class Cell 
{
private:
	int type;			
	int x;			
	int y;
	float* color;

public: 
	Cell();
	~Cell();
	Cell(int,int,int);

	int getType();
	float* getColor();
	int getX();
	int getY();
	void setColor(float *);
	void getxy(int *, int*);
	void setType(int);
};