#pragma once
#include <vector>
#include "../Ray.h"
#include "../GeometricBodies/GeometricBody.h"
#include "../Lights/Light.h"
#include "../HeyraMath/HVec.h"

class Material {

	bool CastRay(const Ray<double>& ray, const std::vector<GeometricBody*>& bodies, const GeometricBody* originBody,
		GeometricBody*& closestBody, HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor);

public:

	Material();
	Material(HVec<double> color, const double shininess, const double reflectivity);
	~Material();

	HVec<double> Color;
	double Shininess;
	double Reflectivity;

	HVec<double> ComputeColor(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal);

	HVec<double> ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const Ray<double>& cameraRay,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal);

	static HVec<double> ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
		const GeometricBody* closestBody,
		const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor);


};