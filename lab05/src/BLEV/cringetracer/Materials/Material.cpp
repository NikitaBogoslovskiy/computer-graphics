#include "../headers/cringetracer/Materials/Material.h"
#include "../headers/cringetracer/GeometricBodies/Plane.h"
#include "../headers/cringetracer/GeometricBodies/Box.h"
Material::Material()
{
	Color = HVec<double>{ 178.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0 };
	Shininess = 0.0;
	Reflectivity = 0.0;
	Transparency = 0.0;
	IOR = 1.0;
}

Material::Material(const HVec<double>& color, const double shininess, const double reflectivity, const double transparency, const double ior)
{
	Color = color;
	Shininess = shininess;
	Reflectivity = reflectivity;
	Transparency = transparency;
	IOR = ior;
}

Material::~Material()
{
}

HVec<double> Material::ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor)
{
	double outIntensity; HVec<double> diffuse(3), outColor(3);
	bool illuminated = false;
	for (auto& light : lights) {
		if (!(light->ComputeLighting(closestInt, closestLocalNormal, closestBody, bodies, outColor, outIntensity))) continue;
		diffuse += outColor * outIntensity;
		illuminated = true;
	}
	return diffuse * closestLocalColor;
	//return illuminated ?
	//	diffuse * closestLocalColor
	//	:
	//	ambientColor * ambientIntensity * closestLocalColor //cache it
	//	;
}

HVec<double> Material::ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& cameraRay,
	const HVec<double>& closestInt, const HVec<double>& closestLocalNormal)
{
	if (Shininess <= 0.0) return HVec<double>(3);

	HVec<double> specular(3);
	for (auto& light : lights) {
		double intensity = 0.0;

		HVec<double> dirToLight = (light->position - closestInt);
		double lightDist = dirToLight.len();
		dirToLight.Normalize();

		HVec<double> start = closestInt + dirToLight * 0.001;
		Ray<double> rayToLight(start, start + dirToLight);

		HVec<double> poi(3), poiNormal(3), poiColor(3);
		bool foundLightBlocker = false;
		for (auto& body : bodies) {
			if (!(foundLightBlocker = body->TestIntersection(rayToLight, poi, poiNormal, poiColor))) continue;
			if ((poi - start).len() > lightDist) foundLightBlocker = false;
			if (foundLightBlocker) break;
		}
		if (!foundLightBlocker) {
			HVec<double> d = rayToLight.direction;
			HVec<double> r = (d - 2 * HVec<double>::dot(d, closestLocalNormal) * closestLocalNormal).Normalized();

			HVec<double> v = cameraRay.direction.Normalized();
			double cosineRV = HVec<double>::dot(r, v);

			if (cosineRV > 0.0)
			{
				intensity = Reflectivity * std::pow(cosineRV, Shininess);
			}
		}
		specular += light->color * intensity;
	}

	return specular;
}

HVec<double> Material::ComputeColor(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& cameraRay,
	const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
	size_t REFLECTIONS_COUNT
) {
	/*HVec<double> diffuse = ComputeDiffuse(bodies, lights, closestBody, closestInt, closestLocalNormal, this->Color);
	HVec<double> reflection = ComputeReflection(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);
	HVec<double> specular = ComputeSpecular(bodies, lights, cameraRay, closestInt, closestLocalNormal);

	return Reflectivity * reflection + (1 - Reflectivity) * diffuse + specular;*/

	HVec<double> diffuse = ComputeDiffuse(bodies, lights, closestBody, closestInt, closestLocalNormal, this->Color);
	HVec<double> reflection = ComputeReflection(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);
	HVec<double> specular = ComputeSpecular(bodies, lights, cameraRay, closestInt, closestLocalNormal);
	HVec<double> transparency = ComputeTransparency(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);

	return transparency * Transparency
		+ (Reflectivity * reflection + (1.0 - Reflectivity) * diffuse) * (1.0 - Transparency)
		+ specular
		+ ambientColor * ambientIntensity * this->Color;
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

HVec<double> Material::ComputeTransparency(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& inRay, const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, size_t REFLECTIONS_COUNT)
{
	if (Transparency <= 0.0) return HVec<double>(3);

	HVec<double> p1 = inRay.direction.Normalized();
	double r1 = 1.0 / IOR;
	HVec<double> n1 = closestLocalNormal;
	double cosine1 = -HVec<double>::dot(n1, p1);
	if (cosine1 < 0.0) {
		n1 *= -1.0;
		cosine1 = -HVec<double>::dot(n1, p1);
	}

	HVec<double> vRefr1 = r1 * p1 + (r1 * cosine1 - std::sqrt(1.0 - r1 * r1 * (1.0 - cosine1 * cosine1))) * n1; // REFRACTED DIRECTION

	// maybe set member property of offset (like surface thickness)? would be interesting
	Ray<double> rRefr(closestInt + vRefr1 * 0.01, closestInt + vRefr1); // THIS GOES INTO OBJECT AGAIN. 0.01 OFFSET FROM SURFACE TO AVOID PICKING UP SAME INTERSECTION TWICE

	// and then test if this ray crushed into obj again
	HVec<double> fPoi(3), fNormal(3), fColor(3),
		rPoi(3), rNormal(3), rColor(3);
	Ray<double> finalRay;
	if (closestBody->TestIntersection(rRefr, rPoi, rNormal, rColor)) { // checking intersection of ray rRefr with the same obj
		// but we wont implement total internal reflection. this is pbr thing i dont care
		// just assuming 
		HVec<double> p2 = rRefr.direction.Normalized();
		HVec<double> n2 = rNormal;
		double r2 = IOR;
		double cosine2 = -HVec<double>::dot(n2, p2);
		if (cosine2 < 0.0) {
			n2 *= -1.0;
			cosine2 = -HVec<double>::dot(n2, p2);
		}
		HVec<double> vRefr2 = r2 * p2 + (r2 * cosine2 - std::sqrt(1.0 - r2 * r2 * (1.0 - cosine2 * cosine2))) * n2;
		Ray<double> rRefr2(rPoi + vRefr2 * 0.01, rPoi + vRefr2);

		finalRay = rRefr2;
	}
	else finalRay = rRefr;

	GeometricBody* targetBody;
	if (!CastRay(finalRay, bodies, closestBody, targetBody, fPoi, fNormal, fColor)) return HVec<double>(3);

	return targetBody->HasMaterial() ?
		targetBody->Mtl->ComputeColor(bodies, lights, finalRay, targetBody, fPoi, fNormal, REFLECTIONS_COUNT)
		:
		Material::ComputeDiffuse(bodies, lights, targetBody, fPoi, fNormal, targetBody->GetColor());
}