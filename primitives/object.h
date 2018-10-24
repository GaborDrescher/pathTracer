#ifndef OBJECT_H
#define OBJECT_H

#include "abstraction/types.h"
#include "math/vec.h"
#include "core/ray.h"
#include "core/aabb.h"
#include "core/intersection.h"
#include "materials/material.h"

class Object
{
	public:
	virtual bool intersect(Ray *ray, Intersection *isct) = 0;
	virtual void getAABB(AABB *aabb) = 0;
	virtual Material* getMaterial() = 0;
};

#endif
