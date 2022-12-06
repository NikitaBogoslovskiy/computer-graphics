#ifndef __BLEV_DATA_H__
#define __BLEV_DATA_H__

#include "Enums.h"
#include "Geometry.h"

namespace BLEV {
	struct Data {
		std::vector<Primitive*> primitives;
		std::set<Primitive*> chosen_prims;
		std::set<ImVec2*> chosen_prim_points;
		std::set<std::pair<ImVec2*, ImVec2*>> chosen_prim_edges;
		Primitive* new_prim;

		std::vector<Lsystem*> fractals;
		std::set<Lsystem*> chosen_lsys;

		ImVector<ImVec2*> intersections;

		std::vector<Mesh*> meshes;
		std::set<Mesh*> chosen_meshes;

		std::vector<FloatingHorizon*> horizons;
		std::set<FloatingHorizon*> chosen_horizons;

		Mode chosenMode;
		AddingLine adding_line;
		PrimEditMode chosenPrimEditMode;
		ViewMode chosenView;

		Primitive prev_displacement = Primitive(ImU32(1), 1);
		Primitive curr_displacement = Primitive(ImU32(1), 1);
		Line3d* rotate_axis;
		Torch* torch;

		static const uint8_t primEditModesSize = 5;
		const char* primEditModesList[primEditModesSize]{ "None", "Select points", "Move points", "Select edges", "Move edges" };

		static const uint8_t modesSize = 6;
		const char* modesList[modesSize]{ "Point", "Edge", "Polygon", "Bezier Curve", "Select", "Free Move" };

		const uint8_t classificationTypeSize = 3;
		const char* classificationType[3]{ "Point and Edge", "Point and Convex Polygon", "Point and Non-convex Polygon" };

		static const uint8_t viewModesSize = 4;
		const char* viewModes[viewModesSize]{ "Wireframe", "Flat Color", "Gouraud Shading", "CringeTracer" };

		const uint8_t cameraModesSize = 2;
		const char* cameraModes[2]{ "Perspective", "Axonometric" };

		const uint8_t funcs3dSize = 6;
		const char* funcs3dTypes[6]{ "5 * (cos(x^2 + z^2 + 1)/(x^2 + z^2 + 1) + 0.1)",
									"cos(x^2 + z^2)/(x^2 + z^2 + 1)",
									"sin(x) * cos(z)",
									"sin(x) + cos(z)",
									"x^2 + z^2",
									"sqrt(x*x/225 + y*y/64)" };
		Func3d meshGraphFuncs[6]{ &BLEVmath::ripples,
								  &BLEVmath::ripples2,
								  &BLEVmath::sinxMultCosz,
								  &BLEVmath::sinxPlusCosz,
								  &BLEVmath::squaresSum,
								  &BLEVmath::cone };
	};
}

#endif // !__BLEV_DATA_H__
