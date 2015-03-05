/******************************************************************************/
/*!
\file	AI.h
\author Group 7
\brief
Header file for AI
*/
/******************************************************************************/
#include <vector>
#include "Vector3.h"
#include "AIDefine.h"
#include <iostream>
#pragma once

using std::vector;

class AI
{
public:
	AI(Vector3 spawnCoord, int startState);
	AI(void);
	~AI(void);

	vector<Vector3> Coordinates, Waypoints;
	vector<float> rotateY;
	vector<int> State;
	int AILimit, AICurrent, spawnRate;

	virtual void spawnAI(void) = 0;
	virtual void updateAI(void) = 0;
	virtual void interactWith(void) = 0;
};

