#include "../../headers/geometry.h"
#include <vector>
#include <iostream>

BezierCurve::BezierCurve(const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{

}

ImVec2& BezierCurve::nextBt3(const ImVector<ImVec2>* points, const float& t) {
	if (points->size() < 4) throw std::invalid_argument("4 points required to get approximate Bezier point");
	Eigen::Matrix<float, 1, 4> tVec{ 1, t, t * t, t * t * t };
	Eigen::Matrix4f coeffs{
			{ 1.f, 0.f, 0.f, 0.f },
			{ -3.f, 3.f, 0.f, 0.f },
			{ 3.f, -6.f, 3.f, 0.f},
			{ -1.f, 3.f, -3.f, 1.f}
	};
	auto CRINGE = tVec * coeffs;
	ImVec2 ans(0.f, 0.f);
	for (size_t i = 0; i < 4; i++) {
		Eigen::Matrix<float, 1, 2> p{ this->at(i).x, this->at(i).y };
		auto cringe_i = CRINGE[i] * p;
		ans.x += cringe_i[0];
		ans.y += cringe_i[1];
	}
	return ans;
}

void BezierCurve::draw_skeleton(ImDrawList* draw_list, const ImVec2& offset)
{
	if (!show()) return;
	for (size_t i = 0; i < size() - 1; i++) {
		draw_list->AddLine(this->points->Data[i] + offset, this->points->Data[i + 1] + offset, IM_COL32((uint8_t)(ImGui::GetTime() * 60), (uint8_t)(ImGui::GetTime() * 40), (uint8_t)(ImGui::GetTime() * 20), 255), thickness());
		draw_list->AddCircleFilled(this->points->Data[i] + offset, 4.f, color(), 10);
	}
	draw_list->AddCircleFilled(this->points->Data[size() - 1] + offset, 4.f, color(), 10);
}

void BezierCurve::draw(ImDrawList* draw_list, const ImVec2& offset)
{
	if (!show()) return;
	this->draw_skeleton(draw_list, offset);
	if (size() < 4) return;

	ImVec2 p0 = this->points->Data[0];
	float step = 0.02;

	for (int i = 1; i <= 1 / step; i++) {
		ImVec2 p1 = nextBt3(this->points, step * i);
		draw_list->AddLine(p0 + offset, p1 + offset, color(), thickness());
		p0 = p1;
	}

}

void BezierCurve::draw_previe(ImDrawList* draw_list, const ImVec2& offset)
{
	this->draw(draw_list, offset);
}