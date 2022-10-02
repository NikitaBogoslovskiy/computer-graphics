#include "../headers/funcs.h"
#include <stdio.h>
#include <cstdlib>
Edge::Edge(const ImVec2& p1, const ImVec2& p2, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p1);
	push_back(p2);
}

Point::Point(const ImVec2& p, const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{
	push_back(p);
}

void Point::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	draw_list->AddCircleFilled(this->operator[](0) + offset, thickness(), color(), 10);
	//draw_list->AddLine(this->operator[](0), this->operator[](0) +ImVec2(0.5f, 0.5f), color(), thickness());
}

void Primitive::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	ImVec2* temp = (ImVec2*)malloc(size() * sizeof(ImVec2));
	for (int i = 0; i < size(); i++) {
		temp[i] = this->operator[](i) + offset;
	}
	draw_list->AddPolyline(temp, size(), color(), 0, thickness());
	if (size() > 2) {
		draw_list->AddLine(front() + offset, back() + offset, color(), thickness());		
	}
}

void Primitive::draw_previe(ImDrawList* draw_list, const ImVec2& offset)
{
	ImVec2* temp = (ImVec2*)malloc(size() * sizeof(ImVec2));
	for (int i = 0; i < size(); i++) {
		temp[i] = this->operator[](i) + offset;
	}
	draw_list->AddPolyline(temp, size(), color(), 0, thickness());
	if (size() > 4) {
		draw_list->AddLine(front() + offset, back() + offset, color(), thickness());
		draw_list->AddLine(front() + offset, (*this)[size() - 2] + offset, IM_COL32((uint8_t)(ImGui::GetTime()*60), (uint8_t)(ImGui::GetTime() * 40), (uint8_t)(ImGui::GetTime() * 20), 255), thickness());
	}
}
