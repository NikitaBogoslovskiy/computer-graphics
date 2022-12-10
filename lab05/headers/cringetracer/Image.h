#pragma once

#include "pch.h"
#include "../headers/geometry/methods/funcs.h"
#include "Ray.h"

class Image
{
	std::vector<std::vector<double>> rChannel;
	std::vector<std::vector<double>> gChannel;
	std::vector<std::vector<double>> bChannel;
	size_t xSize, ySize;

	double _maxRed = 0.0;
	double _maxGreen = 0.0;
	double _maxBlue = 0.0;
	double _Max = 0.0;
	void CalcMaxes();

public:
	std::vector<std::vector<Ray<double>>> rays;
	Image();
	Image(const size_t& xSize, const size_t& ySize);
	~Image();
	void SetPixel(const size_t x, const size_t y, const HVec<double>& color);
	void Draw(ImDrawList* dl, const ImVec2& canvasOffset);
	void Clear();
	void Resize(const size_t& _xSize, const size_t& _ySize);

	const size_t& XSize();
	const size_t& YSize();
};