#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "Globals.h"
#include "Cell.h"
#include "GL/glut.h"
#include "GL/gl.h"

using namespace std;

class World 
{	
private:
	static World* instance;
	static string nomFichier;
	static int nbPills;
	static int nbSuperPills;
	static int nbGhosts;
	static int maxRows;
	static int maxCols;
	Cell*** map;

public:
	World();
	World(string);
	~World();

	Cell*** getMap();
	int getNbPills();
	int getNbSuperPills();
	int getMaxRows();
	int getMaxCols();
	int getNbGhosts();

	static World *getInstance();
	void initMap(std::ifstream&,Cell***&);
	void destroyMap(void);
	void setMaxRows(int);
	void setMaxCols(int);
	void setNbPills(int);
	void setNbSuperPills(int);
};
