/******************************************************************************/
/*!
\file	Inhand.h
\author Group 7
\brief
Header file for Inhand
*/
/******************************************************************************/
#ifndef INHAND_H
#define INHAND_H

#include <vector>
using std::vector;

class Inhand
{
public:
	vector<int> holding;
	int maxAmount;
	bool reachMax;

	Inhand();
	Inhand(int maxAmount);
	~Inhand(void);

	void recive(int a);
	int remove(int currentlyHolding);
	void dropAll();
};
#endif