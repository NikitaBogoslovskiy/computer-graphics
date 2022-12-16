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
	//FillExampleScene2(this);
	//FillExampleScene1(this);

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
	if (materials.Lookup("Gold", gold)) cone->SetMaterial(gold);
	//bodies.push_back(cone);

	// cone 2
	auto cone2 = new Cylinder(HVec<double> {2.0, -1.0, -3.0},
		HVec<double> { 90.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f });

	Material* lightBlue;
	if (materials.Lookup("PlasticCyan", lightBlue)) cone2->SetMaterial(lightBlue);
	//bodies.push_back(cone2);
	/*bodies.push_back(new Cylinder(HVec<double> {0.0, -0.5, 0.0},
		HVec<double> { M_PI * 0.5, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 0.4f, 0.6f, 0.8f }));*/

		// sph1
	auto sph = new Sphere(0.0, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* water;
	if (materials.Lookup("Water", water)) sph->SetMaterial(water);


	// sph2
	auto sph2 = new Sphere(1.5, 0.0, 0.5, 0.5, ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	Material* air;
	if (materials.Lookup("Silver", air)) sph2->SetMaterial(air);
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

	auto cube = new Box(HVec<double> {0.0, -2.0, -3.0},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> {1.0, 1.0, 1.0},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f });
	cube->SetMaterial(lightBlue);

	inScene->bodies.push_back(sph);
	inScene->bodies.push_back(cube);
	inScene->bodies.push_back(cone);
	inScene->bodies.push_back(el1);
	inScene->bodies.push_back(el2);

	auto pl1 = new PointLight(HVec2<double>{ -45.0, 165.0 }, 10.0, HVec<double> { 1.0, 1.0, 1.0 }, 1.0);
	auto pl2 = new PointLight(HVec2<double>{ -45.0, 100.0 }, 10.0, HVec<double> { 0.0, 0.0, 1.0 }, 1.0);

	inScene->lights.push_back(pl1);
	//inScene->bodies.push_back(pl1->LightSource);

	//inScene->lights.push_back(pl2);
	//inScene->bodies.push_back(pl2->LightSource);
}

void Scene::FillExampleScene1(Scene* inScene)
{
	// ================ reflectives
	Material* lightBlueReflective,
		* gold,
		* silver,
		* water,
		* mirror,
		* mintMatte,
		* orangeMatte,
		* greenMatte,
		* redMatte,
		* whiteMatte,
		* lightBlueMatte,
		* blueMatte,
		* emerald,
		* ruby,
		* pearl
		;

	//materials.Lookup("lightBlueReflective", lightBlueReflective);
	materials.Lookup("Gold", gold);
	materials.Lookup("Silver", silver);

	//// ================ transparency 
	materials.Lookup("Water", water);
	materials.Lookup("Emerald", emerald);
	materials.Lookup("Ruby", ruby);
	materials.Lookup("Pearl", pearl);
	//// ===================== mattes
	//materials.Lookup("mintMatte", mintMatte);
	//materials.Lookup("orangeMatte", orangeMatte);
	//materials.Lookup("greenMatte", greenMatte);
	materials.Lookup("RubberRed", redMatte);
	materials.Lookup("RubberWhite", whiteMatte);
	materials.Lookup("RubberCyan", lightBlueMatte);
	materials.Lookup("RubberGreen", mintMatte);

	constexpr double OFFSET = 4.0;
	constexpr double NINETY = 90.0;
	constexpr double FLOOR_SIDE = 12.0;
	constexpr double FLOOR_WIDTH = FLOOR_SIDE;
	constexpr double WALL_HEIGHT = FLOOR_SIDE * 0.5;

	//
	auto backWall = new Plane(HVec<double> {0.0, -WALL_HEIGHT + OFFSET, -FLOOR_SIDE},
		HVec<double> { 0.0, NINETY * 2.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	backWall->SetMaterial(silver);

	auto frontWall = new Plane(HVec<double> {0.0, -WALL_HEIGHT + OFFSET, FLOOR_SIDE},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> { FLOOR_WIDTH, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	frontWall->SetMaterial(redMatte);

	//
	auto leftWall = new Plane(HVec<double> {-FLOOR_WIDTH, -WALL_HEIGHT + OFFSET, 0.0},
		HVec<double> { 0.0, NINETY, 0.0 },
		HVec<double> { FLOOR_SIDE, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	leftWall->SetMaterial(lightBlueMatte);

	auto rightWall = new Plane(HVec<double> { FLOOR_WIDTH, -WALL_HEIGHT + OFFSET, 0.0 },
		HVec<double> { 0.0, -NINETY, 0.0 },
		HVec<double> { FLOOR_SIDE, WALL_HEIGHT, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	rightWall->SetMaterial(mintMatte);

	//
	auto ceiling = new Plane(HVec<double> {0.0, -2.0 * WALL_HEIGHT + OFFSET, 0.0},
		HVec<double> { -NINETY, 0.0, 0.0 },
		HVec<double> { FLOOR_SIDE, FLOOR_SIDE, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	ceiling->SetMaterial(whiteMatte);

	auto floor = new Plane(HVec<double> {0.0, OFFSET, 0.0},
		HVec<double> { NINETY, 0.0, 0.0 },
		HVec<double> { FLOOR_SIDE, FLOOR_SIDE, 1.0 },
		ImVec3{ 1.f, 1.f, 1.f });
	floor->SetMaterial(whiteMatte);

	// =============================================================

	auto el1 = new Ellipsoid(-0.75, -0.5, -0.5,
		0.5, 0.75, 0.5,
		ImVec3{ 255.f / 255.f, 127.f / 255.f, 80.f / 255.f });
	el1->SetMaterial(pearl);

	auto sph = new Sphere(0.0, 0.5, 0.0,
		0.5,
		ImVec3{ 152.f / 255.f, 251.f / 255.f, 152.f / 255.f });
	sph->SetMaterial(ruby);

	auto cube = new Box(HVec<double> {0.75, 0.0, -2.0},
		HVec<double> { 0.0, 20.0, 0.0 },
		HVec<double> {0.5, 1.0, 0.5},
		ImVec3{ 1.f, 1.f, 1.f });
	cube->SetMaterial(emerald);

	auto cube2 = new Box(HVec<double> {0.0, 2.0, -0.5},
		HVec<double> { 0.0, 0.0, 0.0 },
		HVec<double> {2.0, 1.0, 2.5},
		ImVec3{ 100.0f / 255.f, 100.0f / 255.f, 100.0f / 255.f });
	cube2->SetMaterial(silver);

	inScene->bodies.push_back(backWall);
	inScene->bodies.push_back(frontWall);
	inScene->bodies.push_back(leftWall);
	inScene->bodies.push_back(rightWall);
	inScene->bodies.push_back(ceiling);
	inScene->bodies.push_back(floor);

	inScene->bodies.push_back(el1);
	inScene->bodies.push_back(sph);
	inScene->bodies.push_back(cube);
	inScene->bodies.push_back(cube2);
	//inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.5, HVec<double> { 1.0, 1.0, 1.0 }, 1.0));
	//inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.5, HVec<double> { 1.0, 1.0, 1.0 }, 1.0));

	// const HVec<double>& inPosition, const HVec<double>& inColor, const double& inIntensity
	//inScene->lights.push_back(new PointLight(HVec<double> { 0.0, 2.0, 0.0 }, HVec<double> { 1.0, 0.0, 0.0 }, 1.0));
	//inScene->lights.push_back(new PointLight(HVec<double> { -45.0, 90.0 }, 4.0, HVec<double> { 0.0, 0.0, 1.0 }, 1.0));

	auto light1 = new PointLight(HVec2<double> { -45.0, 70.0 }, 4.0, HVec<double> { 1.0, 1.0, 1.0 }, 1.0);
	//auto light2 = new PointLight(HVec2<double> { -45.0, 120.0 }, 4.5, HVec<double> { 0.0, 0.0, 1.0 }, 1.0);
	//inScene->bodies.push_back(light1->LightSource);
	inScene->lights.push_back(light1);

	//inScene->bodies.push_back(light2->LightSource);
	//inScene->lights.push_back(light2);
}
