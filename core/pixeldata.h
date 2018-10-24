#ifndef PIXEL_DATA_H
#define PIXEL_DATA_H

#include "core/color.h"
#include "math/real.h"
#include "abstraction/types.h"

class PixelData
{
	public:
	Color color;
	uint32_t samples;
	PixelData() : samples(0) {}

	static void clear(PixelData pd[], uint32_t width, uint32_t height)
    {
        Color cleared;
		const uint32_t size = width * height;
		for(uint32_t i = 0; i != size; ++i)
		{
            pd[i].color = cleared;
            pd[i].samples = 0;
		}
    }

	static void combine(PixelData dest[], PixelData src[], uint32_t width, uint32_t height)
	{
		const uint32_t size = width * height;
		for(uint32_t i = 0; i != size; ++i)
		{
			if(src[i].samples > 0)
			{
				dest[i].color.add(src[i].color);
				dest[i].samples += src[i].samples;
			}
		}
	}
};

#endif
