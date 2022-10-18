#ifndef _BLEV_H_
#define _BLEV_H_

#include "pch.h"
#include "geometry.h"
#include <functional>

class BLEV
{
private:
	enum class Mode {
		Point,
		Edge,
		Polygon,
		BezierCurve,
		Select,
		FreeMove
	};
	enum class PrimEditMode {
		None,
		SelectPoints,
		MovePoints,
		SelectEdges,
		MoveEdges
	};
	enum AddingLine {
		None,
		FirstClick,
		ReleasedState,
		FinalClick
	} adding_line;

	struct CurrentState {
		Mode mode;
		int edgeAndPointOption = 0;
		std::string answer = "";
	};
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
	struct ConsoleFields {
		char pseudo_console[100] = { '\0' };
		std::string feedback;
		ImVec4 feedback_color;
	};

	//static const uint8_t modesSize = 7;
	//static char* modesList[modesSize]{ "Translation", "Point", "Edge", "Polygon", "Bezier Curve", "Select", "Free Move" };
	static const uint8_t primEditModesSize = 5;
	const char* primEditModesList[primEditModesSize]{ "None", "Select points", "Move points", "Select edges", "Move edges" };


	static const uint8_t modesSize = 6;
	const char* modesList[modesSize]{ "Point", "Edge", "Polygon", "Bezier Curve", "Select", "Free Move" };

	struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
	static const uint8_t hotkeysSize = 1;
	const ImGuiKey hotkeys[hotkeysSize]{ ImGuiKey_M };

	const uint8_t classificationTypeSize = 3;
	const char* classificationType[3]{ "Point and Edge", "Point and Convex Polygon", "Point and Non-convex Polygon" };

	float thickness = 1.0f;
	float curr_color[4] = { 1.f, 1.f, 0.f, 1.f };

	ImVec2 canvas_sz;
	float canvas_width;

	Primitive prev_displacement = Primitive(ImU32(1), 1);
	Primitive curr_displacement = Primitive(ImU32(1), 1);

	Primitive* touched_prim = nullptr;
	int point_of_transformation = -1; // maybe index would be useful for output in some case 
	void setTouchedPrim(Primitive* new_touched_prim, const int& new_point) {
		touched_prim = new_touched_prim;
		point_of_transformation = new_point;
	}

	size_t chosenPrimEditMode = 0;
	size_t chosenMode = 0;
	std::set<ImVec2*> chosen_prim_points;
	std::set<std::pair<ImVec2*,ImVec2*>> chosen_prim_edges;
	ImVec2 prev_point;

	std::vector<Primitive*> primitives;
	std::vector<Lsystem*> fractals;
	std::set<Primitive*> chosen_prims;
	std::set<Lsystem*> chosen_lsys;

	Primitive* new_prim;

	ImVector<ImVec2*> intersections;

	std::vector<ready_l_system*> ready_l_systems{

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
	std::vector<ConsoleFields*> console{
		new ConsoleFields(),
		new ConsoleFields(),
		new ConsoleFields(),
		new ConsoleFields()
	};

	bool rotate_open = false;
	bool translate_open = false;
	bool scale_open = false;
	bool displace_open = false;
	bool lsys_open = false;
	bool classify_open = false;

public:
	BLEV() {}

	void ShowMenuBar();
	void ShowAdditionalWindows();
	void ShowContent();

private:
	void ShowModes();
	void ShowFuncs();
	void PollCallbacks();

	void ShowPrimitiveTableRow(Primitive* prim, size_t idx);
	void ShowFractalTableRow(Lsystem* lsys, size_t idx);

	void NewWindow(const char* label, bool* p_open, void (BLEV::* func)());

	void F_Rotate();
	void F_Translate();
	void F_Scale();
	void F_Displace();
	void F_Lsystem();
	void F_Classify();

	template<typename _Container,
		typename _Value = typename _Container::value_type,
		typename = std::enable_if_t<std::is_convertible_v<_Value, Primitive*>>>
	std::tuple<int, ImVec2*> detect_point(const _Container& primitives);

	template<typename _Container,
		typename _Value = typename _Container::value_type,
		typename = std::enable_if_t<std::is_convertible_v<_Value, Primitive*>>>
	int tr_chpr_rtp(const _Container& primitives, std::function<void(Primitive*, ImVec2*)> lammy);

	Primitive midpointDisplacement(Primitive& displacement, Point* p1, Point* p2, int R, int I, int iter_num);

	bool intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out);
	ImVector<ImVec2*> get_intersections(std::set<Primitive*> prims);

	void pointPositionWithPolygon(Point& point, Primitive& polygon, bool& isInside, float canvas_width);
	bool checkPointAndEdgeConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Edge*& edge);
	bool checkPointAndPolygonConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Primitive*& polygon);
	ImVector<ImVec2*> get_intersections(Primitive* curr, std::vector<Primitive*> prims);
};

#endif // _BLEV_H_
