#include "../headers/cringetracer/Materials/Material.h"

Material::Material()
{
	Color = HVec<double>{ 178.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0 };
	Shininess = 0.0;
	Reflectivity = 0.0;
}

Material::Material(HVec<double> color, const double shininess, const double reflectivity)
{
	Color = color;
	Shininess = shininess;
	Reflectivity = reflectivity;
}

Material::~Material()
{
}

HVec<double> Material::ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor)
{
	double outIntensity; HVec<double> diffuse(3), outColor(3);
	for (auto& light : lights) {
		if (!(light->Illuminate(closestInt, closestLocalNormal, closestBody, bodies, outColor, outIntensity))) continue;
		diffuse += outColor * outIntensity;
	}
	return diffuse * closestLocalColor;
}

HVec<double> Material::ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& cameraRay,
	const HVec<double>& closestInt, const HVec<double>& closestLocalNormal)
{
	if (Shininess <= 0.0) return HVec<double>(3);

	HVec<double> specular(3);
	for (auto& light : lights) {
		HVec<double> dirToLight = (light->position - closestInt).Normalized();
		HVec<double> pathStart = closestInt + dirToLight * 0.001; // !!! OFFSET TO AVOID BLACK DOTS&&&&??? WOOWOWO
		Ray<double> rayToLight(pathStart, pathStart + dirToLight);

		HVec<double> poi(3), poiNormal(3), poiColor(3); bool foundLightBlocker = false;
		for (auto& body : bodies) {
			if (foundLightBlocker = body->TestIntersection(rayToLight, poi, poiNormal, poiColor)) break;
		}
		if (foundLightBlocker) continue;

		HVec<double> d = rayToLight.direction;
		HVec<double> r = (d - 2 * HVec<double>::dot(d, closestLocalNormal) * closestLocalNormal).Normalized();
		HVec<double> v = cameraRay.direction; //v.Normalize();
		double cosineRV = HVec<double>::dot(r, v.Normalized());
		double intensity = cosineRV > 0.0 ? Reflectivity * std::pow(cosineRV, Shininess) : 0.0;
		specular += light->color * intensity;
	}

	return specular;
}

HVec<double> Material::ComputeColor(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& cameraRay,
	const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, 
	size_t REFLECTIONS_COUNT
) {
	HVec<double> diffuse = ComputeDiffuse(bodies, lights, closestBody, closestInt, closestLocalNormal, this->Color);
	HVec<double> reflection = ComputeReflection(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);
	HVec<double> specular = ComputeSpecular(bodies, lights, cameraRay, closestInt, closestLocalNormal);

	return Reflectivity * reflection + (1 - Reflectivity) * diffuse + specular;
}

bool Material::CastRay(const Ray<double>& ray, const std::vector<GeometricBody*>& bodies,
	const GeometricBody* originBody, GeometricBody*& targetBody,
	HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor)
{
	double minDist = 1e6;
	bool foundRayBlocker = false;
	HVec<double> poi(3), poiNormal(3), poiColor(3);
	for (auto& body : bodies) {
		if (body == originBody) continue;
		if (!(body->TestIntersection(ray, poi, poiNormal, poiColor))) continue;
		foundRayBlocker = true;
		double dist = (poi - ray.p1).len();
		if (dist < minDist) {
			minDist = dist;
			targetBody = body;
			closestInt = poi;
			closestLocalNormal = poiNormal;
			closestLocalColor = poiColor;
		}
	}
	return foundRayBlocker;
}

HVec<double> Material::ComputeReflection(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& inRay,
	const GeometricBody* originBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
	size_t REFLECTIONS_COUNT
) {
	HVec<double> reflection(3);
	if (Reflectivity <= 0.0) return reflection;

	HVec<double> d = inRay.direction;
	HVec<double> reflectionDir = d - 2 * HVec<double>::dot(d, closestLocalNormal) * closestLocalNormal;
	Ray<double> reflectionRay(closestInt, closestInt + reflectionDir); // 

	HVec<double> poi(3), poiNormal(3), poiColor(3); GeometricBody* targetBody;
	bool foundReflectionTarget = CastRay(reflectionRay, bodies, originBody, targetBody, poi, poiNormal, poiColor);

	if (foundReflectionTarget && (REFLECTIONS_COUNT++ < MAX_REFLECTIONS)) {
		if (targetBody->HasMaterial()) {
			reflection = targetBody->Mtl->ComputeColor(bodies, lights, reflectionRay, targetBody, poi, poiNormal, REFLECTIONS_COUNT);
		}
		else {
			reflection = Material::ComputeDiffuse(bodies, lights, targetBody, poi, poiNormal, targetBody->GetColor());
		}
	}

	return reflection;
}