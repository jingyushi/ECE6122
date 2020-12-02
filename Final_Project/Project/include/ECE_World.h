/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of World class
*/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "ECE_Globals.h"
#include "ECE_Cell.h"
#include "GL/glut.h"
#include "GL/gl.h"

using namespace std;

class ECE_World 
{	
private:
	static ECE_World* instance;
	static int nNumCoins; //the number of coins
	static int nNumPowerUps; //the number of powerups
	static int nNumECE_Ghosts; //the number of ghosts
	static int nMaxRows; //maximum number of rows of the world
	static string sFile; //string containg the path to the .map file
	static int nMaxCols;  //maximum number of cols of the world
	ECE_Cell*** cMap; //pointer to each cell in the world

public:
	ECE_World();
	ECE_World(string);
	~ECE_World();

	ECE_Cell*** getMap();
	int getNumCoins();
	int getNumPowerUps();
	int getMaxRows();
	int getMaxCols();
	int getNumECE_Ghosts();
	void destroyMap(void);
	void setMaxRows(int);
	void setMaxCols(int);
	void setNumCoins(int);
	void setNumPowerUps(int);
	static ECE_World *getInstance();
	void initMap(std::ifstream&,ECE_Cell***&);
};
