#ifndef AABB_H
#define AABB_H

#include "abstraction/types.h"
#include "config.h"
#include "math/vec.h"

class AABB
{
	public:
	vec minC;
	vec maxC;

	AABB()
	{
		reset();
	}

	vec& get(int idx)
	{
		if(idx == 0) return minC;
		return maxC;
	}

	void reset()
	{
		minC.x = REAL_MAX;
		minC.y = REAL_MAX;
		minC.z = REAL_MAX;

		maxC.x = REAL_MIN;
		maxC.y = REAL_MIN;
		maxC.z = REAL_MIN;
	}

	void add(vec v)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(v.data[i] < minC.data[i])
			{
				minC.data[i] = v.data[i];
			}

			if(v.data[i] > maxC.data[i])
			{
				maxC.data[i] = v.data[i];
			}
		}
	}

	void add(const AABB &aabb)
	{
		add(aabb.minC);
		add(aabb.maxC);
	}

	bool isInit()
	{
		return maxC.x >= minC.x;
	}

	real getSurfaceArea() 
	{
		if(!isInit())
		{
			return 0;
		}

		vec lengths = maxC - minC;
		return real(2) * (lengths[0] * lengths[1] + lengths[1] * lengths[2] + lengths[0] * lengths[2]);
	}

	vec getMidpoint()
	{
		return (minC + maxC) * real(0.5);
	}

	int maxAxis() const
	{
		vec delta = maxC-minC;
		if(delta.x > delta.y)
			return (delta.x > delta.z ? 0 : 2);
		else
			return (delta.y > delta.z ? 1 : 2);
	}

	bool intersect(const Ray &ray, const vec &invDir, const bool dirIsNeg[3])
	{
		real tmin  = (get( dirIsNeg[0]).x - ray.origin.x) * invDir.x;
		real tmax  = (get(!dirIsNeg[0]).x - ray.origin.x) * invDir.x;
		real tymin = (get( dirIsNeg[1]).y - ray.origin.y) * invDir.y;
		real tymax = (get(!dirIsNeg[1]).y - ray.origin.y) * invDir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		real tzmin = (get( dirIsNeg[2]).z - ray.origin.z) * invDir.z;
		real tzmax = (get(!dirIsNeg[2]).z - ray.origin.z) * invDir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		return (tmin < ray.maxt) && (tmax > ray.mint);
	}
};

#endif
