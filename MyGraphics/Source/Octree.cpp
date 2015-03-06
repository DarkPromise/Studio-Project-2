/******************************************************************************/
/*!
\file	Octree.cpp
\author Group 7
\brief
CPP file for Octree
*/
/******************************************************************************/
#include "Octree.h"

/******************************************************************************/
/*!
\brief
default constructor of an octree

\param None
\exception None
\return None
*/
/******************************************************************************/

Octree::Octree(void)
{
	root = NULL;
	std::cout << "Tree Created" << std::endl;
}

/******************************************************************************/
/*!
\brief
destructor of an octree

\param None
\exception None
\return None
*/
/******************************************************************************/

Octree::~Octree(void)
{
	delete root;
}

/******************************************************************************/
/*!
\brief
Creates the initial octree

\param NumberOfLevels - number of levels the octree should have
\param CentreX to CentreZ - Initial position in world space to create the octree
\param SizeX to SizeZ - Size of the Initial Octree

\exception None
\return None
*/
/******************************************************************************/

void Octree::CreateTree(int NumberOfLevels, float centreX, float centreY, float centreZ, float sizeX, float sizeY, float sizeZ)
{
	root = new Child(NULL, 0, centreX, centreY, centreZ, sizeX, sizeY, sizeZ);
	root->createSubPartitions(NumberOfLevels);
}

/******************************************************************************/
/*!
\brief
Keeps track of number of collisions and test

\param NumberOfTests - Number of tests conducted
\param NumberOfCollisions - Number of collisions detected

\exception None
\return None
*/
/******************************************************************************/

void Octree::NumberOfCollisions(int &NumberOfTests, int &NumberOfCollisions)
{
	NumberOfTests = 0;
	NumberOfCollisions = 0;
	root->NumberOfCollisions(NumberOfTests,NumberOfCollisions);
}

/******************************************************************************/
/*!
\brief
Adds an object to the octree

\param Object - The object to be added

\exception None
\return None
*/
/******************************************************************************/

void Octree::AddObject(BoundingBox* object)
{
	root->addObject(object);
}

/******************************************************************************/
/*!
\brief
Prints the details of the tree

\param type - Print either the entire octree or child details

\exception None
\return None
*/
/******************************************************************************/

void Octree::printTree(int type)
{
	if(root != NULL){
		switch(type) {
			case 0:
				root->printDetails();
				break;
			case 1:
				root->printShortDetails();
				break;
		}
	}
}

/******************************************************************************/
/*!
\brief
Renders the octree

\param None
\exception None
\return None
*/
/******************************************************************************/

void Octree::renderParts()
{
	root->Render();
}
