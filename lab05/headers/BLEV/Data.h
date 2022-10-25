#pragma once
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

		Mode chosenMode;
		AddingLine adding_line;
		PrimEditMode chosenPrimEditMode;

		Primitive prev_displacement = Primitive(ImU32(1), 1);
		Primitive curr_displacement = Primitive(ImU32(1), 1);
		Line3d* rotate_axis;

		static const uint8_t primEditModesSize = 5;
		const char* primEditModesList[primEditModesSize]{ "None", "Select points", "Move points", "Select edges", "Move edges" };

		static const uint8_t modesSize = 6;
		const char* modesList[modesSize]{ "Point", "Edge", "Polygon", "Bezier Curve", "Select", "Free Move" };

		const uint8_t classificationTypeSize = 3;
		const char* classificationType[3]{ "Point and Edge", "Point and Convex Polygon", "Point and Non-convex Polygon" };

		const uint8_t cameraModesSize = 2;
		const char* cameraModes[2]{ "Perspective", "Axonometric" };
	};
}

#endif // !__BLEV_DATA_H__