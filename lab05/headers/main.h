#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#include "pch.h"
#include "geometry.h"
#include <string>
#include <deque>
#include <tuple>

enum class Mode {
	Translating,
	Point,
	Edge,
	Polygon,
	BezierCurve
};

struct CurrentState {
	Mode mode;
	int edgeAndPointOption = 0;
	std::string answer = "";
};

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

struct TextFilters
{
	static int FilterLetters(ImGuiInputTextCallbackData* data)
	{
		if (data->EventChar < 256 && strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", (char)data->EventChar))
			return 0;
		return 1;
	}

	static int FilterLsys(ImGuiInputTextCallbackData* data)
	{
		if (data->EventChar < 256 && strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ+-[]@", (char)data->EventChar))
			return 0;
		return 1;
	}
};

static std::vector<ready_l_system*> ready_l_systems{

	//Кривая Коха
	new ready_l_system("Koch curve",
		"F",
		PI / 3.f,
		std::deque<std::pair<char, std::string>>{
			std::make_pair('F', "F-F++F-F")
		},
		""),

	//Квадратный остров Коха
	new ready_l_system("Koch island",
		"F+F+F+F",
		PI / 2.f,
		std::deque<std::pair<char, std::string>>{
			std::make_pair('F', "F+F-F-FF+F+F-F")
		},
		""),

				//Ковёр Серпинского
				new ready_l_system("Sierpinski carpet",
					"FXF--FF--FF",
					PI / 3.f,
					std::deque<std::pair<char, std::string>>{
						std::make_pair('F', "FF"),
						std::make_pair('X', "--FXF++FXF++FXF--")
					},
					""),

				//Наконечник Серпинского(треугольник)
				new ready_l_system("Sierpinski triangle",
					"YF",
					PI / 3.f,
					std::deque<std::pair<char, std::string>>{
						std::make_pair('F', "F"),
						std::make_pair('X', "YF+XF+Y"),
						std::make_pair('Y', "XF-YF-X")
					},
					""),

							//Кривая Гильберта
							new ready_l_system("Hilbert curve",
								"X",
								PI / 2.f,
								std::deque<std::pair<char, std::string>>{
									std::make_pair('F', "F"),
									std::make_pair('X', "-YF+XFX+FY-"),
									std::make_pair('Y', "+XF-YFY-FX+")
								},
								""),

							//Кривая дракона Хартера - Хейтуэя
							new ready_l_system("Dragon curve",
								"X",
								PI / 2.f,
								std::deque<std::pair<char, std::string>>{
									std::make_pair('F', "F"),
									std::make_pair('X', "X+YF+"),
									std::make_pair('Y', "-FX-Y")
								},
								""),

										//Шестиугольная кривая Госпера
										new ready_l_system("Gosper curve",
											"XF",
											PI / 3.f,
											std::deque<std::pair<char, std::string>>{
												std::make_pair('F', "F"),
												std::make_pair('X', "X+YF++YF-FX--FXFX-YF+"),
												std::make_pair('Y', "-FX+YFYF++YF+FX--FX-Y")
											},
											""),

										new ready_l_system("Tree 1",
											"X",
											PI / 9.f,
											std::deque<std::pair<char, std::string>>{
												std::make_pair('F', "FF"),
												std::make_pair('X', "F[+X]F[-X]+X")
											},
											""),

										new ready_l_system("Shrub 1",
											"F",
											PI / 90.f * 11.f, // 22
											std::deque<std::pair<char, std::string>>{
												std::make_pair('F', "FF-[-F+F+F]+[+F-F-F]")
											},
											""),

										new ready_l_system("Shrub 2",
											"X",
											PI / 9.f, // 20
											std::deque<std::pair<char, std::string>>{
												std::make_pair('F', "FF"),
												std::make_pair('X', "F[+X]F[-X]+X")
											},
											""),

										new ready_l_system("Shrub 3",
											"X",
											PI / 8.f, // 22.5
											std::deque<std::pair<char, std::string>>{
												std::make_pair('F', "FF"),
												std::make_pair('X', "F-[[X]+X]+F[+FX]-X")
											},
											""),

													//Шестиугольная мозаика
													new ready_l_system("Hexagonal Mosaic",
														"X",
														PI / 3.f,
														std::deque<std::pair<char, std::string>>{
															std::make_pair('F', "F"),
															std::make_pair('X', "[-F+F[Y]+F][+F-F[X]-F]"),
															std::make_pair('Y', "[-F+F[Y]+F][+F-F-F]")
														},
														""),

													new ready_l_system("\"Random\" tree",
														"X",
														PI / 4.f,
														std::deque<std::pair<char, std::string>>{
															std::make_pair('X', "F[@[-X]+X]")
														},
														"F"),
};

#endif