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
#include <memory>

Scene::Scene()
{
	materials = std::map<std::string, Material*> { // please move it so it become accessible to the interface
			{"brick", new Material()},
			{"lightBlueMatte", new Material(HVec<double> {0.25, 0.3, 0.8}, 5.0, 0.0)},
			{"lightBlue", new Material(HVec<double> {0.25, 0.3, 0.8}, 10.0, 0.8)},
			{"mint", new Material(HVec<double> { 110.0 / 255.0, 245.0 / 255.0, 128.0 / 255.0, }, 10.0, 0.5)},
			{"mirror", new Material(HVec<double> { 1.0, 1.0, 1.0, }, 0.0, 1.0)}
	};

	/*bodies.push_back(new Cone(HVec<double> {0.0, -0.5, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f }));*/

		/*bodies.push_back(new Cylinder(HVec<double> {0.0, -0.5, 0.0},
			HVec<double> { M_PI * 0.5, 0.0, 0.0 },
			HVec<double> {1.0, 1.0, 1.0},
			ImVec3{ 0.4f, 0.6f, 0.8f }));*/
	auto el1 = new Ellipsoid(-1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f });
	el1->SetMaterial(materials["lightBlue"]);

	auto sph = new Sphere(0.0, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	sph->SetMaterial(materials["lightBlue"]);

	auto el2 = new Ellipsoid(1.5, 0.0, -0.5, 0.5, 0.75, 0.5, ImVec3{ 0.4f, 0.6f, 0.8f });
	el2->SetMaterial(materials["mint"]);

	auto plane = new Plane(HVec<double> {0.0, 0.75, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {10.0, 10.0, 10.0},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f });
	plane->SetMaterial(materials["lightBlue"]);

	bodies.push_back(el1); bodies.push_back(sph); bodies.push_back(el2);  bodies.push_back(plane);

	//lights.push_back(new PointLight(HVec<double> {5.0, -5.0, 10.0}, HVec<double> { 159.f / 255.f, 44.f / 255.f, 214.f / 255.f }, 1.0));
	//

	//lights.push_back(new PointLight(HVec<double> {-5.0, -10.0, 5.0}, HVec<double> { 1.0, 1.0, 1.0 }, 1.0));
	lights.push_back(new PointLight(HVec<double> {-5.0, -10.0, 5.0}, HVec<double> { 1.0, 0.0, 0.0 }, 1.0));
	//lights.push_back(new PointLight(HVec<double> {0.0, -10.0, -5.0}, HVec<double> { 0.0, 1.0, 0.0 }, 1.0));
	lights.push_back(new PointLight(HVec<double> {5.0, -10.0, 5.0}, HVec<double> { 0.0, 0.0, 1.0 }, 1.0));
}

Scene::~Scene()
{

}