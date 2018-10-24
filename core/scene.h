#ifndef SCENE_H
#define SCENE_H

#include "abstraction/types.h"
#include "abstraction/array.h"

class Integrator;
class Camera;
class Object;
class PixelData;
class Accelerator;
class Integrator;

class Scene
{
	public:
	uint32_t width;
	uint32_t height;
	uint32_t spp;

	Camera *camera;
	Array<Object*> objects;
	Accelerator *accel;
	Integrator *integrator;

	void render(int ix, int iy, PixelData &target);
	void init();
};

#endif
