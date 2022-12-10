#define _USE_MATH_DEFINES
#include <math.h>
#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/GeometricBodies/Ellipsoid.h"
#include "../headers/cringetracer/GeometricBodies/Plane.h"

#include "../headers/cringetracer/Lights/PointLight.h"
#include "../headers/cringetracer/Materials/Material.h"
#include <memory>

Scene::Scene()
{
	bodies.push_back(new Ellipsoid(-1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f }));
	bodies.push_back(new Sphere(0.0, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f }));
	bodies.push_back(new Ellipsoid(1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f }));
	bodies.push_back(new Plane(HVec<double> {0.0, 0.75, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {4.0, 4.0, 4.0},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f }));

	//lol
	std::map<std::string, Material*> mtlLib{
		{"brick", new Material()},
		{"lightBlue", new Material(HVec<double> {0.25, 0.3, 0.8}, 10.0, 0.8)},
		{"mint", new Material(HVec<double> { 110.0 / 255.0, 245.0 / 255.0, 128.0 / 255.0, }, 10.0, 0.5)},
		{"mirror", new Material(HVec<double> { 1.0, 1.0, 1.0, }, 0.0, 0.9)}
	};

	bodies.at(0)->SetMaterial(mtlLib["brick"]);
	bodies.at(1)->SetMaterial(mtlLib["lightBlue"]);
	bodies.at(2)->SetMaterial(mtlLib["mint"]);
	bodies.at(3)->SetMaterial(mtlLib["mirror"]);

	//lights.push_back(new PointLight(HVec<double> {5.0, -5.0, 10.0}, HVec<double> { 159.f / 255.f, 44.f / 255.f, 214.f / 255.f }, 1.0));
	lights.push_back(new PointLight(HVec<double> {-5.0, -10.0, 5.0}, HVec<double> { 1.f, 0.f, 0.f }, 1.0));
	//lights.push_back(new PointLight(HVec<double> {0.0, -10.0, 5.0}, HVec<double> { 0.f, 1.f, 0.f }, 1.0));
	lights.push_back(new PointLight(HVec<double> {0.0, -10.0, 5.0}, HVec<double> { 1.f, 1.f, 1.f }, 1.0));
}

Scene::~Scene()
{

}