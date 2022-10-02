#ifndef _MAIN_H_
#define _MAIN_H_

#include <string>

enum class Mode {
	Point,
	Edge,
	Polygon,
	ConvexPolygon,
	NonConvexPolygon,
	None
};

struct CurrentState {
	Mode mode;
	int edgeAndPointOption = 0;
	std::string answer = "";
};

const unsigned char modesSize = 5;
const char* modesList[modesSize] { "Point", "Edge", "Polygon", "Convex Polygon", "Non-convex Polygon" };


#endif
