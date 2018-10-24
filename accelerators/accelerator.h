#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include "primitives/object.h"
#include "abstraction/array.h"
#include "core/intersection.h"

class Accelerator
{
	public:
	virtual void setObjects(Array<Object*> *objects) = 0;
	virtual bool intersect(Ray *ray) = 0;
	virtual bool intersect(Ray *ray, Intersection *isct, Object **obj) = 0;
};

#endif
