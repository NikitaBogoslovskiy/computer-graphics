#ifndef _MAIN_H_
#define _MAIN_H_

#include <string>

enum class Mode {
	EdgeAndPoint,
	ConvexPolygon,
	NonConvexPolygon
};

struct CurrentState {
	Mode mode;
	int edgeAndPointOption = 0;
	std::string answer = "";
};

const unsigned char modesSize = 3;
const char* modesList[modesSize] { "Edge and Point", "Convex Polygon", "Non-convex Polygon" };


#endif
