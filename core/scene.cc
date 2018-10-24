#include "core/scene.h"
#include "integrators/integrator.h"
#include "core/camera.h"
#include "accelerators/accelerator.h"
#include "core/pixeldata.h"
#include "primitives/object.h"
#include "math/mathutil.h"
#include "core/random.h"

void Scene::render(int ix, int iy, PixelData &target)
{
	// TODO better pixel sampling
	for(uint32_t i = 0; i < spp; ++i) {
		real x = ix+Random::getSample1D();
    	real y = iy+Random::getSample1D();

		Ray ray;
		camera->getRay(&ray, x, y);
		integrator->calc(ray, target);
	}
}

void Scene::init()
{
	accel->setObjects(&objects);
	integrator->setScene(this);
}
