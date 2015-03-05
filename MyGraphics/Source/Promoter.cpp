/******************************************************************************/
/*!
\file	Promoter.cpp
\author Group 7
\brief
CPP file for Promoter
*/
/******************************************************************************/
#include "Promoter.h"

Promoter::Promoter(void)
{
	this->rotateY.push_back(0);
}

Promoter::~Promoter(void)
{
}

void Promoter::updateAI(void)
{
	if ( this->itemRotateY < 360 )
		this->itemRotateY += itemRotateSpeed;
	else
		this->itemRotateY = 0;
}

void Promoter::spawnAI(void)
{
	if ( this->AICurrent < this->AILimit )
	{
		this->Coordinates.push_back(PromoterSpawn);
		this->rotateY.push_back(promoterFaceForward);
	}
}

void Promoter::interactWith(void)
{
}