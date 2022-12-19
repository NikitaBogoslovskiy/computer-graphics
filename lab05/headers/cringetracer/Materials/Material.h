#pragma once
#include <vector>
#include "../Ray.h"
#include "../GeometricBodies/GeometricBody.h"
#include "../Lights/Light.h"
#include "../HeyraMath/HVec.h"

class Material {

protected:
	inline static double OFFSET = 0.01;

	bool CastRay(const Ray<double>& ray, const std::vector<GeometricBody*>& bodies,
		const GeometricBody* originBody, GeometricBody*& targetBody,
		HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor);

	inline static size_t MAX_REFLECTIONS = 3;

public:

	HVec<double> Ambient{ 1.0, 1.0, 1.0 };
	HVec<double> Diffuse{ 1.0, 1.0, 1.0 };
	HVec<double> Specular{ 1.0, 1.0, 1.0 };
	double Shininess = 0.5;

	double Reflectivity = 0.0;
	double Opacity = 1.0;
	double IOR = 1.0;
	inline double GetTransparency() { return 1.0 - Opacity; }

	Material();

	/*Material(const HVec<double>& color,
		const double shininess,
		const double reflectivity,
		const double transparency = 0.0, const double ior = 1.0);*/

	Material(const HVec<double>& ambient, const HVec<double>& diffuse, const HVec<double>& specular, const double shininess,
		const double reflectivity, const double opacity = 1.0, const double ior = 1.0);

	~Material();

	HVec<double> ComputeColor(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);

	HVec<double> ComputeAmbient(const HVec<double>& closestInt, const std::vector<Light*>& lights);

	static HVec<double> ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const GeometricBody* closestBody,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor);

	HVec<double> ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal);

	HVec<double> ComputeReflection(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& inRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);

	HVec<double> ComputeTransparency(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);
};