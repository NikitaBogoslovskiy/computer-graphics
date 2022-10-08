#include "../../headers/geometry.h"
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