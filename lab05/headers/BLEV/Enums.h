#pragma once
#ifndef __BLEV_ENUMS_H__

namespace BLEV {
	enum class PrimEditMode {
		None,
		SelectPoints,
		MovePoints,
		SelectEdges,
		MoveEdges
	};

	enum class Mode {
		Point,
		Edge,
		Polygon,
		BezierCurve,
		Select,
		FreeMove
	};
	
	enum AddingLine {
		None,
		FirstClick,
		ReleasedState,
		FinalClick
	};

	enum ViewMode {
		Wireframe,
		FlatColor,
		GouraudShading,
		RayTracing
	};
}

#define __BLEV_ENUMS_H__
#endif // !__BLEV_ENUMS_H__