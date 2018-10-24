#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include "core/scene.h"
#include "core/random.h"
#include "core/pixeldata.h"

extern void writeOut(PixelData *data, int width, int height);
extern Scene* makeStaticScene(int width, int height, int spp);

int main()
{
	const int spp = 16;
	const int width  = 512;
	const int height = 512;

	// load objects, materials, camera, integrator and accelerator
	Scene *scene = makeStaticScene(width, height, spp);

	PixelData *data = new PixelData[width * height];

	// seed random number generator with thread-specific seed
	uint64_t seed = 42;
	seed += (uint64_t)((uintptr_t)(&seed));
	Random::init(seed);

	//for(;;) {
		// call scene->render(x, y)
		for(int i = 0; i < height; i++) {
			for(int k = 0; k < width; ++k) {
				PixelData pd = data[k + i * width];
				scene->render(k, i, pd);
				data[k + i * width] = pd;
			}
		}

		writeOut(data, width, height);
	//}

	return 0;
}

