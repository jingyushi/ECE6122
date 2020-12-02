#pragma once
#include "Cell.h"
#include "Model.h"
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

class View
{
protected:
	static View *instance;		// Instance de la vue pour le pattern singleton
	double time;
	Model *model;				// Instance du Model pour accéder aux données modifiables
	int h_global, w_global;		// Pour stocker hauteur et largeur de la fenêtre d'écran
	GLdouble d1_reel,d2_reel;
	GLdouble delta1_reel,delta2_reel;
	GLdouble d1World, d2World;
	GLdouble delta1World, delta2World;
	int idList_PacMan, idList_Mouse_Controls,
		idList_Pill, idList_SuperPill,
		idList_PacMan_2D, idList_Ghost;

	void viewportInfos();			// Définit une viewport pour les informations concernant la partie en cours
	void drawWorld();		// Dessine le labyrinthe
	void drawCell(Cell cell, int i, int j);
	virtual void drawPacMan();		// Définit un carré ayant subi une certaine rotation autour de son centre
	virtual void drawGhosts();		// Dessine les fantômes
	virtual void drawHLine();
	virtual void drawWLine();
	virtual void drawPills();	// Dessine les pastilles
	void drawLifes();

	void viewportControls(); // Définit une viewport pour les contrôles souris

	void initDlPacMan();
	void initDlPills();
	void initDlSuperPills();
	void initDisplayLists(void);

public:
	void display(void);				// Définit les formes dans l'univers de dessin
	void reshape(int w, int h);		// Appelé en cas de redimensionnement de la fenêtre d'écran
	void redraw(void);				// Rafraichit l'affichage
	void gameOver(void);			// Affiche un GAME OVER
	void setInstance(View*);		// Pattern stratégie pour changer entre View3D et 2D
	virtual void viewportWorld();			// Définit une viewport pour le monde
	//virtual void drawCentralControl(GLenum);		// Dessine un point central permettant au joueur de cliquer pour commencer la rotation du plateau de jeu
	void setHGlobal(int);			// Positionne la hauteur globale
	void setWGlobal(int);			// Positionne la largeur globale
	virtual void initLights(void);			// Initialisation de l'éclairage

	GLint getHGlobal();

	GLint getWGlobal();

	GLdouble getD1();

	GLdouble getD2();

	GLdouble getDelta1();
	GLdouble getDelta2();
	View();
	virtual ~View();

	// Pattern singleton
	static View *getInstance();
	static float * getNormal(float * p1, float * p2, float * p3);
	void resetSpin();
	void drawTopPacman(double r, int lats, int longs);
	void drawBottomPacman(double r, int lats, int longs);
};
