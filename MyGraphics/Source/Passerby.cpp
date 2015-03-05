/******************************************************************************/
/*!
\file	Passerby.cpp
\author Group 7
\brief
CPP file for Passerby
*/
/******************************************************************************/
#include "Passerby.h"

Passerby::Passerby(void)
{
	this->Waypoints.push_back(PasserbyEndWaypoint);
}

Passerby::~Passerby(void)
{
}

void Passerby::updateAI(void)
{
	for ( int i = 0; i < this->AICurrent; i++ )
	{
		//std::cout << "AICurrent: " << this->AICurrent << std::endl;
		//std::cout << "Coordinates: " << this->Coordinates[i].x << ", " << this->Coordinates[i].y << ", " << this->Coordinates[i].z << std::endl;
		//std::cout << "Waypoints: " << this->Waypoints[Destination].x << ", " << this->Waypoints[Destination].y << ", " << this->Waypoints[Destination].z << std::endl;

		if ( this->State[i] == SpawntoDestination )
		{
			this->rotateY[i] = passerbyForward;
			if ( this->Coordinates[i].z > this->Waypoints[Destination].z )
			{
				this->Coordinates[i].z -= passerbySpeed;
			}
			else
			{
				this->Coordinates.erase(this->Coordinates.begin() + i);
				this->rotateY.erase(this->rotateY.begin() + i);
				this->AICurrent -= 1;
			}
		}
	}
}

void Passerby::spawnAI(void)
{
	if ( this->AICurrent < this->AILimit )
	{
		int SpawnChance = rand() % this->spawnRate;
		if ( SpawnChance == 1 )
		{
			if ( ( rand() % 1 ) == 0 )
			{
				this->Coordinates.push_back(PasserbySpawn1);
				this->rotateY.push_back(passerbyForward);
				this->State.push_back(SpawntoDestination);
				this->Max.push_back(PasserbyBounds);
				this->Min.push_back(-PasserbyBounds);
				this->AICurrent += 1;
			}
			else
			{
				this->Coordinates.push_back(PasserbySpawn2);
				this->rotateY.push_back(passerbyForward);
				this->State.push_back(SpawntoDestination);
				this->Max.push_back(PasserbyBounds);
				this->Min.push_back(-PasserbyBounds);
				this->AICurrent += 1;
			}
		}
	}
}

void Passerby::interactWith(void)
{
}