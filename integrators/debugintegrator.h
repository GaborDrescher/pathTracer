#ifndef DEBUG_INTEGRATOR_H
#define DEBUG_INTEGRATOR_H

#include "integrators/integrator.h"
#include "abstraction/array.h"
#include "accelerators/accelerator.h"
#include "primitives/object.h"
#include "core/intersection.h"

class DebugIntegrator : public Integrator
{
	void calc(Ray &ray, PixelData &data)
	{
		Accelerator *accel = scene->accel;
		
		Object *obj;
		Intersection isct;

		bool intersected = accel->intersect(&ray, &isct, &obj);

		if(intersected)
		{
			//Material *material = obj->getMaterial();
			//vec normal = isct.normal;
			//real intens = normal * (ray.dir * real(-1));
			//vec intv = vec(intens,intens,intens);
			//Color intc(intv);
			//intc.mix(material->getColor());
			//data.color.add(intc);

			vec normal = isct.normal;
			if(normal.x < real(0)) normal.x *= real(-1);
			if(normal.y < real(0)) normal.y *= real(-1);
			if(normal.z < real(0)) normal.z *= real(-1);
			data.color.add(Color(normal));

			//Material *material = obj->getMaterial();
			//data.color.add(material->getColor());

			data.samples += 1;
		}
	}
};

#endif
