/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Implementation of World class
*/
#include "ECE_World.h"

using namespace std;

ECE_World *ECE_World::instance = 0;
int ECE_World::nMaxRows = 0;
int ECE_World::nMaxCols = 0;
string ECE_World::sFile = "";
int ECE_World::nNumCoins = 0;
int ECE_World::nNumPowerUps = 0;
int ECE_World::nNumECE_Ghosts = 0;

std::ifstream &operator>>(std::ifstream &fichier, ECE_Cell*** &cMap) 
{
	if(fichier && fichier.is_open()) 
	{
		ECE_World* w = ECE_World::getInstance();
		w->initMap(fichier,cMap);
	}
	return fichier;
}

ECE_World::ECE_World()
{
}

ECE_World::ECE_World(string name) 
{
	sFile = name;
	nMaxRows = 0;
	nMaxCols = 0;
	ifstream fichier(name, ios_base::in);
	nNumCoins = 0;
	nNumPowerUps = 0;
	nNumECE_Ghosts = 0;

	fichier >> cMap;
	if(nMaxRows == 0) {
		exit(EXIT_SUCCESS);
	}
	for (int i = 0; i < nMaxRows; i++)
	{
		for (int j = 0; j < nMaxCols; j++)
		{
			cout << cMap[i][j]->getType() << ' ';
		}
		std::cout << endl;
	}
}

ECE_World::~ECE_World()
{
	this->destroyMap();
}

ECE_Cell*** ECE_World::getMap()
{
	return cMap;
}

int ECE_World::getNumCoins()
{
	return nNumCoins;
}

int ECE_World::getNumPowerUps()
{
	return nNumPowerUps;
}

int ECE_World::getMaxRows()
{
	return nMaxRows;
}

int ECE_World::getMaxCols()
{
	return nMaxCols;
}

int ECE_World::getNumECE_Ghosts()
{
	return nNumECE_Ghosts;
}

ECE_World *ECE_World::getInstance()
{
	if (instance == 0)
		instance = new ECE_World();

	return instance;
}

void ECE_World::setMaxRows(int newMaxRows) {
	nMaxRows = newMaxRows;
}

void ECE_World::setMaxCols(int newMaxCols) {
	nMaxCols = newMaxCols;
}

void ECE_World::setNumCoins(int nvNbPills) {
	nNumCoins = nvNbPills;
}

void ECE_World::setNumPowerUps(int nvNbSuperPills) {
	nNumPowerUps = nvNbSuperPills;
}

void ECE_World::initMap(std::ifstream &ifsFile, ECE_Cell*** &cMap) 
{
	string line;
	int k=0;

	while(getline(ifsFile, line))
	{
		if(nMaxRows == 0)
		{ 
			for(unsigned int i=0;i< line.size();i++)
			{
				nMaxCols++;
			}
		}
		nMaxRows++;
	}
	ifsFile.close();

	cMap=new ECE_Cell**[nMaxRows];
	for(int i=0;i<nMaxRows;i++)
	{
		cMap[i]=new ECE_Cell*[nMaxCols];
		for(int j=0; j<nMaxCols; j++) 
		{
			cMap[i][j] = new ECE_Cell();
		}
	}

	ifsFile.open(sFile, ios_base::in);

	if(ifsFile && ifsFile.is_open())
	{
		while(getline(ifsFile, line))
		{
			for(unsigned int i=0;i< line.size();i++)
			{
				char c = line[i];

				int type = -1;
				
				switch (c)
				{
				case 'P':
					type = 18;
					break;
				case 'E':
					type = 13;
					break;
				case 'C':
					type = 11;
					break;
				case 'G':
					type = 14;
					break;
				case 'S':
					type = 12;
					break;
				default:
					break;
				}

				int x = k * SCALE;
				int y = i * SCALE;

				if(type == -1)
					type = atoi(&c);

				x -= 5 * SCALE;
				y -= 3 * SCALE;
				y = -y;

				if(type == COIN)
					nNumCoins++;
				else if(type == POWER_UP) 
					nNumPowerUps++;
				else if(type == GHOST)
					nNumECE_Ghosts++;

				cMap[k][i] = new ECE_Cell(x,y,type,i,k);
			}
			k++;
		}
	}
}


void ECE_World::destroyMap() {
for (int i = 0; i < nMaxRows; i++) 
	{
		for(int j = 0; j < nMaxCols; j++) 
		{
			delete cMap[i][j];
		}
		delete[] cMap[i];
	}
	delete[] cMap;
}