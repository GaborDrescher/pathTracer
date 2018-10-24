#include "loaders/plyloader.h"

#include "core/color.h"
#include "core/camera.h"
#include "core/scene.h"

#include "integrators/debugintegrator.h"
#include "integrators/pathintegrator.h"

#include "accelerators/bfaccel.h"
#include "accelerators/bbaccel.h"

#include "materials/diffuse.h"
#include "materials/mirror.h"
#include "materials/glas.h"

#include "meshes/dragon.h"
#include "meshes/back.h"
#include "meshes/ceil.h"
#include "meshes/dragon.h"
#include "meshes/floor.h"
#include "meshes/left.h"
#include "meshes/light.h"
#include "meshes/right.h"
#include "meshes/largebox.h"
#include "meshes/smallbox.h"

#include "primitives/trianglemesh.h"
#include "primitives/sphere.h"

Scene* makeStaticScene(int width, int height, int spp)
{
	Scene *scene = new Scene;

	// choose accelerator
	//BruteForceAccel *accel = new BruteForceAccel;
	BoxAccel *accel = new BoxAccel;

	// choose integrator
	//DebugIntegrator *integrator = new DebugIntegrator;
	PathIntegrator *integrator = new PathIntegrator;

	scene->accel = accel;
	scene->integrator = integrator;
	scene->width = width;
	scene->height = height;
	scene->spp = spp;

	// camera
	Camera *cam = new Camera(vec(27.8, 27.3, -80.0), vec(27.8, 27.3, 0.0), vec(0,1,0), 39.5, width, height);
	scene->camera = cam;

	// load scene

	PLYLoader loader;

	Color diff(0.75, 0.75, 0.75);
	Color lightmul(30.0, 30.0, 30.0);
	Color white(1,1,1);
	Color rwhite(1,1,1);
	Color red(0.57,0.025,0.025);
	Color green(0.025,0.236,0.025);
	Color lightcolor(1.0, 0.67, 0.21);

	lightcolor.mix(lightmul);
	white.mix(diff);
	red.mix(diff);
	green.mix(diff);

	GlasMaterial *material = new GlasMaterial(Color(1.0, 1.0, 1.0));
	TriangleMesh *lmesh = new TriangleMesh(*material, true);

	loader.load(lmesh, dragonString, sizeof(dragonString));

	real invvert = real(1) / lmesh->nverts;
	vec center;
	for(size_t i = 0; i != lmesh->nverts; ++i)
	{
		center = center + (lmesh->vertices[i].pos * invvert);
	}
	for(size_t i = 0; i != lmesh->nverts; ++i)
	{
		lmesh->vertices[i].pos = lmesh->vertices[i].pos - center;

		real alph = 3.141 * 1.10;
		real ca = Math::cos(alph);
		real sa = Math::sin(alph);
		vec rot = vec(lmesh->vertices[i].pos * vec(ca,0,sa), lmesh->vertices[i].pos * vec(0,1,0), lmesh->vertices[i].pos * vec(-sa,0,ca));
		lmesh->vertices[i].pos = rot;

		lmesh->vertices[i].pos = lmesh->vertices[i].pos.mult( vec(200,200,200) ) + vec(27,11.5,27);

		lmesh->vertices[i].pos.x += real(1);
	}
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}




//	{
//	DiffuseMaterial &mat = *(new DiffuseMaterial(white));
//	TriangleMesh *lmesh = new TriangleMesh(mat, false);
//	loader.load(lmesh, smallboxString, sizeof(smallboxString));
//	lmesh->init();
//	for(size_t i = 0; i != lmesh->ntris; ++i)
//	{
//		scene->objects.add(&(lmesh->triangles[i]));
//	}
//	}
//	{
//	DiffuseMaterial &mat = *(new DiffuseMaterial(white));
//	//MirrorMaterial &mat = *(new MirrorMaterial(white));
//	TriangleMesh *lmesh = new TriangleMesh(mat, false);
//	loader.load(lmesh, largeboxString, sizeof(largeboxString));
//	lmesh->init();
//	for(size_t i = 0; i != lmesh->ntris; ++i)
//	{
//		scene->objects.add(&(lmesh->triangles[i]));
//	}
//	}




	//light mesh
	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(lightcolor));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, lightString, sizeof(lightString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}
    



	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(white));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, floorString, sizeof(floorString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}

	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(white));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, ceilString, sizeof(ceilString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}

	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(white));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, backString, sizeof(backString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}
	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(green));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, rightString, sizeof(rightString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}
	{
	DiffuseMaterial &mat = *(new DiffuseMaterial(red));
	//GlasMaterial material(Color(1,1,1));
	TriangleMesh *lmesh = new TriangleMesh(mat, false);
	loader.load(lmesh, leftString, sizeof(leftString));
	lmesh->init();
	for(size_t i = 0; i != lmesh->ntris; ++i)
	{
		scene->objects.add(&(lmesh->triangles[i]));
	}
	}


	// init internal accelerator and integrator with loaded objects
	scene->init();

	return scene;
}
