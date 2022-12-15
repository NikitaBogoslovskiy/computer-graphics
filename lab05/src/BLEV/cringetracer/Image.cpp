#include "../headers/cringetracer/Image.h"

Image::Image()
{
	xSize = 0;
	ySize = 0;
}

Image::Image(const size_t& _xSize, const size_t& _ySize)
{
	Resize(xSize, ySize);
	//Clear();
}

void Image::Resize(const size_t& _xSize, const size_t& _ySize)
{
	xSize = _xSize;
	ySize = _ySize;

	rChannel.resize(xSize);
	gChannel.resize(xSize);
	bChannel.resize(xSize);
	rays.resize(xSize);

	for (size_t i = 0; i < xSize; ++i)
	{
		rChannel[i].resize(ySize, 0.0);
		gChannel[i].resize(ySize, 0.0);
		bChannel[i].resize(ySize, 0.0);
		rays[i].resize(ySize, Ray<double>());
	}
}

void Image::Clear()
{
	for (size_t i = 0; i < xSize; ++i)
	{
		std::fill(rChannel[i].begin(), rChannel[i].end(), 0);
		std::fill(gChannel[i].begin(), gChannel[i].end(), 0);
		std::fill(bChannel[i].begin(), bChannel[i].end(), 0);
		std::fill(rays[i].begin(), rays[i].end(), Ray<double>());
	}
}

void Image::Draw(ImDrawList* dl, const ImVec2& canvasOffset) {
	/*CalcMaxes();*/

	for (size_t x = 0; x < xSize; ++x)
	{
		for (size_t y = 0; y < ySize; ++y)
		{
			/*dl->AddRectFilled(
				ImVec2(x, y) + canvasOffset,
				ImVec2(x + 1, y + 1) + canvasOffset,
				IM_COL32(static_cast<ImU32>((rChannel.at(x).at(y) / _Max) * 255.0),
					static_cast<ImU32>((gChannel.at(x).at(y) / _Max) * 255.0),
					static_cast<ImU32>((bChannel.at(x).at(y) / _Max) * 255.0),
					255)
			);*/
			dl->AddRectFilled(
				ImVec2(x, y) + canvasOffset,
				ImVec2(x + 1, y + 1) + canvasOffset,
				IM_COL32(static_cast<ImU32>(std::min(rChannel.at(x).at(y), 1.0) * 255.0),
					static_cast<ImU32>(std::min(gChannel.at(x).at(y), 1.0) * 255.0),
					static_cast<ImU32>(std::min(bChannel.at(x).at(y), 1.0) * 255.0),
					255)
			);
		}
	}
}

void Image::SetPixel(const size_t x, const size_t y, const HVec<double>& color)
{
	rChannel.at(x).at(y) = color.At(0);
	gChannel.at(x).at(y) = color.At(1);
	bChannel.at(x).at(y) = color.At(2);
}

void Image::CalcMaxes()
{
	_maxRed = 0.0;
	_maxGreen = 0.0;
	_maxBlue = 0.0;
	_Max = 0.0;
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			double redValue = rChannel.at(x).at(y);
			double greenValue = gChannel.at(x).at(y);
			double blueValue = bChannel.at(x).at(y);

			if (redValue > _maxRed) _maxRed = redValue;
			if (greenValue > _maxGreen) _maxGreen = greenValue;
			if (blueValue > _maxBlue) _maxBlue = blueValue;
			if (_maxRed > _Max) _Max = _maxRed;
			if (_maxGreen > _Max) _Max = _maxGreen;
			if (_maxBlue > _Max) _Max = _maxBlue;
		}
	}
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