/******************************************************************************/
/*!
\file	Mesh.h
\author Group 7
\brief
Header file for Mesh
*/
/******************************************************************************/
#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"
#include "BoundingBox.h"

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_TRIANGLE_FAN,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned,unsigned);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;

	Material material;
	BoundingBox lookatBox;
};

#endif