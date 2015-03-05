/******************************************************************************/
/*!
\file	Camera2.cpp
\author Group 7
\brief
CPP file for Camera2
*/
/******************************************************************************/
#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"
#include <string>
#include "GLFW\glfw3.h"

bool firstInit = true;

using std::cout;
using std::endl;

/***********************************************************/
/*!
\brief
	Camera2's constructor
*/
/***********************************************************/
Camera2::Camera2()
{
}

/***********************************************************/
/*!
\brief
	Camera2's destruction
*/
/***********************************************************/
Camera2::~Camera2()
{
}

/***********************************************************/
/*
\brief
	Initialize the Camera variables values
\param pos - position of the camera

\param target - position of the target

\param up - the up direction of the camera's position
*/
/***********************************************************/
void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	defaultView = view = (target - position).Normalized();
	this->right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	yaw = 0.0f;
	pitch = 0.0f;
	delay = 0.0;
	mouseSpeed = 0.0f;
	lookSpeed = 1.0f;

	Maximum.Set(10000,10000,10000);
	Minimum.Set(-10000,-10000,-10000);
}

/***********************************************************/
/*!
\brief
	Update the camera position based on key pressed
\param dt - the delta time since the last time the function was called
*/
/***********************************************************/
void Camera2::Update(double dt,bool move, double xpos, double ypos)
{
	delay += dt;

	aspect = (float)Application::getWidth() / (float)Application::getHeight();

	double xoffset = ((Application::getWidth()/2) - xpos);
	double yoffset = (((Application::getHeight()/2) - ceil(ypos)));

	xoffset *= mouseSpeed;
	yoffset *= mouseSpeed;

	yaw += (float)xoffset;
	pitch += (float)yoffset;

	if(pitch > 90.0f)
	{
		pitch = 90.0f;
	}
	if(pitch < 45.0f)
	{
		pitch = 45.0f;
	}

	if(delay > 0.1)
	{
		/*std::cout << "Yaw : " << yaw << std::endl;
		std::cout << "Pitch : " << pitch << std::endl;*/
		delay = 0;
	}

	/*Vector3 front;
	front.x = cos(Math::DegreeToRadian(yaw) * cos(Math::DegreeToRadian(pitch)));
	front.y = sin(Math::DegreeToRadian(pitch));
	front.z = sin(Math::DegreeToRadian(yaw) * cos(Math::DegreeToRadian(pitch)));
	direction = front.Normalized();*/

	Vector3 temptarget = target;
	Vector3 tempposition = position;
	view = (target - position).Normalized();
	right = (view.Cross(up)).Normalized();
	right.y = 0;
	right.Normalize();
	up = (right.Cross(view)).Normalized();

	static const float CAMERA_SPEED = 500.f;
	static const float ROTATE_SPEED = 5.f;

	if(Application::IsKeyPressed('W') && move == true)
	{
		position += (view * CAMERA_SPEED * dt);
		target += (view * CAMERA_SPEED * dt);

		position.y = tempposition.y; //No change in Y
		target.y = temptarget.y;
	}

	if(Application::IsKeyPressed('S') && move == true)
	{
		position -= (view * CAMERA_SPEED * dt);
		target -= (view * CAMERA_SPEED * dt);

		position.y = tempposition.y; //No change in Y
		target.y = temptarget.y;
	}

	if(Application::IsKeyPressed('A') && move == true)
	{
		position -= (right * CAMERA_SPEED * 1.2 * dt);
		target -= (right * CAMERA_SPEED * 1.2 * dt);

		target.y = temptarget.y; //No change in Y
		position.y = tempposition.y;
	}

	if(Application::IsKeyPressed('D') && move == true)
	{
		position += (right * CAMERA_SPEED * 1.2 * dt);
		target += (right * CAMERA_SPEED * 1.2 * dt);

		target.y = temptarget.y; //No change in Y
		position.y = tempposition.y;
	}

	if(xoffset != 0)
	{
		Mtx44 rotation;
		target -= position;
		rotation.SetToRotation(xoffset,0,1,0);
		target = rotation * target;
		up = rotation * up;
		target += position;
	}

	if(yoffset != 0)
	{
		Mtx44 rotation;
		target -= position;
		rotation.SetToRotation(yoffset,right.x,right.y,right.z);
		target = rotation * target;
		target += position;
	}

	if(Application::IsKeyPressed(GLFW_KEY_UP))
	{
		Mtx44 rotation;
		float pitch = (float)(ROTATE_SPEED * dt);
		target -= position;
		rotation.SetToRotation(Math::RadianToDegree(pitch), right.x, right.y, right.z);
		view = rotation * view;
		target = rotation * target;
		target+=position;
	}

	if(Application::IsKeyPressed(GLFW_KEY_DOWN))
	{
		Mtx44 rotation;
		float pitch = (float)(-ROTATE_SPEED * dt);
		target -= position;
		rotation.SetToRotation(Math::RadianToDegree(pitch), right.x, right.y, right.z);
		target = rotation * target;
		target+=position;
	}

	if(Application::IsKeyPressed(GLFW_KEY_LEFT))
	{
		Mtx44 rotation;
		float yaw = (float)(ROTATE_SPEED * dt);
		target -= position;
		rotation.SetToRotation(Math::RadianToDegree(yaw), 0, 1, 0);
		target = rotation * target;
		up = rotation * up;
		target += position;
		//std::cout << "Camera Angle : " << rotation << std::endl;
	}

	if(Application::IsKeyPressed(GLFW_KEY_RIGHT))
	{
		Mtx44 rotation;
		float yaw = (float)(-ROTATE_SPEED * dt);
		target -= position;
		rotation.SetToRotation(Math::RadianToDegree(yaw), 0, 1, 0);
		target = rotation * target;
		up = rotation * up;
		target += position;
		//std::cout << "Camera Angle : " << rotation << std::endl;
	}

	if(Application::IsKeyPressed('Q'))
	{
		float yaw = (float)(-CAMERA_SPEED * 2 * dt);
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
	}

	if(Application::IsKeyPressed('E'))
	{
		float yaw = (float)(CAMERA_SPEED * 2 * dt);
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
	}

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Change(double &dx, double &dy)
{

}

/***********************************************************/
/*!
\brief
	Reset's the camera's variables to default
*/
/***********************************************************/
void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	view = defaultView;
}