/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of PacMan class
*/
#pragma once
#include <iostream>
#include "ECE_Mobile.h"

using namespace std;

class ECE_PacMan : public ECE_Mobile
{
private:
	int mode; //the mode of the pacman,0 for normal, 1 for super
	int nextDir; //the direction the pacman is moving towards at next time step

public:
	ECE_PacMan();
	~ECE_PacMan();

	int getMode();
	int getNextDir();
	void changeDir(int);
	void changeColor();
	void changeMode(bool);
};

