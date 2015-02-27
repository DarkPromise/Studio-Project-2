#include "Octree.h"

Octree::Octree(void)
{
	root = NULL;
	std::cout << "Tree Created" << std::endl;
}


Octree::~Octree(void)
{
	delete root;
}

void Octree::CreateTree(int NumberOfLevels, float centreX, float centreY, float centreZ, float sizeX, float sizeY, float sizeZ)
{
	root = new Child(NULL, 0, centreX, centreY, centreZ, sizeX, sizeY, sizeZ);
	root->createSubPartitions(NumberOfLevels);
}

void Octree::NumberOfCollisions(int &NumberOfTests, int &NumberOfCollisions)
{
	NumberOfTests = 0;
	NumberOfCollisions = 0;
	root->NumberOfCollisions(NumberOfTests,NumberOfCollisions);
}

void Octree::AddObject(BoundingBox* object)
{
	root->addObject(object);
}

void Octree::printTree(int type)
{
	//DebugOut("==================== TREE ====================");
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
	//DebugOut("==============================================");
}

void Octree::renderParts()
{
	root->Render();
}
