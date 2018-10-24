#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "core/pixeldata.h"
#include "core/ray.h"
#include "core/scene.h"

class Integrator
{
	protected:
	Scene *scene;

	public:
	void setScene(Scene *sc)
	{
		scene = sc;
	}

	virtual void calc(Ray &ray, PixelData &data) = 0;
};

#endif
