#ifndef PROMOTER_H
#define PROMOTER_H

#include "AI.h"

#pragma once

class Promoter :	public AI
{
public:
	Promoter(void);
	~Promoter(void);

	float itemRotateY;

	void updateAI(void);
	void spawnAI(void);
	void interactWith(void);
};

#endif