#define _USE_MATH_DEFINES
#include <math.h>
#include "../headers/cringetracer/Scene.h"
#include "../headers/cringetracer/GeometricBodies/Sphere.h"
#include "../headers/cringetracer/GeometricBodies/Ellipsoid.h"
#include "../headers/cringetracer/GeometricBodies/Plane.h"
#include "../headers/cringetracer/GeometricBodies/Cylinder.h"
#include "../headers/cringetracer/GeometricBodies/Cone.h"
#include "../headers/cringetracer/GeometricBodies/Box.h"
#include "../headers/cringetracer/Lights/PointLight.h"
#include "../headers/cringetracer/Materials/Material.h"

Scene::Scene()
{
	FillExampleScene1(this);
}

Scene::~Scene()
{

}

void Scene::FillExampleScene2(Scene* inScene)
{
	// cone 1
	auto cone = new Cone(HVec<double> {0.0, -1.0, -3.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f });
	Material* gold;
	if (materials.Lookup("gold", gold)) cone->SetMaterial(gold);
	//bodies.push_back(cone);

	// cone 2
	auto cone2 = new Cone(HVec<double> {2.0, -1.0, -3.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f });

	Material* lightBlue;
	if (materials.Lookup("lightBlue", lightBlue)) cone2->SetMaterial(lightBlue);
	//bodies.push_back(cone2);
	/*bodies.push_back(new Cylinder(HVec<double> {0.0, -0.5, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f }));*/

		// sph1
	auto sph = new Sphere(0.0, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* water;
	if (materials.Lookup("water", water)) sph->SetMaterial(water);
	//bodies.push_back(sph);

	// sph2
	auto sph2 = new Sphere(1.5, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* air;
	if (materials.Lookup("air", air)) sph2->SetMaterial(air);
	//bodies.push_back(sph2);

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
	inScene->bodies.push_back(plane);

	auto cube = new Box(HVec<double> {0.0, -0.5, 0.0},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> {0.5, 0.5, 0.5},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f });
	cube->SetMaterial(lightBlue);
	inScene->bodies.push_back(cube);

	inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 80.0 }, 10.0, HVec<double> { 1.0, 0.0, 0.0 }, 1.0));
	inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 100.0 }, 10.0, HVec<double> { 0.0, 0.0, 1.0 }, 1.0));
}

void Scene::FillExampleScene1(Scene* inScene)
{
	// ================ reflectives
	Material* lightBlueReflective;
	materials.Lookup("lightBlueReflective", lightBlueReflective);

	Material* gold;
	materials.Lookup("gold", gold);

	Material* silver;
	materials.Lookup("silver", silver);

	Material* mirror;
	materials.Lookup("mirror", mirror);

	// ================ transparency 
	Material* water;
	materials.Lookup("water", water);

	// ===================== mattes
	Material* mintMatte;
	materials.Lookup("mintMatte", mintMatte);

	Material* orangeMatte;
	materials.Lookup("orangeMatte", orangeMatte);

	Material* greenMatte;
	materials.Lookup("greenMatte", greenMatte);

	Material* redMatte;
	materials.Lookup("redMatte", redMatte);

	Material* whiteMatte;
	materials.Lookup("whiteMatte", whiteMatte);

	Material* lightBlueMatte;
	materials.Lookup("lightBlueMatte", lightBlueMatte);

	Material* blueMatte;
	materials.Lookup("blueMatte", blueMatte);

	const double OFFSET = 1.0;
	const double NINETY = 90.0;
	const double FLOOR_SIDE = 6.0;
	const double FLOOR_WIDTH = 2.2;
	const double WALL_HEIGHT = FLOOR_SIDE * 0.4;
	// floor
	auto floor = new Plane(HVec<double> {0.0, OFFSET, 0.0},
		HVec<double> { NINETY, 0.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, FLOOR_SIDE, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	floor->SetMaterial(lightBlueMatte);

	auto ceiling = new Plane(HVec<double> {0.0, -2.0 * WALL_HEIGHT + OFFSET, 0.0},
		HVec<double> { -NINETY, 0.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, FLOOR_SIDE, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	ceiling->SetMaterial(lightBlueMatte);

	auto backWall = new Plane(HVec<double> {0.0, -WALL_HEIGHT + OFFSET, -FLOOR_SIDE},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	backWall->SetMaterial(lightBlueReflective);

	auto frontWall = new Plane(HVec<double> {0.0, -WALL_HEIGHT + OFFSET, FLOOR_SIDE},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	frontWall->SetMaterial(redMatte);

	auto leftWall = new Plane(HVec<double> {-FLOOR_WIDTH, -WALL_HEIGHT + OFFSET, 0.0},
		HVec<double> { 0.0, NINETY, 0.0 },
		HVec<double> { FLOOR_SIDE, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	leftWall->SetMaterial(orangeMatte);

	auto rightWall = new Plane(HVec<double> { FLOOR_WIDTH, -WALL_HEIGHT + OFFSET, 0.0 },
		HVec<double> { 0.0, -NINETY, 0.0 },
		HVec<double> { FLOOR_SIDE, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	rightWall->SetMaterial(blueMatte);


	auto el1 = new Ellipsoid(-0.75, -0.75 * 0.5, -0.5, 0.5, 0.75, 0.5, ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f });
	el1->SetMaterial(mintMatte);
	auto sph = new Sphere(0.0, -0.5 + OFFSET, 0.0,
		0.5,
		ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	sph->SetMaterial(orangeMatte);
	auto cubeHD2 = 0.75;
	auto cube = new Box(HVec<double> {0.75, -cubeHD2 * 0.5 + OFFSET, -2.0},
		HVec<double> { 0.0, 30.0, 0.0 },
		HVec<double> {0.5, cubeHD2, 0.5},
		ImVec3{ 1.f, 1.f, 1.f });
	cube->SetMaterial(silver);

	inScene->bodies.push_back(backWall);
	inScene->bodies.push_back(frontWall);
	inScene->bodies.push_back(leftWall);
	inScene->bodies.push_back(rightWall);
	inScene->bodies.push_back(ceiling);
	inScene->bodies.push_back(floor);

	inScene->bodies.push_back(el1);
	inScene->bodies.push_back(sph);
	inScene->bodies.push_back(cube);
	inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.5, HVec<double> { 1.0, 1.0, 1.0 }, 1.0));
	//inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.5, HVec<double> { 1.0, 1.0, 1.0 }, 1.0));

	// const HVec<double>& inPosition, const HVec<double>& inColor, const double& inIntensity
	//inScene->lights.push_back(new PointLight(HVec<double> { 0.0, 2.0, 0.0 }, HVec<double> { 1.0, 0.0, 0.0 }, 1.0));
	//inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.0, HVec<double> { 0.0, 0.0, 1.0 }, 1.0));
}
