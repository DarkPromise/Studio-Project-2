#ifndef CHILD_H
#define CHILD_H

#include <algorithm>
#include <vector>
#include "Vector3.h"

using std::vector;

//extern void DebugOut(const char *);

class BoundingBox;

const int NUMBER_OF_CHILDREN = 8;

class Child
{
public :
	Child(Child* parent, int levels, float CentreX, float CentreY, float CentreZ, float SizeX, float SizeY, float SizeZ);

	~Child(void);

	void createSubPartitions(int NumberOfLevels);
	void addObject(BoundingBox* object);
	void removeObject(int objectID);

	bool contains(BoundingBox* object);

	void NumberOfCollisions(int &NumberOfTests, int &NumberOfCollisions);
	void NumberOfBorderCollisions(Child* child, int &NumberOfTests, int &NumberOfCollisions);

	Child* getRoot();

	bool hasChildren()
	{
		return child[0] != NULL;
	}

	void printDetails();
	void printShortDetails();
	void Render();

private:
	Child* parent;
	Child* child[8];

	vector<BoundingBox*> objects;

	float centreX, centreY, centreZ;
	float lowX, highX;
	float lowY, highY;
	float lowZ, highZ;

	int level;

	Child(void){};

	Vector3 verts[8];

	float r,g,b,a;

	void calcCoordinates();
	void drawFace(int v0, int v1, int v2, int v3);
	void drawEdge(int v0, int v1);
};


#endif