#include "../../headers/geometry.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

void MyPolyline(ImDrawList* draw_list, const ImVec2* points, const size_t points_count, ImU32 col, float thickness, const ImVec2& offset) {
	for (size_t i = 0; i < points_count - 1; i++) {
		draw_list->AddLine(points[i] + offset, points[i + 1] + offset, col, thickness);
	}
}

void Primitive::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	if (show()) {
		MyPolyline(draw_list, this->points->Data, size(), color(), thickness(), offset);
		if (size() > 2) {
			draw_list->AddLine(front() + offset, back() + offset, color(), thickness());
		}
	}
}

void Primitive::draw_previe(ImDrawList* draw_list, const ImVec2& offset)
{
	if (show()) {
		MyPolyline(draw_list, this->points->Data, size(), color(), thickness(), offset);
		if (size() > 3) {
			draw_list->AddLine(front() + offset, (*this)[size() - 2] + offset, IM_COL32((uint8_t)(ImGui::GetTime() * 60), (uint8_t)(ImGui::GetTime() * 40), (uint8_t)(ImGui::GetTime() * 20), 255), thickness());
		}
	}
}

void Primitive::rotate(const float& angle)
{
	float y = 0.f, x = 0.f;
	for (size_t i = 0; i < size(); i++) {
		x += this->at(i).x;
		y += this->at(i).y;
	}

	auto m3f = Affine::rotate(angle, ImVec2(x / size(), y / size()));

	for (size_t i = 0; i < size(); i++) {
		Eigen::Matrix<float, 1, 3> v3fi{ this->at(i).x, this->at(i).y, 1.f };
		auto v3fi_2 = v3fi * m3f;
		this->at(i).x = v3fi_2(0, 0);
		this->at(i).y = v3fi_2(0, 1);
	}
}

void Primitive::scale(const float& scaleCoeffX, const float& scaleCoeffY)
{
	float y = 0.f, x = 0.f;
	for (size_t i = 0; i < size(); i++) {
		x += this->at(i).x;
		y += this->at(i).y;
	}

	auto m3f = Affine::scale(scaleCoeffX, scaleCoeffY, ImVec2(x / size(), y / size()));

	for (size_t i = 0; i < size(); i++) {
		Eigen::Matrix<float, 1, 3> v3fi{ this->at(i).x, this->at(i).y, 1.f };
		auto v3fi_2 = v3fi * m3f;
		this->at(i).x = v3fi_2(0, 0);
		this->at(i).y = v3fi_2(0, 1);
	}
}

void Primitive::translate(const ImVec2& d)
{
	auto m3f = Affine::translate(d);

	for (size_t i = 0; i < size(); i++) {
		Eigen::Matrix<float, 1, 3> v3fi{ this->at(i).x, this->at(i).y, 1.f };
		auto v3fi_2 = v3fi * m3f;
		this->at(i).x = v3fi_2(0, 0);
		this->at(i).y = v3fi_2(0, 1);
	}
}
