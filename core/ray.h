#ifndef RAY_H
#define RAY_H

#include "abstraction/types.h"
#include "math/vec.h"

class Ray
{
	public:
	vec origin;
	vec dir;
	real mint;
	real maxt;

	Ray() {}

	Ray(const vec o, const vec d)
	{
		origin = o;
		dir = d;
		mint = EPSILON;
		maxt = REAL_MAX;
	}

	vec getPoint(real dist)
	{
		return origin + (dir * dist);
	}
};

#endif

