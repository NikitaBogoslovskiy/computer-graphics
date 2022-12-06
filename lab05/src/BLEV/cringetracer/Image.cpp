#include "../headers/cringetracer/Image.h"

Image::Image()
{
	xSize = 0;
	ySize = 0;
}

Image::Image(const size_t& _xSize, const size_t& _ySize)
{
	Resize(xSize, ySize);
	Clear();
}

void Image::Resize(const size_t& _xSize, const size_t& _ySize)
{
	xSize = _xSize;
	ySize = _ySize;

	rChannel.resize(xSize);
	gChannel.resize(xSize);
	bChannel.resize(xSize);
	rays.resize(xSize);

	//intersections.resize(xSize);
	//localNormals.resize(xSize);
	//localColors.resize(xSize);

	for (size_t i = 0; i < xSize; ++i)
	{
		rChannel[i].resize(ySize, 0.0);
		gChannel[i].resize(ySize, 0.0);
		bChannel[i].resize(ySize, 0.0);
		rays[i].resize(ySize, Ray());
		//intersections[i].resize(ySize, Ray());
	}
}

void Image::Clear()
{
	for (size_t i = 0; i < xSize; ++i)
	{
		std::fill(rChannel[i].begin(), rChannel[i].end(), 0);
		std::fill(gChannel[i].begin(), gChannel[i].end(), 0);
		std::fill(bChannel[i].begin(), bChannel[i].end(), 0);
		std::fill(rays[i].begin(), rays[i].end(), Ray());
	}
}

void Image::Draw(ImDrawList* dl, const ImVec2& canvasOffset) {
	for (size_t x = 0; x < xSize; ++x)
	{
		for (size_t y = 0; y < ySize; ++y)
		{
			dl->AddRectFilled(
				ImVec2(x, y) + canvasOffset,
				ImVec2(x + 1, y + 1) + canvasOffset,
				IM_COL32(static_cast<ImU32>(rChannel.at(x).at(y)),
					static_cast<ImU32>(gChannel.at(x).at(y)),
					static_cast<ImU32>(bChannel.at(x).at(y)),
					255)
			);
		}
	}
}

void Image::SetPixel(const size_t x, const size_t y, const double red, const double green, const double blue)
{
	rChannel.at(x).at(y) = red;
	gChannel.at(x).at(y) = green;
	bChannel.at(x).at(y) = blue;
}

const size_t& Image::XSize()
{
	return xSize;
}

const size_t& Image::YSize()
{
	return ySize;
}

Image::~Image()
{
}