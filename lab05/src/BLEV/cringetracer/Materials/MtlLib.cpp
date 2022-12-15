#include "../headers/cringetracer/Materials/MtlLib.h"

MtlLib::MtlLib()
{
	mtls = std::map<std::string, Material*>{
		{ "none"  , nullptr},
		{ "brick" , new Material()},

		{ "whiteMatte" , new Material(HVec<double> {1.0, 1.0, 1.0}, 0.1, 0.0)},

		{ "greenMatte" , new Material(HVec<double> { 0.0, 1.0, 0.0 }, 0.05, 0.0)},

		{ "orangeMatte" , new Material(HVec<double> { 1.0, 0.5, 0.0 }, 0.05, 0.0)},
		{ "orangeReflective" , new Material(HVec<double> { 1.0, 0.5, 0.0 }, 10.0, 0.75)},

		{ "blueMatte" , new Material(HVec<double> {0.125, 0.125, 1.0}, 0.05, 0.0)},
		{ "blueReflective" , new Material(HVec<double> {0.125, 0.125, 1.0}, 10.0, 0.75)},

		{ "redMatte" , new Material(HVec<double> {1.0, 0.5, 0.31}, 0.05, 0.0)},
		{ "redReflective" , new Material(HVec<double> {1.0, 0.5, 0.31}, 10.0, 0.75)},

		{ "lightBlueMatte" , new Material(HVec<double> {0.25, 0.3, 0.8}, 0.05, 0.0)},
		{ "lightBlueReflective" , new Material(HVec<double> {0.25, 0.3, 0.8}, 10.0, 0.8)},
		{ "skyBlueMatte" , new Material(HVec<double> {0.0, 0.74, 1.0}, 0.05, 0.0)},
		{ "skyBlueReflective" , new Material(HVec<double> {0.0, 0.74, 1.0}, 10.0, 0.8)},

		{ "mintMatte"  , new Material(HVec<double> { 0.24, 0.7, 0.53 }, 0.05, 0.0)},
		{ "mintReflective"  , new Material(HVec<double> { 0.24, 0.7, 0.53 }, 10.0, 0.5)},

		{ "mirror"  , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 5.0, 1.0)},
		{ "gold"  , new Material(HVec<double> { 0.75164, 0.70648, 0.22648 }, 10.0, 0.5)},
		{ "silver"  , new Material(HVec<double> { 0.5, 0.5, 0.8 }, 10.0, 0.5)},

		{ "glass" , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 32.0, 0.25, 1.0, 1.6)},
		{ "air" , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 0.0, 0.0, 1.0, 1.0)},
		{ "water" , new Material(HVec<double> { 0.25, 0.3, 0.8 }, 10.0, 0.8, 1.0, 1.333)},
		{ "emerald" , new Material(HVec<double> { 0.07568, 0.91424, 0.07568 }, 10.0, 0.8, 1.0, 1.333)},
	};

	for (auto it = mtls.begin(); it != mtls.end(); ++it) {
		mtlLibNames[std::distance(mtls.begin(), it)] = (*it).first.c_str();
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
	auto it = mtls.find(name);
	if (it == mtls.end()) return false;
	outMtl = (*it).second;
	return true;
}

bool MtlLib::GetInd(const Material* outMtl, int& outInd)
{
	for (auto it = mtls.begin(); it != mtls.end(); ++it)
		if (it->second == outMtl) {
			outInd = std::distance(mtls.begin(), it);
			return true;
		}
	return false;
}
