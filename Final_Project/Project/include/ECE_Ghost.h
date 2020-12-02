/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Nov.22, 2020
Description:
Definition of Ghost class
*/
#pragma once
#include "ECE_Mobile.h"

class ECE_Ghost : public ECE_Mobile
{
private:
	bool bIsAlive; //whether this Ghost is alive. If it is dead it has to stay in prison
public:
	ECE_Ghost();
	~ECE_Ghost();
	bool isAlive();
	void setAlive(bool isAlive);
};