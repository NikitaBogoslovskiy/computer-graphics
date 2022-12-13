#pragma once
#include <set>
#include "Material.h"

class MtlLib
{
	//struct mtlCmp {
	//	bool operator()(const Material*& x, const Material*& y) const { return x->Name() < y->Name(); }
	//};

	//std::set<Material*, mtlCmp>  materials{
	//	new Material(),
	//	new Material("lightBlueMatte", HVec<double> {0.25, 0.3, 0.8}, 5.0, 0.0),
	//	new Material("lightBlue", HVec<double> {0.25, 0.3, 0.8}, 10.0, 0.8),
	//	new Material("mint", HVec<double> { 110.0 / 255.0, 245.0 / 255.0, 128.0 / 255.0, }, 10.0, 0.5),
	//	new Material("mirror", HVec<double> { 1.0, 1.0, 1.0, }, 0.0, 1.0),
	//};

public:
	MtlLib();
	~MtlLib();

	// well sorry someday ill get optionals
	bool Lookup(const char* name, Material*& outMtl);
private:

};
