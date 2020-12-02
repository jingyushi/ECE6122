/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Model class
*/
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ECE_PacMan.h"
#include "ECE_Ghost.h"
#include "ECE_World.h"

using namespace std;

//In a model, the instance of the PacMan, the world and the four ghosts are declared
class ECE_Model
{
private:
	static ECE_Model *instance; 
	ECE_PacMan* pacman;
	ECE_World* world;
	ECE_Ghost** ghosts;
	bool bIsStarted; //whether the game has begun or not
	bool bIsWon; //whether the game has been won or not
	int nLifes;	// the number of lifes the player has
	int posCamX; //the x coordinate of the camera
	int posCamY; //the y coordinate of the camera		
	int posCamZ; //the z coordinate of the camera		
	int zoom; // the zooming scale of the model
public:
	bool hasBegun(void);
	ECE_World* getECE_World();
	ECE_Model();
	~ECE_Model();
	static ECE_Model *getInstance();
	ECE_PacMan* getECE_PacMan();
	int getLifes();
	ECE_Ghost* getECE_Ghost(int i);
	int getCamPosX();
	int getCamPosY();
	int getCamPosZ();
	int getZoom();
	bool isStarted();
	bool isWon();
	void reset(void);
	void initECE_Ghosts();
	void initECE_PacMan();
	void startGame();
	void stopGame();
	void gameWon();
	void pacManDie();
	void incCamPosX(int);
	void incCamPosY(int);
	void incCamPosZ(int);
	void zoomInOut(int);
	void reinitPosCam(void);
	void reinitZoom(void);
	void reinitAttributes(char*);
};