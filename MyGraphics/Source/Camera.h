/******************************************************************************/
/*!
\file	Camera.h
\author Group 7
\brief
Header file for Camera
*/
/******************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 view;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif