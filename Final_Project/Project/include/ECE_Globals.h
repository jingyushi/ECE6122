/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
All global variables are defined here
*/
#pragma once

//TIME
#define SUPER_TIME 5
#define GHOSTS_FREEZE_TIME 5
#define GHOSTS_SPAWN_SLOT 2
#define TIMESTEP 80 

//SCALE
#define SCALE 5 
#define COIN_RADIUS 1  
#define PACMAN_RADIUS 2.25	
#define GHOST_RADIUS 2.25	
#define POWERUP_HEIGHT 1.5
#define POWERUP_RADIUS 1.5
#define WALL_DEPTH 0.5f

//DEFINITION
#define WALL1 0
#define WALL2 1
#define WALL3 2
#define WALL4 3
#define WALL5 4
#define WALL6 5
#define WALL7 6
#define WALL8 7
#define WALL9 8
#define WALL10 9
#define WALL11 10
#define COIN 11
#define POWER_UP 12
#define EMPTY 13
#define GHOST 14
#define PACMAN 18
#define NORMAL 0
#define SUPER 1 

//View
#define ZOOM_DEFAULT 2
#define X_VIEWPORT 0.25
#define Y_VIEWPORT 0.0666
#define X_VIEWPORT_CONTROL 0.2
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3