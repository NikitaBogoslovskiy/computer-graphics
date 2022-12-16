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

public:

	inline static size_t MAX_REFLECTIONS = 3;

	inline static double ambientIntensity = 0.1;
	inline static HVec<double> ambientColor{ 1.0, 1.0, 1.0 };

	Material();
	Material(const HVec<double>& color,
		const double shininess,
		const double reflectivity,
		const double transparency = 0.0, const double ior = 1.0);
	Material(const HVec<double>& ambient, const double ambientIntensity,
		const HVec<double>& diffuse, const double diffuseIntensity,
		const HVec<double>& specular, const double reflectivity,
		const double shininess,
		const double transparency = 0.0, const double ior = 1.0);
	~Material();

	HVec<double> Ambient;
	double AmbientIntensity;

	HVec<double> Diffuse;
	double DiffuseIntensity;

	HVec<double> Specular;
	double Reflectivity;

	double Shininess;
	
	double Transparency;
	double IOR;

	HVec<double> ComputeColor(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);

	HVec<double> ComputeReflection(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& inRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);

	HVec<double> ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal);

	HVec<double> ComputeTransparency(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
		size_t REFLECTIONS_COUNT);

	static HVec<double> ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const GeometricBody* closestBody,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor);
};