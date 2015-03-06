/******************************************************************************/
/*!
\file	Child.cpp
\author Group 7
\brief
CPP file for Child
*/
/******************************************************************************/
#include "Child.h"
#include "BoundingBox.h"

/******************************************************************************/
/*!
\brief
default constructor of a octree child

\param parent - the original octree
\param level - the number of levels the child's bounding box should have
\param centreX to centreZ - the position to create the child at
\param sizeX to sizeZ - the size of the child's bounding box

\exception None
\return None
*/
/******************************************************************************/

Child::Child(Child *parent, int level, 	float centreX, float centreY, float centreZ, float sizeX, float sizeY, float sizeZ)
{
	this->parent = parent;
	this->level = level;
	this->centreX = centreX;
	this->centreY = centreY;
	this->centreZ = centreZ;

	lowX = centreX - sizeX/2;
	highX = centreX + sizeX/2;
	lowY = centreY - sizeY/2;
	highY = centreY + sizeY/2;
	lowZ = centreZ - sizeZ/2;
	highZ = centreZ + sizeZ/2;

	for(int i = 0; i < 8; i++)
	{
		child[i] = NULL;
	}

	r = 0.5f;
	g = (level * 0.2f);
	b = 0.0f;
	a = 0.1f;
	calcCoordinates();
}

/******************************************************************************/
/*!
\brief
destructor of an octree child

\param None
\exception None
\return None
*/
/******************************************************************************/

Child::~Child(void)
{
	for(int i = 0; i < 8; i++)
	{
		delete child[i];
	}
	objects.clear();
}

/******************************************************************************/
/*!
\brief
Gets the parent of the child part

\param None
\exception None
\return pointer of child's parent
*/
/******************************************************************************/

Child* Child::getRoot()
{
	if(parent == NULL)
	{
		return this;
	} else {
		return parent->getRoot();
	}
}

/******************************************************************************/
/*!
\brief
Check for collision using AABB checking

\param Obj - The object to check with
\exception None
\return True or False
*/
/******************************************************************************/

bool Child::contains(BoundingBox* obj)
{
	float nLowX = obj->getPos().x - obj->getCollisionRadius();
	float nHighX = obj->getPos().x + obj->getCollisionRadius();
	float nLowY = obj->getPos().y - obj->getCollisionRadius();
	float nHighY = obj->getPos().y + obj->getCollisionRadius();
	float nLowZ = obj->getPos().z - obj->getCollisionRadius();
	float nHighZ = obj->getPos().z + obj->getCollisionRadius();

	return(nLowX > lowX && nHighX < highX && nLowY > lowY && nHighY < highY && nLowZ > lowZ && nHighZ < highZ);
}

/******************************************************************************/
/*!
\brief
Adds an object to the child

\param obj - The object to add to the child
\exception None
\return None
*/
/******************************************************************************/

void Child::addObject(BoundingBox *obj)
{
	if(child[0] == NULL)
	{		//moves down the roots, checking if each has children.
		obj->child = this;		//tell the cube it belongs in this partition
		objects.push_back(obj);		//vector function to store pointer
		return;
	}

	int p = 0;

	p += (obj->getPos().x < centreX) ? 0 : 1;
	p += (obj->getPos().y < centreY) ? 2 : 0;
	p += (obj->getPos().z < centreZ) ? 4 : 0;

	if(child[p]->contains(obj))
	{
		child[p]->addObject(obj);
	}
	else
	{
		obj->child = this;
		objects.push_back(obj);
	}
}

/******************************************************************************/
/*!
\brief
Removes an object from the child

\param id - Id of the object to be removed
\exception None
\return None
*/
/******************************************************************************/

void Child::removeObject(int id)
{
	vector<BoundingBox*>::iterator it;
	for(it = objects.begin(); it != objects.end(); it++)
	{
		if((*it)->getID() == id)
		{
			objects.erase(it);
			return;
		}
	}
}

/******************************************************************************/
/*!
\brief
Checks for number of collisions and tests

\param NumberOfTests - Number of Tests conducted
\param NumberOfCollisions - Number of Collisions detected

\exception None
\return None
*/
/******************************************************************************/

void Child::NumberOfCollisions(int &NumberOfTests, int &NumberOfCollisions)
{
	if(hasChildren())
	{
		for(int i = 0; i < 8; i++)
		{
			child[i]->NumberOfCollisions(NumberOfTests, NumberOfCollisions);
		}
	}
	int n = this->objects.size();
	if(n >= 2)
	{
		for(int i = 0; i < n-1; i++)
		{
			for(int j = i+1; j < n; j++)
			{
				NumberOfTests++;
				if(objects[i]->collidesWith(objects[j]))
					NumberOfCollisions++;
			}
		}
	}
	if( n >= 1 && parent != NULL)
		parent->NumberOfBorderCollisions(this, NumberOfTests, NumberOfCollisions);
}

/******************************************************************************/
/*!
\brief
Checks for number of tests and collisions at the intersections of the child's bounding box

\param Child - The child to check with
\param NumberOfTests - Number of Tests conducted
\param NumberOfCollisions - Number of Collisions detected

\exception None
\return None
*/
/******************************************************************************/

void Child::NumberOfBorderCollisions(Child* child, int &NumberOfTests, int &NumberOfCollisions){
	int nPart = child->objects.size();
	int n = objects.size();

	if(n > 0){
		for(int i = 0; i < nPart; i++){
			for(int j = 0; j < n; j++){
				NumberOfTests++;
				if(child->objects[i]->collidesWith(objects[j]))
					NumberOfCollisions++;
			}
		}
	}
	if(parent != NULL)
		parent->NumberOfBorderCollisions(child, NumberOfTests, NumberOfCollisions);
}

/******************************************************************************/
/*!
\brief
Creates more childs

\param NumberOfLevels - Number of levels the new child should have

\exception None
\return None
*/
/******************************************************************************/

void Child::createSubPartitions(int NumberOfLevels)
{
	if(level < NumberOfLevels - 1)
	{
		float newSizeX = (highX - lowX)/2;
		float newSizeY = (highY - lowY)/2;
		float newSizeZ = (highZ - lowZ)/2;

		child[0] = new Child(this, level + 1, centreX - newSizeX/2, centreY + newSizeY/2, centreZ + newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[1] = new Child(this, level + 1, centreX + newSizeX/2, centreY + newSizeY/2, centreZ + newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[2] = new Child(this, level + 1, centreX - newSizeX/2, centreY - newSizeY/2, centreZ + newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[3] = new Child(this, level + 1, centreX + newSizeX/2, centreY - newSizeY/2, centreZ + newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[4] = new Child(this, level + 1, centreX - newSizeX/2, centreY + newSizeY/2, centreZ - newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[5] = new Child(this, level + 1, centreX + newSizeX/2, centreY + newSizeY/2, centreZ - newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[6] = new Child(this, level + 1, centreX - newSizeX/2, centreY - newSizeY/2, centreZ - newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);
		child[7] = new Child(this, level + 1, centreX + newSizeX/2, centreY - newSizeY/2, centreZ - newSizeZ/2,
			newSizeX, newSizeY, newSizeZ);

		for(int i = 0; i < 8; i++)
		{
			child[i]->createSubPartitions(NumberOfLevels);
		}
	}
}

/******************************************************************************/
/*!
\brief
Prints the details of the child

\param None
\exception None
\return None
*/
/******************************************************************************/

void Child::printDetails()
{
	char text[256];

	sprintf_s(text, "Level %d  at %5.1f %5.1f, %5.1f   X[%5.1f to %5.1f]  y[%5.1f to %5.1f]  z[%5.2f to %5.2f] %d",
		level, centreX, centreY, centreZ, lowX, highX, lowY, highY, lowZ, highZ, objects.size());

	if(child[0] != NULL)
	{
		for(int i = 0; i < 8; i++)
		{
			child[i]->printDetails();
		}
	}
}

/******************************************************************************/
/*!
\brief
Prints the shorter details of the child

\param None
\exception None
\return None
*/
/******************************************************************************/

void Child::printShortDetails()
{
	char text[256];

	sprintf_s(text, "Level %d  at %5.1f %5.1f, %5.1f No of objects %d", level, centreX, centreY, centreZ, objects.size());

	if(child[0] != NULL)
	{
		for(int i = 0; i < 8; i++)
		{
			child[i]->printShortDetails();
		}
	}
}

/******************************************************************************/
/*!
\brief
Renders the child

\param None
\exception None
\return None
*/
/******************************************************************************/

void Child::Render()
{
	if(hasChildren())
		for(int i = 0; i < NUMBER_OF_CHILDREN; i++)
			child[i]->Render();

	if(objects.size() > 0)
	{
		glPushMatrix();
			// Draw edges
			glColor3f(r,g,b);
			glLineWidth(2);
			drawEdge(0,1);
			drawEdge(1,2);
			drawEdge(2,3);
			drawEdge(3,0);
			drawEdge(4,5);
			drawEdge(5,6);
			drawEdge(6,7);
			drawEdge(7,4);
			drawEdge(0,4);
			drawEdge(1,5);
			drawEdge(2,6);
			drawEdge(3,7);
			// Draw faces
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(r,g,b,a);
			drawFace(0, 4, 5, 1);
			drawFace(3, 7, 4, 0);
			drawFace(2, 6, 7, 3);
			drawFace(1, 5, 6, 2);
			drawFace(3, 0, 1, 2);
			drawFace(4, 7, 6, 5);
			glDisable(GL_BLEND);
		glPopMatrix();
	}
}

/******************************************************************************/
/*!
\brief
Draws the face of the child's bounding box in openGL

\param v0 to v3 - The vertices of the child box
\exception None
\return None
*/
/******************************************************************************/

void Child::drawFace(int v0, int v1, int v2, int v3)
{
	glBegin(GL_QUADS);
		glVertex3f(verts[v0].x, verts[v0].y, verts[v0].z);
		glVertex3f(verts[v1].x, verts[v1].y, verts[v1].z);
		glVertex3f(verts[v2].x, verts[v2].y, verts[v2].z);
		glVertex3f(verts[v3].x, verts[v3].y, verts[v3].z);
	glEnd();
}

/******************************************************************************/
/*!
\brief
Draws the edge of a child's bounding box in openGL

\param v0 to v1 - The two vertices to draw a line with

\exception None
\return None
*/
/******************************************************************************/

void Child::drawEdge(int v0, int v1)
{
	glBegin(GL_LINES);
		glVertex3f(verts[v0].x, verts[v0].y, verts[v0].z);
		glVertex3f(verts[v1].x, verts[v1].y, verts[v1].z);
	glEnd();
}

/******************************************************************************/
/*!
\brief
Sets where the child's bounding box should be drawn

\param None
\exception None
\return None
*/
/******************************************************************************/

void Child::calcCoordinates()
{
	verts[0].Set(lowX,  highY, highZ);
	verts[1].Set(highX, highY, highZ);
	verts[2].Set(highX, highY, lowZ);
	verts[3].Set(lowX,  highY, lowZ);
	verts[4].Set(lowX,  lowY,  highZ);
	verts[5].Set(highX, lowY,  highZ);
	verts[6].Set(highX, lowY,  lowZ);
	verts[7].Set(lowX,  lowY,  lowZ);
}