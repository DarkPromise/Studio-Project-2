/******************************************************************************/
/*!
\file	Light.h
\author Group 7
\brief
Header file for Light
*/
/******************************************************************************/
#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

struct Light
{
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;

	Light()
	{

	}
};

#endif