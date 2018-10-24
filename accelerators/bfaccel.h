#ifndef BF_ACCEL_H
#define BF_ACCEL_H

#include "accelerators/accelerator.h"
#include "primitives/object.h"
#include "config.h"

class BruteForceAccel : public Accelerator
{
	private:
	Array<Object*> objs;

	public:
	void setObjects(Array<Object*> objects)
	{
		objs = objects;
	}

	bool intersect(Ray *ray)
	{
		Object *obj;
		Intersection isct;
		return intersect(ray, &isct, &obj);
	}

	bool intersect(Ray *ray, Intersection *isct, Object **obj)
	{
		real dist = REAL_MAX;
		Object *nearest = 0;
		Intersection bestisct;

		const size_t nobjs = objs.getsize();
		for(size_t i = 0; i != nobjs; ++i)
		{
			Object *current = objs[i];
			if(current->intersect(ray, isct))
			{
				if(isct->distance < dist)
				{
					dist = isct->distance;
					nearest = objs[i];
					bestisct = *isct;
				}
			}
		}

		if(nearest != 0)
		{
			*obj = nearest;
			*isct = bestisct;
			return true;
		}
		else
		{
			return false;
		}
	}
};

#endif
