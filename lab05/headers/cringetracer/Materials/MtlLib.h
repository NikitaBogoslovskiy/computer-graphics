#pragma once
#include <set>
#include "Material.h"

class MtlLib
{
	//http://devernay.free.fr/cours/opengl/materials.html
	std::map<std::string, Material*> materials;

public:

	const int8_t mtlLibSize = 10;
	const char* mtlLibNames[10];

	MtlLib();
	~MtlLib();

	// god forgive me please someday ill get optionals
	bool Lookup(const char* name, Material*& outMtl);
	bool LookupByInd(const size_t mtlInd, Material*& outMtl);
	bool GetInd(const Material* outMtl, int& outInd);

private:

};
