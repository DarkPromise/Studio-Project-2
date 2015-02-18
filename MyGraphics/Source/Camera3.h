/******************************************************************************/
/*!
\file	Camera3.h
\author Chong Jun Xiang
\par	email: Chongjx60@gmail.com
\brief
Header file for Camera3 Class. It is the current camera that the scene uses.
*/
/******************************************************************************/
#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "Mtx44.h"


/******************************************************************************/
/*!
		Class Camera3:
\brief	Defines the default values of the Camera's properties (position, target, up, view) and its various states (Init, Update, Reset).
*/
/******************************************************************************/
class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 defaultView;
	Vector3 maxWorld;
	Vector3 minWorld;

	float viewAngle;

	Mtx44 rotation;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif