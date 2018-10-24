#ifndef VERTEX_H
#define VERTEX_H

#include "math/vec.h"

class Vertex
{
	public:
	vec pos;
	vec normal;
	real tmp;
	Vertex() : tmp(0) {}
	Vertex(const vec &v) : pos(v), tmp(0) {}
	Vertex(real x, real y, real z) : pos(x,y,z), tmp(0) {}
};


#endif
