#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "math/real.h"
#include "math/vec.h"

class Intersection
{
	public:
	real distance;
	vec hitpoint;
	vec normal;
	Intersection() : distance(0) {}
};

#endif
