/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.29, 2020
Description:
Implementation of Ghost
*/
#include "ECE_Ghost.h"

ECE_Ghost::ECE_Ghost()
{
	x = 0.0;
	y = 0.0;
	dir = 0;
	color = 0;
}

ECE_Ghost::ECE_Ghost(int colorNum)
{
	x = 0.0;
	y = 0.0;
	dir = 0;
	color = colorNum;
}
ECE_Ghost::~ECE_Ghost()
{

}