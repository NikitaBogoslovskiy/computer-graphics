#define _USE_MATH_DEFINES
#include <math.h>
#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/GeometricBodies/Ellipsoid.h"
#include "../headers/cringetracer/GeometricBodies/Plane.h"
#include "../headers/cringetracer/GeometricBodies/Cylinder.h"
#include "../headers/cringetracer/GeometricBodies/Cone.h"
#include "../headers/cringetracer/Lights/PointLight.h"
#include "../headers/cringetracer/Materials/Material.h"

Scene::Scene()
{
	// cone 1
	auto cone = new Cone(HVec<double> {0.0, -1.0, -3.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f });
	Material* gold;
	if (materials.Lookup("gold", gold)) cone->SetMaterial(gold);
	bodies.push_back(cone);

	// cone 2
	auto cone2 = new Cone(HVec<double> {2.0, -1.0, -3.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f });

	Material* lightBlue;
	if (materials.Lookup("lightBlue", lightBlue)) cone2->SetMaterial(lightBlue);
	bodies.push_back(cone2);
	/*bodies.push_back(new Cylinder(HVec<double> {0.0, -0.5, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f }));*/

	// sph1
	auto sph = new Sphere(0.0, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* water;
	if (materials.Lookup("mirror", water)) sph->SetMaterial(water);
	bodies.push_back(sph);

	// sph2
	auto sph2 = new Sphere(1.5, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* air;
	if (materials.Lookup("air", air)) sph2->SetMaterial(air);
	bodies.push_back(sph2);

	// eggs
	auto el1 = new Ellipsoid(-1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f });
	el1->SetMaterial(lightBlue);

	auto el2 = new Ellipsoid(1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f });
	el2->SetMaterial(lightBlue);

	// floor
	auto plane = new Plane(HVec<double> {0.0, 0.75, 0.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {10.0, 10.0, 10.0},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f });
	plane->SetMaterial(lightBlue);
	bodies.push_back(plane);

	lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 10.0, HVec<double> { 1.0, 0.0, 0.0 }, 1.0));
	lights.push_back(new PointLight(HVec<double> { -45.0, 100.0 }, 10.0, HVec<double> { 0.0, 0.0, 1.0 }, 1.0));
}

Scene::~Scene()
{

}