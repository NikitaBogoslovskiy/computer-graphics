#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/Lights/PointLight.h"

Scene::Scene()
{
	bodies.push_back(new Sphere(-1.5, -1.5, 0.0, 0.5, ImVec3{ 255.f / 255.f, 215.f / 255.f, 0.f / 255.f }));
	bodies.push_back(new Sphere(0.0, -1.5, 0.0, 0.5, ImVec3{ 46.f / 255.f, 139.f / 255.f, 87.f / 255.f }));
	bodies.push_back(new Sphere(1.5, -1.5, 0.0, 0.5, ImVec3{ 0.f / 255.f, 255.f / 255.f, 255.f / 255.f }));

	bodies.push_back(new Sphere(-1.5, 0.0, 0.0, 0.5, ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f }));
	bodies.push_back(new Sphere(0.0, 0.0, 0.0, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f }));
	bodies.push_back(new Sphere(1.5, 0.0, 0.0, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f }));

	bodies.push_back(new Sphere(-1.5, 1.5, 0.0, 0.5, ImVec3{ 140.f / 255.f, 30.f / 255.f, 56.f / 255.f }));
	bodies.push_back(new Sphere(0.0, 1.5, 0.0, 0.5, ImVec3{ 173.f / 255.f, 255.f / 255.f, 47.f / 255.f }));
	bodies.push_back(new Sphere(1.5, 1.5, 0.0, 0.5, ImVec3{ 0.f / 255.f, 139.f / 255.f, 139.f / 255.f }));

	lights.push_back(new PointLight(HVec<double> {5.0, -5.0, 10.0}, HVec<double> { 255.0, 255.0, 255.0 }, 1.0));
}

Scene::~Scene()
{

}