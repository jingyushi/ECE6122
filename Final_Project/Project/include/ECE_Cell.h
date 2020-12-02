/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Cell class
*/

#pragma once
#include "GL/glut.h"
#include "GL/gl.h"
#include <fstream>
#include <iostream>
#include <string>

class ECE_Cell 
{
private:
	int type; //Type of the cell			
	int x;	//x coordinate in the scaled world		
	int y;  //y coordinate in the scaled world
	int posX; // integer x coordinate (not scaled)
	int posY; // integer y coordinate (not scaled)

public: 
	ECE_Cell();
	~ECE_Cell();
	ECE_Cell(int,int,int,int,int);

	int getType();
	int getX();
	int getY();
	int getPosX();
	int getPosY();
	void setType(int);
};