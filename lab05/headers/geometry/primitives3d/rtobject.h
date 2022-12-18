#pragma once

#include "pch.h"
#include "structers.h"


class RTEntity 
{ 
public:
	virtual bool getNearestIntersection(Ray& ray, float& mu, ImVec3& intersection_point, ImVec3& intersection_normal) { return false; }
};

class RTObject : public RTEntity
{
	float diffuse = 1.0;
	float transparency = 0.0;
	float reflection = 0.0;
	//float emission = 0.0;
	ImVec4 surfaceColor;
	//ImVec4 emissionColor;
public:
	RTObject() {}

	float& getDiffuse() { return diffuse; }
	float& getTrasparency() { return transparency; }
	float& getReflection() { return reflection; }
	//float& getEmission() { return emission; }
	ImVec4& getSurfaceColor() { return surfaceColor; }
	//ImVec4& getEmissionColor() { return emissionColor; }

	void setDiffuse(float& _diffuse) { diffuse = _diffuse; }
	void setTrasparency(float& _transparency) { transparency =_transparency; }
	void setReflection(float& _reflection) { reflection = _reflection; }
	//void setEmission(float& _emission) { emission = _emission; }
	void setSurfaceColor(const ImVec4& _color) { surfaceColor = _color; }
	//void setEmissionColor(ImVec4& _color) { emissionColor = _color; }
};
