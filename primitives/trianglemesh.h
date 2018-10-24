#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "abstraction/types.h"
#include "materials/material.h"
#include "primitives/vertex.h"

class Triangle;

class TriangleMesh
{
	public:
	Material *material;
	bool smoothed;

	Vertex *vertices;
	size_t nverts;

	Triangle *triangles;
	size_t ntris;

	TriangleMesh(const Material &mat, bool smooth) : material((Material*)&mat), smoothed(smooth) {}

	void init();
};



#endif
