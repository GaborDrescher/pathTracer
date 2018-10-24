#ifndef PATH_INTEGRATOR_H
#define PATH_INTEGRATOR_H

#include "integrators/integrator.h"
#include "abstraction/array.h"
#include "accelerators/accelerator.h"
#include "primitives/object.h"
#include "core/random.h"

class PathIntegrator : public Integrator
{
	public:

	void calc(Ray &ray_, PixelData &data)
	{
		Accelerator *accel = scene->accel;
	
		Ray r = ray_;
		
		Color cl(0,0,0);
		Color cf(1,1,1);

		for(int i = 0; i < 10; i++)
		{
			Object *obj = 0;
			Intersection isct;
			bool intersected = accel->intersect(&r, &isct, &obj);
		
			if((!intersected) || (obj == 0))
			{
				data.color.add(cl);
				data.samples += 1;
				return;
			}

			Material *material = obj->getMaterial();

			Query query;
			query.origin = r.origin;
			query.dir = r.dir;
			query.hitpoint = isct.hitpoint;
			query.normal = isct.normal;
			query.distance = isct.distance;

			material->sample(query);
			Color f = material->getColor();

			if(f.emissive())
			{
				Color cft = cf;
				cft.mix(query.color);
				cl.add(cft);

				data.color.add(cl);
				data.samples += 1;
				return;
			}

			if(i > 5)
			{
				real fx,fy,fz;
				f.getRGB(&fx,&fy,&fz);
				real p = fx > fy && fx > fz ? fx : fy > fz ? fy : fz;
				if(Random::getSample1D() < p) 
				{
					query.color.scale(real(1)/p);
				}
				else
				{
					data.color.add(cl);
					data.samples += 1;
					return;
				}
			}
			cf.mix(query.color);

			r = Ray(query.hitpoint, query.outdir);
		}

		data.color.add(cl);
		data.samples += 1;
	}
};

#endif
