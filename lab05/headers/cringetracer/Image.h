#pragma once

#include "pch.h"
#include "../headers/geometry/methods/funcs.h"
#include "Ray.h"

class Image
{
	// Arrays to store image data.
	std::vector<std::vector<double>> rChannel;
	std::vector<std::vector<double>> gChannel;
	std::vector<std::vector<double>> bChannel;
	//std::vector<std::vector<HVec<double>>> intersections;
	//std::vector<std::vector<HVec<double>>> localNormals;
	//std::vector<std::vector<HVec<double>>> localColors;

	// And store the size of the image.
	size_t xSize, ySize;

public:
	std::vector<std::vector<Ray>> rays;
	Image();
	Image(const size_t& xSize, const size_t& ySize);
	~Image();
	void SetPixel(const size_t x, const size_t y, const double red, const double green, const double blue);
	void Draw(ImDrawList* dl, const ImVec2& canvasOffset);
	void Clear();
	void Resize(const size_t& _xSize, const size_t& _ySize);

	const size_t& XSize();
	const size_t& YSize();
};