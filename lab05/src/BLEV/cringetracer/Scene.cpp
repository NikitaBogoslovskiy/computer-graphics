#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/Lights/PointLight.h"

Scene::Scene()
{
	bodies.push_back(new Sphere(-1.5, 1.5, 0.0, 0.5, ImVec3{ 140.f / 255.f, 30.f / 255.f, 56.f / 255.f }));
	bodies.push_back(new Sphere(0.0,  1.5, 0.0, 0.5, ImVec3{ 33.f / 255.f, 159.f / 255.f, 92.f / 255.f }));
	bodies.push_back(new Sphere(1.5,  1.5, 0.0, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f }));

	bodies.push_back(new Sphere(-1.5, 0.0, 0.0, 0.5, ImVec3{ 140.f / 255.f, 30.f / 255.f, 56.f / 255.f }));
	bodies.push_back(new Sphere(0.0, 0.0, 0.0, 0.5, ImVec3{ 33.f / 255.f, 159.f / 255.f, 92.f / 255.f }));
	bodies.push_back(new Sphere(1.5, 0.0, 0.0, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f }));

	bodies.push_back(new Sphere(-1.5, -1.5, 0.0, 0.5, ImVec3{ 140.f / 255.f, 30.f / 255.f, 56.f / 255.f }));
	bodies.push_back(new Sphere(0.0, -1.5, 0.0, 0.5, ImVec3{ 33.f / 255.f, 159.f / 255.f, 92.f / 255.f }));
	bodies.push_back(new Sphere(1.5, -1.5, 0.0, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f }));

	lights.push_back(new PointLight(HVec<double> {5.0, -5.0, 10.0}, HVec<double> { 255.0, 255.0, 255.0 }, 1.0));
}

Scene::~Scene()
{

}