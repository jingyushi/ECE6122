/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Controller class
*/
#pragma once
#include "ECE_Model.h"
#include "ECE_Display.h"
#include <time.h>
#include <exception>
#include "windows.h"
#include <cmath>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <iostream>


using namespace std;

class ECE_Controller
{
private:
	static ECE_Controller *instance; // the instance of controller
	ECE_Model *myECE_Model;	 //the model declared in this controller
	ECE_Display  *myECE_Display;  //the display declared in this controller
	ECE_Controller();
	~ECE_Controller();
	clock_t timeModeSuper;  //the time for the PacMan to stay in the super mode
	clock_t *timeECE_GhostInPrison;	// the times (4 in total) for the Ghosts to stay in prison
	clock_t lastTimeECE_GhostRelease; // the time from the last time a ghost is released
	bool bTeleport; // whether the pacman can be teleported from one border to its corresponding one
	
	void moveECE_Ghosts();
	void ghostCollisions(int, ECE_Ghost*);
	//Using BFS algorithm for the ghosts to navigate
	ECE_Cell* BFS(ECE_Cell*** currentMap, int sourceX, int sourceY, int targetX, int targetY, int nMaxCols, int nMaxRows);
	//This is the returning function of BFS algorithm
	ECE_Cell* getShortestDir(map<ECE_Cell*, ECE_Cell*> myMap, ECE_Cell* target, int sourceX, int sourceY);
	void powerUp();
	void changePacmanDir();	
	void smoothPacman();
	void handleNewDir(int);
	void handleTeleport(int, int, int, int, int*);
	void changeDirECE_PacMan(int, bool);			
	void pickCoins();							

public:
	void keyBoardInput(int key, int x, int y);				
	void display();								
	static ECE_Controller *getInstance();					
};

