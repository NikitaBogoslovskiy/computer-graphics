#include "../headers/funcs.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

Edge::Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p1);
	push_back(p2);
}

void Edge::rotate(const float& angle)
{
	auto m3f = Affine::rotate(angle, ImVec2((this->at(1).x + this->at(0).x) / 2.f, (this->at(1).y + this->at(0).y) / 2.f));
	Eigen::Matrix<float, 1, 3> v3f1{ this->at(0).x, this->at(0).y, 1.f };
	Eigen::Matrix<float, 1, 3> v3f2{ this->at(1).x, this->at(1).y, 1.f };

	auto v3f1_2 = v3f1 * m3f;
	auto v3f2_2 = v3f2 * m3f;

	this->at(0).x = v3f1_2(0, 0);
	this->at(0).y = v3f1_2(0, 1);
	this->at(1).x = v3f2_2(0, 0);
	this->at(1).y = v3f2_2(0, 1);
}

Point::Point(const ImVec2& p, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p);
}

void Point::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	if (show()) {
		draw_list->AddCircleFilled(this->operator[](0) + offset, thickness(), color(), 10);
	}
}

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
			draw_list->AddLine(front() + offset, (*this)[size() - 2] + offset, IM_COL32((uint8_t)(ImGui::GetTime()*60), (uint8_t)(ImGui::GetTime() * 40), (uint8_t)(ImGui::GetTime() * 20), 255), thickness());
		}
	}
}

void Primitive::rotate(const float& angle)
{
	// todo:
}
