/******************************************************************************/
/*!
\file	Shelfslot.h
\author Group 7
\brief
Header file for Shelfslot
*/
/******************************************************************************/
#ifndef SHELFSLOT_H
#define SHELFSLOT_H

#include "Vector3.h"
class Shelfslot
{
public:
	Vector3 position, boundMax, boundMin;
	int itemid;
	bool isempty;

	Shelfslot(Vector3 position, Vector3 boundMax, Vector3 boundMin, int itemid, bool isempty);
	~Shelfslot(void);
};

#endif