#include "../headers/cringetracer/Materials/MtlLib.h"

MtlLib::MtlLib()
{
	mtls = std::map<std::string, Material*>{
		{ "none"  , nullptr},
		{ "whiteMatte" , new Material()},

		/*{ "whiteMatte" , new Material(HVec<double> {1.0, 1.0, 1.0}, 0.1, 0.0)},

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
		{ "mintReflective"  , new Material(HVec<double> { 0.24, 0.7, 0.53 }, 10.0, 0.5)},*/
		//{ "mirror"  , new Material(HVec<double> { 1.0, 1.0, 1.0, }, 5.0, 1.0)},

		{ "lightBlueReflective" , new Material(
			HVec<double> { 0.25, 0.3, 0.8  },
			HVec<double> { 0.25, 0.3, 0.8 },
			HVec<double> { 0.25, 0.3, 0.8 },
			10.0, 0.8)},

		{ "Water" , new Material(
			HVec<double> { 0.1, 0.3, 0.2  },
			HVec<double> { 0.25, 0.3, 0.8 },
			HVec<double> { 0.792157, 0.741176, 0.807843 },
			10.0, 0.8, 0.0, 1.333)},

		{ "Water2" , new Material(
			HVec<double> { 0.2, 0.2, 0.2  },
			HVec<double> { 0.25, 0.3, 0.8 },
			HVec<double> { 1.0, 1.0, 1.0 },
			10.0, 0.8, 0.0, 1.333)},

		{ "Air" , new Material(
			HVec<double> {  0.0, 0.0, 0.0 },
			HVec<double> { 0.0, 0.0, 0.0 },
			HVec<double> {  0.0, 0.0, 0.0 },
			0.0, 0.0, 0.0, 1.0)},

		{ "Glass" , new Material(
			HVec<double> { 0.1, 0.1, 0.1 },
			HVec<double> { 1.0, 1.0, 1.0 },
			HVec<double> { 0.992157, 0.941176, 0.807843 },
			32.0, 0.25, 0.0, 1.6)},

		{ "OpalGreen" , new Material(
			HVec<double> { 0.1, 0.223529, 0.027451 },
			HVec<double> { 0.25, 0.3, 0.8 },
			HVec<double> { 0.992157, 0.941176, 0.807843 },
			10.0, 0.8, 0.0, 1.450)},

		{ "Brass" , new Material(
			HVec<double> { 0.329412, 0.223529, 0.027451 },
			HVec<double> { 0.780392, 0.568627, 0.113725 },
			HVec<double> { 0.992157, 0.941176, 0.807843 },
			27.8974, 0.6)},

		{ "Bronze" , new Material(
			HVec<double> { 0.2125, 0.1275,0.054 },
			HVec<double> { 0.714,0.4284,0.18144 },
			HVec<double> { 0.393548,0.271906,0.166721 },
			25.6, 0.6)},

		{ "BronzePolished" , new Material(
			HVec<double> { 0.25,0.148,0.06475},
			HVec<double> { 0.4,0.2368,0.1036 },
			HVec<double> { 0.774597,0.458561,0.200621 },
			76.8, 0.8)},

		{ "Chrome" , new Material(
			HVec<double> { 0.25, 0.25, 0.25},
			HVec<double> {0.4,0.4,0.4},
			HVec<double> { 0.774597,0.774597,0.774597 },
			76.8, 0.6)},

		{ "Copper" , new Material(
			HVec<double> { 0.19125,0.0735,0.0225},
			HVec<double> {0.7038,0.27048,0.0828},
			HVec<double> { 0.256777,0.137622,0.086014 },
			12.8, 0.6)},

		{ "CopperPolished" , new Material(
			HVec<double> { 0.2295,0.08825,0.0275},
			HVec<double> {0.5508,0.2118,0.066},
			HVec<double> { 0.580594,0.223257,0.0695701 },
			51.2, 0.8)},

		{ "Gold" , new Material(
			HVec<double> { 0.24725,0.1995,0.0745},
			HVec<double> {0.75164,0.60648,0.22648},
			HVec<double> { 0.628281,0.555802,0.366065 },
			51.2, 0.6)},

		{ "GoldPolished" , new Material(
		HVec<double> { 0.24725, 0.2245, 0.0645},
			HVec<double> {0.34615,0.3143,0.0903},
			HVec<double> { 0.797357,0.723991,0.208006 },
			83.2, 0.8) },

		{ "Pewter" , new Material(
			HVec<double> { 0.105882,0.058824,0.113725},
			HVec<double> {0.427451,0.470588,0.541176},
			HVec<double> { 0.333333,0.333333,0.521569 },
			9.84615, 0.5) },

		{ "Silver" , new Material(
			HVec<double> {0.19225,0.19225,0.19225},
			HVec<double> {0.50754,0.50754,0.50754},
			HVec<double> { 0.508273,0.508273,0.508273 },
			51.2, 0.6) },

		{ "SilverPolished" , new Material(
			HVec<double> {0.23125,0.23125,0.23125},
			HVec<double> {0.2775,0.2775,0.2775},
			HVec<double> { 0.773911,0.773911,0.773911 },
			89.6, 0.8) },

		{ "Emerald" , new Material(
			HVec<double> {0.0215,0.1745,0.0215},
			HVec<double> {0.07568,0.61424,0.07568},
			HVec<double> { 0.633,0.727811,0.633 },
			76.8, 0.5, 0.55, 1.560) },

		{ "Jade" , new Material(
			HVec<double> {0.135,0.2225,0.1575},
			HVec<double> {0.54, 0.89, 0.63},
			HVec<double> { 0.316228,0.316228,0.316228 },
			12.8, 0.3, 0.95, 1.640) },

		{ "Obsidian" , new Material(
			HVec<double> {0.05375, 0.05, 0.06625},
			HVec<double> {0.18275, 0.17, 0.22525},
			HVec<double> { 0.332741, 0.328634, 0.346435 },
			38.4, 0.2, 0.82, 1.486) },

		{ "Pearl" , new Material(
			HVec<double> {0.25,0.20725,0.20725},
			HVec<double> {1.0,0.829,0.829},
			HVec<double> { 0.296648,0.296648,0.296648 },
			11.264, 0.2, 0.922, 1.530) },

		{ "Ruby" , new Material(
			HVec<double> {0.1745,0.01175,0.01175},
			HVec<double> {0.61424,0.04136,0.04136},
			HVec<double> { 0.727811,0.626959,0.626959 },
			76.8, 0.3, 0.55, 1.757) },

		{ "Turquoise" , new Material(
			HVec<double> {0.1,0.18725,0.1745},
			HVec<double> {0.396,0.74151,0.69102},
			HVec<double> { 0.297254,0.30829,0.306678 },
			12.8, 0.4, 0.8, 1.610) },

		{ "PlasticWhite" , new Material(
			HVec<double> {0.0,	0.0,	0.0},
			HVec<double> {0.55,	0.55,	0.55},
			HVec<double> {0.70,	0.70,	0.70 },
			0.25 * 128, 0.1) },

		{ "PlasticYellow" , new Material(
			HVec<double> {0.0, 0.0,	0.0},
			HVec<double> {0.5, 0.5, 0.0},
			HVec<double> {0.60, 0.60, 0.50 },
			0.25 * 128, 0.1) },

		{ "PlasticCyan" , new Material(
			HVec<double> {0.0,	0.1,	0.06},
			HVec<double> {0.0,	0.50980392,	0.50980392},
			HVec<double> { 0.50196078,	0.50196078,	0.50196078 },
			0.25 * 128, 0.1) },

		{ "PlasticBlack" , new Material(
			HVec<double> {0.0,0.0,0.0},
			HVec<double> {0.01,0.01,0.01},
			HVec<double> { 0.50,0.50,0.50 },
			32, 0.1) },

		{ "PlasticGreen" , new Material(
			HVec<double> {0.0, 0.0, 0.0},
			HVec<double> {0.1, 0.35, 0.1},
			HVec<double> {0.45, 0.55, 0.45 },
			0.25 * 128, 0.1) },

		{ "PlasticRed" , new Material(
			HVec<double> {0.0,	0.0,	0.0},
			HVec<double> {0.5,	0.0,	0.0},
			HVec<double> { 0.7,	0.6,	0.6 },
			0.25 * 128, 0.1) },

		{ "RubberBlack" , new Material(
			HVec<double> {0.02,0.02,0.02},
			HVec<double> {0.01,0.01,0.01},
			HVec<double> { 0.4,0.4,0.4 },
			10, 0.0) },

		{ "RubberCyan" , new Material(
			HVec<double> {	0.0,	0.05,	0.05},
			HVec<double> {0.4,	0.5,	0.5},
			HVec<double> { 0.04,	0.7,	0.7 },
			128 * 0.078125, 0.0) },

		{ "RubberGreen" , new Material(
			HVec<double> {0.0,	0.05	,0.0},
			HVec<double> {0.4,	0.5,	0.4},
			HVec<double> { 0.04,	0.7,	0.04 },
			128 * 0.078125, 0.0) },

		{ "RubberRed" , new Material(
			HVec<double> {0.05, 0.0,	0.0},
			HVec<double> {0.5,	0.4,	0.4},
			HVec<double> { 0.7,	0.04,	0.04 },
			128 * 0.078125, 0.0) },

		{ "RubberWhite" , new Material(
			HVec<double> {0.05,	0.05,	0.05},
			HVec<double> {0.5,	0.5,	0.5},
			HVec<double> { 0.7,	0.7,	0.7	 },
			128 * 0.078125, 0.0) },

		{ "RubberYellow" , new Material(
			HVec<double> { 0.05,	0.05,	0.0},
			HVec<double> { 0.5,	0.5,	0.4},
			HVec<double> { 0.7,	0.7,	0.04 },
			128 * 0.078125, 0.0) },
	};

	//printf("%d\n", mtls.size());
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
