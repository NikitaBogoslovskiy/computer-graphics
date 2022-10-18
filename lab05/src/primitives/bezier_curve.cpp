#include "../../headers/geometry.h"
#include <vector>
#include <deque>
#include <iostream>
BezierCurve::BezierCurve(const ImU32& color, const float& thickness) : Primitive(new ImVector<ImVec2>(), color, thickness)
{

}

// draws frame of a curve
void BezierCurve::draw_skeleton(ImDrawList* draw_list, const ImVec2& offset, const ImU32& col)
{
	if (!show()) return;
	for (size_t i = 0; i < size() - 1; i++) {
		draw_list->AddLine(this->points->Data[i] + offset, this->points->Data[i + 1] + offset, IM_COL32((uint8_t)(ImGui::GetTime() * 60), (uint8_t)(ImGui::GetTime() * 40), (uint8_t)(ImGui::GetTime() * 20), 255), thickness());
		draw_list->AddCircleFilled(this->points->Data[i] + offset, this->thickness() + 2.f, col, 10);
	}
	draw_list->AddCircleFilled(this->points->Data[size() - 1] + offset, this->thickness() + 2.f, col, 10);
}

// spits out point corresponding to t assuming we draw a curve based on 4 points
ImVec2 BezierCurve::nextBt3(const std::deque<ImVec2>& points, const float& t) {
	if (points.size() < 4) throw std::invalid_argument("4 points required to get approximate Bezier point");
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
		Eigen::Matrix<float, 1, 2> p{ (points[i]).x, (points[i]).y };
		auto cringe_i = CRINGE[i] * p;
		ans.x += cringe_i[0];
		ans.y += cringe_i[1];
	}
	return ans;
}

// draws a curve using 4 points given
void BezierCurve::draw_curve3(ImDrawList* draw_list, const std::deque<ImVec2>& points, const ImVec2& offset, const ImU32& col) {
	if (!show() || points.size() < 4) return;
	ImVec2 p0 = points[0];
	for (int i = 1; i <= 1 / _step; i++) {
		ImVec2 p1 = nextBt3(points, _step * i);
		draw_list->AddLine(p0 + offset, p1 + offset, col, thickness());
		p0 = p1;
	}
}

void BezierCurve::draw(ImDrawList* draw_list, const ImVec2& offset, const ImU32& col)
{
	if (!show()) return;
	this->draw_skeleton(draw_list, offset, col);
	if (this->points->size() == 2) {
		draw_list->AddLine(this->points->Data[0] + offset, this->points->Data[1] + offset, col, thickness());
		return;
	}

	bool oddSize = this->size() % 2 != 0; // drawing the last piece of composite curve depends on the amount of points in frame
										  // if we have odd amount of points, the last piece consists of 3 points, which makes it necessary to insert one more point between the last and last.prev
										  // even - 4 points, we just draw the curve
	int curvesRemained = (this->size() + 1) / 2 - 1;
	std::deque<ImVec2> d = { this->front() }; // 4 points we base our cubic-beziers on
	int i = 1;
	while (curvesRemained > 0) {
		if (curvesRemained == 1) {
			if (oddSize) {
				d.push_back(this->points->Data[i]);
				d.push_back(0.5f * (this->points->Data[i] + this->points->Data[i + 1]));
				d.push_back(this->points->Data[i+1]);
			} else {
				while (d.size() < 4) {
					d.push_back(this->points->Data[i++]);
				}
			}
			this->draw_curve3(draw_list, d, offset, col);
			return;
		}
		while (d.size() < 3) { // for each non-closer curve we just add next two frame points and the linking dummy
			d.push_back(this->points->Data[i++]);
		}
		d.push_back(0.5f * (this->points->Data[i - 1] + this->points->Data[i]));
		this->draw_curve3(draw_list, d, offset, col);

		while (d.size() > 1) { // one last point of the current curve (dummy) is left to serve as a start for the next curve
			d.pop_front();
		}
		curvesRemained -= 1;
	}
}

void BezierCurve::draw_previe(ImDrawList* draw_list, const ImVec2& offset)
{
	this->draw(draw_list, offset, color());
}