/******************************************************************************/
/*!
\file	Item.h
\author Group 7
\brief
Header file for Item
*/
/******************************************************************************/
#ifndef ITEM_H
#define ITEM_H

#include "Vector3.h"

class Item
{
public:
	Vector3 position, boundMax, storeBoundMax, boundMin, storeBoundMin, translate, scale;
	float rotateY, resetRotateyY;
	int mesh, uimesh;
	
	Item(Vector3 boundMax, Vector3 boundMin, Vector3 translate, Vector3 scale, float rotateY, int mesh, int uimesh);
	~Item(void);

	void takeItem(Vector3 newPos);
	void placeItem(Vector3 newPos);
	void updateRotate(float rotate);
	void resetRotate();
};

#endif