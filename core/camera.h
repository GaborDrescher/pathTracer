#ifndef CAMERA_H
#define CAMERA_H

#include "abstraction/types.h"
#include "math/vec.h"
#include "core/ray.h"

class Camera
{
	private:
	real fov;
	real near;
	real width;
	real height;
	vec pos;
	vec viewDir;
	vec up;
	vec left;
	vec corner;

	public:
	Camera(vec pos_, vec lookat_, vec up_, real fovDeg_, int w_, int h_)
	{
		//pos_ = vec(pos_.x, pos_.z, pos_.y);
		fov = fovDeg_ * (real(M_PI)/real(180));
		near = real(0.5) / tan(real(0.5) * fov);
		
		width = w_;
		height = h_;
		pos = pos_;

		//base vectors
		viewDir = lookat_ - pos;
		viewDir.normalize();

		up = up_;
		up.normalize();
				
		left = up % viewDir;
		left.normalize();
		
		up = viewDir % left;
		up.normalize();
		
		corner = pos + (viewDir * near) + (left * real(0.5))+ (up * (height/width) * real(0.5));
	}

	void getRay(Ray *ray, real x, real y)
	{
		real xn = x/width;
		real yn = y/height;

		ray->origin = (corner - (left * xn)) - (up * yn);

		vec dir = ray->origin - pos;
		real dlen = dir.normalize();
		ray->dir = dir;

		ray->mint = dlen;
		ray->maxt = REAL_MAX;
	}
};

#endif
