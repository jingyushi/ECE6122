#pragma once
#include <iostream>
#include "Globals.h"
#include "Mobile.h"

using namespace std;

class PacMan : public Mobile
{
private:
	int mode;
	int nextDir;
	bool mouthOpen;
	int dying;

public:
	PacMan();
	~PacMan();

	int getMode();
	int getNextDir();
	bool isMouthOpen();
	int getDying();
	void setNextDir(int);
	void switchMouth();
	void couleurAleatoire();
	void animSuperMode(bool);
	void setDying(int);
};

