#include "../headers/cringetracer/Materials/Material.h"
#include "../headers/cringetracer/GeometricBodies/Plane.h"
#include "../headers/cringetracer/GeometricBodies/Box.h"
#include "../headers/cringetracer/GeometricBodies/LightSphere.h"

Material::Material()
{
	Diffuse = HVec<double>{ 178.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0 };
	Shininess = 0.0;
	Reflectivity = 0.0;
	Transparency = 0.0;
	IOR = 1.0;
}

Material::Material(const HVec<double>& color, const double shininess, const double reflectivity, const double transparency, const double ior)
{
	Diffuse = color;
	Shininess = shininess;
	Reflectivity = reflectivity;
	Transparency = transparency;
	IOR = ior;
}

Material::Material(const HVec<double>& ambient, const double ambientIntensity,
	const HVec<double>& diffuse, const double diffuseIntensity,
	const HVec<double>& specular, const double reflectivity,
	const double shininess,
	const double transparency, const double ior) : 
	Ambient(ambient), AmbientIntensity(ambientIntensity),
	Diffuse(diffuse), DiffuseIntensity(diffuseIntensity),
	Specular(specular), Reflectivity(reflectivity),
	Shininess(shininess),
	Transparency(transparency), IOR(ior)
{
	/*Diffuse = color;
	Shininess = shininess;
	Reflectivity = reflectivity;
	Transparency = transparency;
	IOR = ior;*/
}

Material::~Material()
{
}

HVec<double> Material::ComputeDiffuse(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const GeometricBody* closestBody,
	const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, const HVec<double>& closestLocalColor)
{
	double outIntensity;
	HVec<double> diffuse, outColor;
	bool illuminated = false;
	for (auto& light : lights) {
		if (!light->Show) continue;
		if (!(light->ComputeLighting(closestInt, closestLocalNormal, closestBody, bodies, outColor, outIntensity))) continue;
		diffuse += outColor * outIntensity;
		illuminated = true;
	}
	return diffuse * closestLocalColor;
	/*if (illuminated) {
		return diffuse * closestLocalColor;
	}
	else {

	}*/

	//return illuminated ?
	//diffuse * closestLocalColor
	//:
	//ambientColor * ambientIntensity * closestLocalColor //cache it
	//;
}

HVec<double> Material::ComputeSpecular(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& cameraRay,
	const HVec<double>& closestInt, const HVec<double>& closestLocalNormal)
{
	if (Shininess <= 0.0) return HVec<double>();

	HVec<double> specular;
	for (auto& light : lights) {
		if (!light->Show) continue;
		double intensity = 0.0;

		HVec<double> dirToLight = (light->position - closestInt);
		double lightDist = dirToLight.len();
		dirToLight.Normalize();

		HVec<double> start = closestInt + dirToLight * 0.001;
		Ray<double> rayToLight(start, start + dirToLight);

		HVec<double> poi, poiNormal, poiColor;
		bool foundLightBlocker = false;
		for (auto& body : bodies) {
			if (!body->Show) continue;

			if (dynamic_cast<LightSphere*>(body) != nullptr) continue; // 1) light source body does not cast shadow
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
		specular += light->intensity * light->color * intensity;
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

	HVec<double> diffuse = ComputeDiffuse(bodies, lights, closestBody, closestInt, closestLocalNormal, this->Diffuse);
	HVec<double> reflection = ComputeReflection(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);
	HVec<double> specular = ComputeSpecular(bodies, lights, cameraRay, closestInt, closestLocalNormal);
	HVec<double> transparency = ComputeTransparency(bodies, lights, cameraRay, closestBody, closestInt, closestLocalNormal, REFLECTIONS_COUNT);

	return transparency * Transparency
		+ (Reflectivity * reflection + (1.0 - Reflectivity) * diffuse) * (1.0 - Transparency)
		+ specular
		+ /*ambientColor **/ ambientIntensity * this->Diffuse;// should try different ambient colors for materials
}

bool Material::CastRay(const Ray<double>& ray,
	const std::vector<GeometricBody*>& bodies, const GeometricBody* originBody,
	GeometricBody*& targetBody, HVec<double>& closestInt, HVec<double>& closestLocalNormal, HVec<double>& closestLocalColor)
{
	double minDist = std::numeric_limits<double>::max();
	targetBody = nullptr;
	HVec<double> poi, poiNormal, poiColor;
	for (auto& body : bodies) {
		if (body == originBody) continue;
		if (!body->Show) continue;
		if (!(body->TestIntersection(ray, poi, poiNormal, poiColor))) continue;

		double dist = (poi - ray.p1).len();
		if (dist >= minDist) continue;
		minDist = dist;
		targetBody = body;
		closestInt = poi;
		closestLocalNormal = poiNormal;
		closestLocalColor = poiColor;
	}
	return targetBody != nullptr;
}

HVec<double> Material::ComputeReflection(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& inRay,
	const GeometricBody* originBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal,
	size_t REFLECTIONS_COUNT
) {
	HVec<double> reflection;
	if (Reflectivity <= 0.0) return reflection;

	HVec<double> d = inRay.direction;
	HVec<double> rDir = d - 2 * HVec<double>::dot(d, closestLocalNormal) * closestLocalNormal;

	HVec<double> startPoint = closestInt + closestLocalNormal * OFFSET;
	Ray<double> rRay(startPoint, startPoint + rDir); // 

	HVec<double> poi, poiNormal, poiColor;
	GeometricBody* targetBody = nullptr;
	bool foundReflectionTarget = CastRay(rRay, bodies, originBody, targetBody, poi, poiNormal, poiColor);

	if (foundReflectionTarget && (REFLECTIONS_COUNT++ < MAX_REFLECTIONS)) {
		if (targetBody->HasMaterial()) {
			reflection = targetBody->Mtl->ComputeColor(bodies, lights, rRay, targetBody, poi, poiNormal, REFLECTIONS_COUNT);
		}
		else if (dynamic_cast<LightSphere*>(targetBody) == nullptr) {
			reflection = Material::ComputeDiffuse(bodies, lights, targetBody, poi, poiNormal, targetBody->GetColor());
		}
		//else { // 3) LIGHT_SOURCE is reflected
			//reflection = targetBody->GetColor();
		//}
	}

	return reflection;
}

HVec<double> Material::ComputeTransparency(const std::vector<GeometricBody*>& bodies, const std::vector<Light*>& lights,
	const Ray<double>& inRay, const GeometricBody* closestBody, const HVec<double>& closestInt, const HVec<double>& closestLocalNormal, size_t REFLECTIONS_COUNT)
{
	if (Transparency <= 0.0) return HVec<double>();

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
	Ray<double> rRefr(closestInt + vRefr1 * OFFSET, closestInt + vRefr1); // THIS GOES INTO OBJECT AGAIN. OFFSET FROM SURFACE TO AVOID PICKING UP SAME INTERSECTION TWICE

	// and then test if this ray crushed into obj again
	HVec<double> fPoi, fNormal, fColor,
		rPoi, rNormal, rColor;
	Ray<double> finalRay;
	if (closestBody->TestIntersection(rRefr, rPoi, rNormal, rColor)) { // checking intersection of ray rRefr with the same obj
		HVec<double> p2 = rRefr.direction.Normalized();
		HVec<double> n2 = rNormal;
		double r2 = IOR;
		double cosine2 = -HVec<double>::dot(n2, p2);
		if (cosine2 < 0.0) {
			n2 *= -1.0;
			cosine2 = -HVec<double>::dot(n2, p2);
		}
		HVec<double> vRefr2 = r2 * p2 + (r2 * cosine2 - std::sqrt(1.0 - r2 * r2 * (1.0 - cosine2 * cosine2))) * n2;
		Ray<double> rRefr2(rPoi + vRefr2 * OFFSET, rPoi + vRefr2);

		finalRay = rRefr2;
	}
	else finalRay = rRefr;

	GeometricBody* targetBody = nullptr;
	if (!CastRay(finalRay, bodies, closestBody, targetBody, fPoi, fNormal, fColor)) return HVec<double>();

	//return targetBody->HasMaterial() ? // 4) LIGHT_SOURCE is reflected in transparent objects
	//	targetBody->Mtl->ComputeColor(bodies, lights, finalRay, targetBody, fPoi, fNormal, REFLECTIONS_COUNT)
	//	: (dynamic_cast<LightSphere*>(targetBody) == nullptr) ?
	//	Material::ComputeDiffuse(bodies, lights, targetBody, fPoi, fNormal, targetBody->GetColor())
	//	: targetBody->GetColor();
	//;
	return targetBody->HasMaterial() ? // 4) LIGHT_SOURCE is reflected in transparent objects
		targetBody->Mtl->ComputeColor(bodies, lights, finalRay, targetBody, fPoi, fNormal, REFLECTIONS_COUNT)
		:
		Material::ComputeDiffuse(bodies, lights, targetBody, fPoi, fNormal, targetBody->GetColor());
	;
}