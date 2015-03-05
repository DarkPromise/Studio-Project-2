/******************************************************************************/
/*!
\file	Cashier.h
\author Group 7
\brief
Header file for Cashier
*/
/******************************************************************************/
#ifndef CASHIER_H
#define CASHIER_H

#include "AI.h"

#pragma once

class Cashier	:	public AI
{
public:
	Cashier(void);
	~Cashier(void);

	vector<float> turnDelay;

	void spawnAI(void);
	void updateAI(void);
	void interactWith(void);
};

#endif