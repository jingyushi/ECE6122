#pragma once
#include "Model.h"
#include "View.h"
#include <time.h>
#include <exception>
#include <iostream>
#include "windows.h"

using namespace std;

#define TEMPSBASE 80 

class Controller
{
private:
	static Controller *instance;
	Model *model;					
	View  *view;					
	Controller();
	~Controller();
	int stepOK;
	clock_t timeModeSuper;			
	clock_t *timeGhostInPrison;		
	bool stargate;
	
	void animateGhosts();
	void handleCollisions(int, Ghost*);
	void switchSuperMode();								
	void changeDirGhost(Ghost*, int, int, int*);
	void handleChangeDirPacMan();				
	void changeDirPacMan(int, bool);			
	void handlePills();							
	void handlePacManSmooth();					
/*	void handleSwitchDimension();*/				
	void handleNewDir(int);						
	void handleStargate(int,int,int,int,int*);	

public:
	void special(int key, int x, int y);				
	void animateDisplay();								
	static Controller *getInstance();					
/*	void togglePacManViewMode();*/						
	//void mouseRight(int button, int state, float x, float y, int* viewport);
	//void mouseMiddle(float x, float y, int* viewport);
	//void handlePicking(int button, int state, int x, int y);
};

