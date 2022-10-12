#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#include <string>
#include <deque>
#include <tuple>

enum class Mode {
	Point,
	Edge,
	Polygon,
	//ConvexPolygon,
	//NonConvexPolygon,
	None
};

struct CurrentState {
	Mode mode;
	int edgeAndPointOption = 0;
	std::string answer = "";
};

const unsigned char modesSize = 3;
const char* modesList[modesSize]{ "Point", "Edge", "Polygon" }; //, "Convex Polygon", "Non-convex Polygon" };

const unsigned char classificationTypeSize = 3;
const char* classificationType[classificationTypeSize]{ "Point and Edge", "Point and Convex Polygon", "Point and Non-convex Polygon" };

struct ready_l_system
{
	std::string name;
	std::string axiom;
	float angle; //rad
	std::deque<std::pair<char, std::string>> rules;
	std::string fwd_atoms;
	ready_l_system(
		std::string _name,
		std::string _axiom,
		float _angle,
		std::deque<std::pair<char, std::string>> _rules,
		std::string _fwd_atoms) {
		name = _name;
		axiom = _axiom;
		angle = _angle;
		rules = _rules;
		fwd_atoms = _fwd_atoms;
	}
};

#endif