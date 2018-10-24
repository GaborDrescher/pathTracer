#ifndef COLOR_H
#define COLOR_H

#include "abstraction/types.h"
#include "math/vec.h"

class Color
{
	private:
	vec color;

	public:
	Color() {}
	Color(real r, real g, real b) { color = vec(r,g,b);}
	Color(vec rgb) : color(rgb) {}
	Color(const Color &other) : color(other.color) {}
	void mix(const Color &other)
	{
		color = color.mult(other.color);
	}
	void scale(real x)
	{
		color = color * x;
	}

	void add(const Color &other)
	{
		color = color + other.color;
	}

	void getRGB(real *r, real *g, real *b)
	{
		*r = color.x;
		*g = color.y;
		*b = color.z;
	}

	bool emissive()
	{
		return (color.x > real(1)) || (color.y > real(1)) || (color.z > real(1));
	}
	bool approxBlack()
	{
		return Math::approxZero(color.x) && Math::approxZero(color.y) && Math::approxZero(color.z);
	}
};

#endif

