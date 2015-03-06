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

/******************************************************************************/
/*!
\brief
Sets the position of the bounding box

\param translation - changes the box position to its translation

\exception None
\return None
*/
/******************************************************************************/
void BoundingBox::setPos(Vector3 translation)
{
	Pos.x = translation.x;
	Pos.y = translation.y;
	Pos.z = translation.z;
}

/******************************************************************************/
/*!
\brief
Sets the velocity of the bounding box

\param velocity - changes the box's speed

\exception None
\return None
*/
/******************************************************************************/
void BoundingBox::setVel(Vector3 velocity)
{
	Vel.x = velocity.x;
	Vel.y = velocity.y;
	Vel.z = velocity.z;
}

/******************************************************************************/
/*!
\brief
Checks if any bounding box collides with each other in the Octree

\param object - the opposing bounding box to check with

\exception None
\return None
*/
/******************************************************************************/

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