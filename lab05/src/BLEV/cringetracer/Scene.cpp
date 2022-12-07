#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/Lights/PointLight.h"

Scene::Scene()
{
	bodies.push_back(new Sphere(0.0, 0.0, 0.0, 1.0, ImVec3{ 0.4f, 0.6f, 0.8f }));
	lights.push_back(new PointLight(HVec<double> {5.0, -5.0, -5.0}, HVec<double> { 255.0, 255.0, 255.0 }, 1.0));
}

Scene::~Scene()
{

}