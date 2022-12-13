#include "../headers/cringetracer/Materials/MtlLib.h"

MtlLib::MtlLib()
{
	materials = std::map<std::string, Material*>{
		{ "none"  , nullptr},
		{ "brick" , new Material()},

		{ "lightBlue" , new Material(HVec<double> {0.25, 0.3, 0.8}, 10.0, 0.8)},
		{ "lightBlueMatte" , new Material(HVec<double> {0.25, 0.3, 0.8}, 5.0, 0.0)},

		{ "mint"  , new Material(HVec<double> { 0.24, 0.7, 0.53 }, 10.0, 0.5)},
		{ "mintMatte"  , new Material(HVec<double> { 0.24, 0.7, 0.53 }, 5.0, 0.0)},

		{ "mirror"  , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 5.0, 1.0)},
		{ "gold"  , new Material(HVec<double> { 0.75164, 0.70648, 0.22648 }, 10.0, 0.5)},
		{ "silver"  , new Material(HVec<double> { 0.5, 0.5, 0.8 }, 10.0, 0.5)},

		{ "glass" , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 32.0, 0.25, 0.75, 1.333)},
		{ "air" , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 0.0, 0.0, 1.0, 1.0)},
		{ "water" , new Material(HVec<double> { 0.25, 0.3, 0.8 }, 10.0, 0.4, 1.0, 1.333)},
	};

	for (auto it = materials.begin(); it != materials.end(); ++it) {
		mtlLibNames[std::distance(materials.begin(), it)] = (*it).first.c_str();
	}
}

MtlLib::~MtlLib()
{
}

bool MtlLib::LookupByInd(const size_t mtlInd, Material*& outMtl) {
	Material* newMtl;
	if (Lookup(mtlLibNames[mtlInd], newMtl)) {
		outMtl = newMtl;
		return true;
	}
	return false;
}

bool MtlLib::Lookup(const char* name, Material*& outMtl)
{
	auto it = materials.find(name);
	if (it == materials.end()) return false;
	outMtl = (*it).second;
	return true;
}

bool MtlLib::GetInd(const Material* outMtl, int& outInd)
{
	for (auto it = materials.begin(); it != materials.end(); ++it)
		if (it->second == outMtl) {
			outInd = std::distance(materials.begin(), it);
			return true;
		}
	return false;
}
