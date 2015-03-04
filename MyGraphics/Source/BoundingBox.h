#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//Josh Code
#include "Child.h"
#include "Vector3.h"
#include "GL\glew.h"

class BoundingBox
{
public:
	BoundingBox() //Inside = BoundingBox for indoors
		: isInside(true),
	isObj(false),
	canPhase(false),
	isPlayer(false),
	isInteractive(false),
	frontMove(false),
	sideMove(false)
	{
		//Default Constructor With Nothing :D
	}

	bool isInside;
	bool isObj;
	bool isInteractive;
	bool canPhase;
	bool isPlayer;
	bool frontMove;
	bool sideMove;

	int id;
	Child* child;

	float collisionRadius;

	Vector3 Max;
	Vector3 Min;
	Vector3 Pos;
	Vector3 Vel;
	
	virtual int getID(){ return id; }
	virtual Vector3& getPos(){ return Pos; }
	virtual Vector3& getVel(){ return Vel; }
	virtual float getCollisionRadius(){ return collisionRadius; }
	void setPos(Vector3 translation);
	void setVel(Vector3 velocity);
	bool BoundingBox::collidesWith(BoundingBox* object);

};

#endif