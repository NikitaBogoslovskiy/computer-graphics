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

		{ "brass" , new Material(
			HVec<double> { 0.329412, 0.223529, 0.027451 }, 1.0,
			HVec<double> { 0.780392, 0.568627, 0.113725 }, 1.0,
			HVec<double> { 0.992157, 0.941176, 0.807843 }, 1.0,
			27.8974)},

		{ "bronze" , new Material(
			HVec<double> { 0.2125,
0.1275,
0.054 }, 1.0,
			HVec<double> { 0.714,
0.4284,
0.18144 }, 1.0,
			HVec<double> { 0.393548,
0.271906,
0.166721 }, 1.0,
			25.6)},

		{ "polishedBronze" , new Material(
			HVec<double> { 0.25,
0.148,
0.06475}, 1.0,
			HVec<double> { 0.4,
0.2368,
0.1036 }, 1.0,
			HVec<double> { 0.774597,
0.458561,
0.200621 }, 1.0,
			76.8)},

		{ "chrome" , new Material(
			HVec<double> { 0.25,
0.25,
0.25}, 1.0,
			HVec<double> {0.4,
0.4,
0.4}, 1.0,
			HVec<double> { 0.774597,
0.774597,
0.774597 }, 1.0,
			76.8)},

		{ "copper" , new Material(
			HVec<double> { 0.19125,
0.0735,
0.0225}, 1.0,
			HVec<double> {0.7038,
0.27048,
0.0828}, 1.0,
			HVec<double> { 0.256777,
0.137622,
0.086014 }, 1.0,
			12.8)},

		{ "polishedCopper" , new Material(
			HVec<double> { 0.2295,
0.08825,
0.0275}, 1.0,
			HVec<double> {0.5508,
0.2118,
0.066}, 1.0,
			HVec<double> { 0.580594,
0.223257,
0.0695701 }, 1.0,
			51.2)},

		{ "Gold" , new Material(
			HVec<double> { 0.24725,
0.1995,
0.0745}, 1.0,
			HVec<double> {0.75164,
0.60648,
0.22648}, 1.0,
			HVec<double> { 0.628281,
0.555802,
0.366065 }, 1.0,
			51.2)},

		{ "polishedGold" , new Material(
		HVec<double> { 0.24725,
0.2245,
0.0645}, 1.0,
			HVec<double> {0.34615,
0.3143,
0.0903}, 1.0,
			HVec<double> { 0.797357,
0.723991,
0.208006 }, 1.0,
			83.2) },

		{ "pewter" , new Material(
	HVec<double> { 0.105882,
0.058824,
0.113725}, 1.0,
			HVec<double> {0.427451,
0.470588,
0.541176}, 1.0,
			HVec<double> { 0.333333,
0.333333,
0.521569 }, 1.0,
			9.84615) },

			{ "Silver" , new Material(
HVec<double> {0.19225,
0.19225,
0.19225}, 1.0,
			HVec<double> {0.50754,
0.50754,
0.50754}, 1.0,
			HVec<double> { 0.508273,
0.508273,
0.508273 }, 1.0,
			51.2) },

{ "Silver" , new Material(
HVec<double> {0.19225,
0.19225,
0.19225}, 1.0,
			HVec<double> {0.50754,
0.50754,
0.50754}, 1.0,
			HVec<double> { 0.508273,
0.508273,
0.508273 }, 1.0,
			51.2) },

{ "polishedSilver" , new Material(
HVec<double> {0.23125,
0.23125,
0.23125}, 1.0,
			HVec<double> {0.2775,
0.2775,
0.2775}, 1.0,
			HVec<double> { 0.773911,
0.773911,
0.773911 }, 1.0,
			89.6) },

{ "Emerald" , new Material(
HVec<double> {0.0215,
0.1745,
0.0215}, 0.55,
			HVec<double> {0.07568,
0.61424,
0.07568}, 0.55,
			HVec<double> { 0.633,
0.727811,
0.633 }, 0.55,
			76.8) },

{ "Jade" , new Material(
HVec<double> {0.135,
0.2225,
0.1575}, 0.95,
			HVec<double> {0.54,
0.89,
0.63}, 0.95,
			HVec<double> { 0.316228,
0.316228,
0.316228 }, 0.95,
			12.8) },

{ "Obsidian" , new Material(
HVec<double> {0.05375,
0.05,
0.06625}, 0.82,
			HVec<double> {0.18275,
0.17,
0.22525}, 0.82,
			HVec<double> { 0.332741,
0.328634,
0.346435 }, 0.82,
			38.4) },

{ "Pearl" , new Material(
HVec<double> {0.25,
0.20725,
0.20725}, 0.922,
			HVec<double> {1.0,
0.829,
0.829}, 0.922,
			HVec<double> { 0.296648,
0.296648,
0.296648 }, 0.922,
			11.264) },

{ "Ruby" , new Material(
HVec<double> {0.1745,
0.01175,
0.01175}, 0.55,
			HVec<double> {0.61424,
0.04136,
0.04136}, 0.55,
			HVec<double> { 0.727811,
0.626959,
0.626959 }, 0.55,
			76.8) },

{ "Turquoise" , new Material(
HVec<double> {0.1,
0.18725,
0.1745}, 0.8,
			HVec<double> {0.396,
0.74151,
0.69102}, 0.8,
			HVec<double> { 0.297254,
0.30829,
0.306678 }, 0.8,
			12.8) },

{ "BlackPlastic" , new Material(
HVec<double> {0.0,
0.0,
0.0}, 1.0,
			HVec<double> {0.01,
0.01,
0.01}, 1.0,
			HVec<double> { 0.50,
0.50,
0.50 }, 1.0,
			32) },

{ "BlackRubber" , new Material(
HVec<double> {0.02,
0.02,
0.02}, 1.0,
			HVec<double> {0.01,
0.01,
0.01}, 1.0,
			HVec<double> { 0.4,
0.4,
0.4 }, 1.0,
			10) },
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
