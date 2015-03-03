#ifndef PASSERBY_H
#define PASSERBY_H

#include "AI.h"

#pragma once

class Passerby :	public AI
{
public:
	Passerby(void);
	~Passerby(void);

	void spawnAI(void);
	void updateAI(void);
	void interactWith(void);
};

enum PasserbyPath
{
	SpawntoDestination,
};

enum PasserbyWaypoint
{
	Destination,
};

#endif