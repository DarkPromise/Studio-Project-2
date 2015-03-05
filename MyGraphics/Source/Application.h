/******************************************************************************/
/*!
\file	Application.h
\author Group 7
\brief
Header file for Application
*/
/******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Octree.h"

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Run();
	void Exit();

	static bool keys[1024];

	static bool IsKeyPressed(unsigned short key);
	static bool IsButtonPressed(unsigned int);
	static void derpClose();

	static int getWidth();
	static int getHeight();

	double xpos;
    double ypos;
private:
	//Declare a window objec
	static const int width;
	static const int height;
	StopWatch m_timer;
};

#endif