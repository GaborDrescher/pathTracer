#define PPM_OUTPUT
//#define RAW_OUTPUT

#include <stdio.h>
#include <stdint.h>

#include "core/pixeldata.h"

void writeOut(PixelData *data, int width, int height)
{
	//const char * tmppath = "output/tmp.ppm";
	//const char * outpath = "output/out.ppm";

	FILE *outstream = stdout;//fopen(tmppath, "w");
	//fprintf(outstream, "TRACER_START\n");

    #ifdef RAW_OUTPUT
	fprintf(outstream, "%d %d\n", (int)width, (int)height);
	for(int i = 0; i < height; ++i)
	{
		for(int k = 0; k < width; ++k)
		{
			real r, g, b;
			data[i * width + k].color.getRGB(&r, &g, &b);
			uint32_t samples = data[i*width+k].samples;
			fprintf(outstream, "%.*e ", 20, r);
			fprintf(outstream, "%.*e ", 20, g);
			fprintf(outstream, "%.*e ", 20, b);

			fprintf(outstream, "%u\n", samples);

		}
	}
	#endif

    #ifdef PPM_OUTPUT
	//ppm header 256 colors per channel
	fprintf(outstream, "P3\n%d %d\n%d\n", (int)width, (int)height, 255);
	for(int i = 0; i < height; ++i)
	{
		for(int k = 0; k < width; ++k)
		{
			real r, g, b;
			data[i * width + k].color.getRGB(&r, &g, &b);
			uint32_t samples = data[i*width+k].samples;
			real invsamples = real(1) / samples;
			if(samples > 0)
			{
				r *= invsamples;
				g *= invsamples;
				b *= invsamples;
			}
			r = Math::pow(r, real(1)/real(2.2));
			g = Math::pow(g, real(1)/real(2.2));
			b = Math::pow(b, real(1)/real(2.2));
			int ir = Math::clamp((int)(r * real(255)), 0, 255);
			int ig = Math::clamp((int)(g * real(255)), 0, 255);
			int ib = Math::clamp((int)(b * real(255)), 0, 255);
			fprintf(outstream, "%d %d %d\n", ir, ig, ib);
		}
	}
    #endif

	//fprintf(outstream, "TRACER_END\n");
    fflush(outstream);
	//fclose(outstream);

	//rename(tmppath, outpath);
    //system("convert output/out.ppm output/outtmp.png");
	//rename("output/outtmp.png", "output/out.png");
}
