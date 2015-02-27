#ifndef OCTREE_H
#define OCTREE_H

#include "Vector3.h"
#include "BoundingBox.h"
#include "Child.h"

class Octree
{
public:
	Child* root;

	Octree(void);
	~Octree(void);

	void CreateTree(int levels, float centreX, float centreY, float centreZ, float sizeX, float sizeY, float sizeZ);
	void NumberOfCollisions(int &NumberOfTests, int &NumberOfCollisionsDetected);
	void AddObject(BoundingBox *obj);
	void printTree(int type);
	void renderParts();
};

#endif
