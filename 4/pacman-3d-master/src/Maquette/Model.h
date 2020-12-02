#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Globals.h"
#include "PacMan.h"
#include "Ghost.h"
#include "World.h"


#define INCRANGLE .5

using namespace std;

class Model
{
private:
	static Model *instance;	// Instance du Model pour le pattern singleton
	bool started;
	bool won;
	int level;				// niveau de jeu
	int lifes;				// nombre de vies du PacMan				
	int scoreBearing;		// score atteint pour le gain de vie
	int projectionMode;		// type de projection utilisé pour afficher la viewportWorld (celle qui contient le labyrinthe)
	bool spinningMode;		// définit si le plateau de jeu est en train de tourner ou pas
	//bool pacmanViewMode;	// définit si l'on place la caméra derrière le PacMan pour jouer
	PacMan *pacman;			// Bonhomme pacman
	World *world;			// carte
	Ghost **ghosts;			// fantômes

	int posCamX;			// position de la caméra sur l'axe des X
	int posCamY;			// position de la caméra sur l'axe des Y
	int posCamZ;			// position de la caméra sur l'axe des Z

	int zoom;				// zoom sur le labyrinthe

public:
	bool hasBegun(void);
	World* getWorld();
	Model();
	~Model();
	static Model *getInstance();
	PacMan* getPacMan();
	int getLifes();
	int getLevel();
	Ghost* getGhost(int i);
	int getCamPosX();
	int getCamPosY();
	int getCamPosZ();
	int getZoom();
	int getProjectionMode();
	bool isStarted();
	bool isWon();
	//bool isInPacManViewMode();
	bool isSpinning();
	void incLifes(int);
	void setProjectionMode(int);
	void nextLevel(void);
	void reset(void);
	void initGhosts();
	void initPacMan();
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
	void toggleSpinning();
	void reinitAttributes(char*);
	//void setPacManViewMode(bool);
};