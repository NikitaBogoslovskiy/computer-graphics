#pragma once

//#include "pch.h"
#include "structers.h"

class Material
{
public:
	ImVec4 ambient = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
	ImVec4 diffuse = { 0.07568f, 0.61424f, 0.07568f, 1.0f };
	ImVec4 specular = { 0.633f, 0.727811f, 0.633f, 1.0f };
	float shininess = 76.8f;
	float reflection = 0.5f;
	float refraction = 0.9f;
	float ior = 1.560f;
	Material() {}
	Material(ImVec4& _ambient, ImVec4& _diffuse, ImVec4& _specular, float _shininess, float _reflection, float _refraction, float _ior)
	{
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
		shininess = _shininess;
		reflection = _reflection;
		refraction = _refraction;
		ior = _ior;
	}
};

class RTEntity 
{ 
public:
	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) { return false; }
};

class RTObject : public RTEntity
{
public:
	Material* material;
	RTObject() {
		material = new Material();
	}

	//float& getDiffuse() { return diffuse; }
	//float& getTrasparency() { return transparency; }
	//float& getReflection() { return reflection; }
	//float& getShininess() { return shininess; }
	////float& getEmission() { return emission; }
	//ImVec4& getSurfaceColor() { return surfaceColor; }
	////ImVec4& getEmissionColor() { return emissionColor; }

	//void setDiffuse(const float& _diffuse) { diffuse = _diffuse; }
	//void setTrasparency(const float& _transparency) { transparency =_transparency; }
	//void setReflection(const float& _reflection) { reflection = _reflection; }
	//void setShininess(const float& _shininess) { shininess = _shininess; }
	////void setEmission(float& _emission) { emission = _emission; }
	//void setSurfaceColor(const ImVec4& _color) { surfaceColor = _color; }
	////void setEmissionColor(ImVec4& _color) { emissionColor = _color; }
};
