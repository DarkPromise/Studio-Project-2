/******************************************************************************/
/*!
\file	BoundingBox.cpp
\author Group 7
\brief
CPP file for Bound Checking
*/
/******************************************************************************/
#include "BoundingBox.h"
#include "Camera.h"

void BoundingBox::setPos(Vector3 translation)
{
	Pos.x = translation.x;
	Pos.y = translation.y;
	Pos.z = translation.z;
}

void BoundingBox::setVel(Vector3 velocity)
{
	Vel.x = velocity.x;
	Vel.y = velocity.y;
	Vel.z = velocity.z;
}

bool BoundingBox::collidesWith(BoundingBox* object){
	// Square of distance between cube centres
	float d2 = Vector3::Distance2(this->Pos, object->getPos());
	// Square of the sum of the collision radii
	float sr2 = (collisionRadius + object->getCollisionRadius()) * (collisionRadius + object->getCollisionRadius());
	// Swap velocities
	if(d2 < sr2){
		Vector3 v = Vel;
		Vel = object->getVel();
		object->getVel() = v;
		return true;
	}
	return false;
}