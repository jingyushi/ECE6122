/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Display class
*/
#pragma once
#include "ECE_Cell.h"
#include "ECE_Model.h"
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

class ECE_Display
{
protected:
	static ECE_Display *instance;
	ECE_Model *myECE_Model;
	int h_global, w_global;	//width and height of the window
	GLdouble d1_real,d2_real; //viewport control parameters
	GLdouble delta1_reel,delta2_reel; //the deltas of d1_reel and d2_real
	GLdouble d1ECE_World, d2ECE_World; //viewport control parameters
	GLdouble delta1ECE_World, delta2ECE_World; //the deltas of d1ECE_World and d2ECE_World
	int idList_ECE_PacMan, idList_Mouse_Controls,
		idList_Coin, idList_SuperCoin,
		idList_ECE_PacMan_2D, idList_ECE_Ghost; //indices for lists containing different objects

	void viewportInfos();
	void drawECE_World();
	void drawECE_Cell(ECE_Cell cell, int i, int j,GLUquadricObj* quadratic);
	virtual void drawECE_PacMan();
	virtual void drawECE_Ghosts();
	virtual void drawCoins();
	void drawLifes();
	void viewportControls(); 
	void initECE_PacMan();
	void initCoins();
	void initSuperCoins();
	void initDisplayLists(void);

public:
	void display(void);			
	void reshape(int w, int h);				
	void gameOver(void);			
	void setInstance(ECE_Display*);		
	virtual void viewportECE_World();			
	virtual void initLights(void);			

	ECE_Display();
	virtual ~ECE_Display();
	static ECE_Display *getInstance();

};
