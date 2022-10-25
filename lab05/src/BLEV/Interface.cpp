#define _CRT_SECURE_NO_WARNINGS
#include "BLEV/Interface.h"
#include <unordered_set>

const std::vector<BLEV::Interface::ready_l_system*> BLEV::Interface::ready_l_systems{

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

struct BLEV::Interface::ConsoleFields {
	char pseudo_console[100] = { '\0' };
	std::string feedback;
	ImVec4 feedback_color;
};

void BLEV::Interface::F_Rotate() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleRotate", console[0]->pseudo_console, 100);
	if (!console[0]->feedback.empty()) {
		ImGui::TextColored(console[0]->feedback_color, console[0]->feedback.c_str());
	}
	ImGui::EndGroup();
	HelpPrevItem("anlge(degree)");

	if (ImGui::Button("Rotate 90")) {
		try {
			auto lammy = [](Primitive* prim, ImVec2* origin) { prim->rotate(DegreesToRadians(90.f), origin); };
			if (_data.chosen_prims.size() != 0) {
				tr_chpr_rtp(_data.chosen_prims, lammy);
			}
			for (auto lsys : _data.chosen_lsys) {
				lsys->rotate(DegreesToRadians(90.f), nullptr);
			}
			console[0]->feedback = "";
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Rotate N")) {
		try {
			char* nstr = console[0]->pseudo_console; float angle;
			if (sscanf(nstr, "%f", &angle) != 1) throw std::invalid_argument("Incorrect arguments format for rotate N");
			console[0]->feedback = "";
			auto lammy = [&angle](Primitive* prim, ImVec2* origin) { prim->rotate(DegreesToRadians(angle), origin); };
			if (_data.chosen_prims.size() + _data.chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}
			if (_data.chosen_prims.size() != 0) {
				tr_chpr_rtp(_data.chosen_prims, lammy);
			}
			for (auto lsys : _data.chosen_lsys) {
				lsys->rotate(DegreesToRadians(angle), nullptr);
			}
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}
void BLEV::Interface::F_Translate() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleTranslate", console[1]->pseudo_console, 100);
	if (!console[1]->feedback.empty()) {
		ImGui::TextColored(console[1]->feedback_color, console[1]->feedback.c_str());
	}
	ImGui::EndGroup();
	HelpPrevItem("dx;dy");

	if (ImGui::Button("Translate")) {
		try {
			char* nstr = console[1]->pseudo_console; float dx, dy;
			if (sscanf(nstr, "%f%*c%f", &dx, &dy) != 2) throw std::invalid_argument("Incorrect arguments format for translate");
			console[1]->feedback = "";
			auto d = ImVec2(-1 * dx, -1 * dy);
			auto lammy = [&dx, &dy, &d](Primitive* prim) { prim->translate(&d); };

			if (_data.chosen_prims.size() + _data.chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}
			std::for_each(_data.chosen_prims.begin(), _data.chosen_prims.end(), lammy);
			for (auto lsys : _data.chosen_lsys) {
				lsys->translate(&d);
			}
		}
		catch (std::exception& e) {
			console[1]->feedback = e.what();
			console[1]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}
void BLEV::Interface::F_Scale() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleScale", console[2]->pseudo_console, 100);
	if (!console[2]->feedback.empty()) {
		ImGui::TextColored(console[2]->feedback_color, console[2]->feedback.c_str());
	}
	ImGui::EndGroup();
	HelpPrevItem("s_x;s_y (scale factors)");

	if (ImGui::Button("Scale")) {
		try {
			char* nstr = console[2]->pseudo_console; float scaleX, scaleY;
			if (sscanf(nstr, "%f%*c%f", &scaleX, &scaleY) != 2) throw std::invalid_argument("Incorrect arguments format for scale");
			if (scaleX <= 0 || scaleY <= 0) throw std::invalid_argument("Incorrect arguments format for scale");
			console[2]->feedback = "";

			auto lammy = [&scaleX, &scaleY](Primitive* prim, ImVec2* origin) {
				if (dynamic_cast<Point*>(prim) != NULL) return;
				prim->scale(scaleX, scaleY, origin);
			};

			if (_data.chosen_prims.size() + _data.chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}

			if (_data.chosen_prims.size() != 0) {
				tr_chpr_rtp(_data.chosen_prims, lammy);
			}

			for (auto lsys : _data.chosen_lsys) {
				lsys->scale(scaleX, scaleY, nullptr);
			}
		}
		catch (std::exception& e) {
			console[2]->feedback = e.what();
			console[2]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}
void BLEV::Interface::F_Displace() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleDisplace", console[3]->pseudo_console, 100);
	if (!console[3]->feedback.empty()) {
		ImGui::TextColored(console[3]->feedback_color, console[3]->feedback.c_str());
	}
	ImGui::EndGroup();
	HelpPrevItem("R=_ I=_ iters=_");

	if (ImGui::Button("Displace")) {
		try {
			if (_data.chosen_prims.size() != 2)
				throw std::invalid_argument("You should choose 2 points");
			std::vector<Point*> points;
			for (auto it = _data.chosen_prims.begin(); it != _data.chosen_prims.end(); ++it)
			{
				auto prim = dynamic_cast<Point*>(*it);
				if (prim == nullptr)
					throw std::invalid_argument("You should choose 2 points");
				points.push_back(prim);
			}

			char* nstr = console[3]->pseudo_console;
			int R, I, iter_num;
			if (sscanf(nstr, "R=%d I=%d iters=%d", &R, &I, &iter_num) != 3)
				throw std::invalid_argument("Incorrect arguments format for displace");
			if (R < 0 || I < 0)
				throw std::invalid_argument("R and I cannot be negative");
			if (iter_num < 1)
				throw std::invalid_argument("Iterations number must be positive");
			console[3]->feedback = "";

			_data.curr_displacement = midpointDisplacement(_data.prev_displacement, points[0], points[1], R, I, iter_num);
			if (_data.prev_displacement.size() != 0)
			{
				auto it = std::find(_data.primitives.begin(), _data.primitives.end(), &_data.prev_displacement);
				if (it != _data.primitives.end())
					_data.primitives.erase(it);
			}
			_data.prev_displacement = _data.curr_displacement;
			_data.prev_displacement.set_connect_bounds(2);
			_data.primitives.push_back(&_data.prev_displacement);
		}
		catch (std::exception& e) {
			console[3]->feedback = e.what();
			console[3]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}
void BLEV::Interface::F_Lsystem() {
	HelpMarker(
		"An example:\n"
		"Axiom: 'A'\n"
		"rule1: 'A', 'A-B--B+A++AA+B-'\n"
		"rule2: 'B', '+A-BB--B-A++A+B'");

	static char axiom[255] = "\0";
	static std::deque<std::pair<char, char*>> rules;
	if (rules.size() == 0) {
		char term;
		char* rule = (char*)malloc(sizeof(char) * 255);
		rule[0] = (term = '\0');
		rules.push_back(std::pair<char, char*>(term, rule));
	}

	ImGuiInputTextFlags flags2 = ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CallbackCharFilter;


	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);
	ImGui::InputText("Axiom", axiom, 254, flags2, TextFilters::FilterLsys);


	for (size_t i = 0; i < rules.size(); i++) {
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 1);
		char buf1[32];
		sprintf(buf1, "##lsysterm%d", i + 1);
		ImGui::InputText(buf1, &rules[i].first, 2, flags2, TextFilters::FilterLetters);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 13);
		char buf2[32];
		sprintf(buf2, "rule%d##lsysrule%d", i + 1, i + 1);
		ImGui::InputText(buf2, rules[i].second, 255, flags2, TextFilters::FilterLsys);
	}

	if (rules.back().first != '\0' && rules.back().second[0] != '\0') {
		char term;
		char* rule = (char*)malloc(sizeof(char) * 255);
		rule[0] = (term = '\0');
		rules.push_back(std::pair<char, char*>(term, rule));
	}

	static char additional[30] = { '\0' };
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10);
	ImGui::InputText("forward atoms", additional, 30, flags2, TextFilters::FilterLetters);
	HelpPrevItem("Used to draw a line immediately");

	ImGui::Separator();

	static float angle = PI / 3.f; // 60 deg
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::SliderAngle("##lsysangle", &angle, 0.f);

	ImGui::SameLine();

	static int iters = 3;
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::SliderInt("##lsysiters", &iters, 2, 5, "%d iters");

	ImGui::SameLine();
	static float src_col[4] = { 1.f, 1.f, 0.f, 1.f };
	ImGui::ColorEdit4("src##lsysSrcColor", src_col, ImGuiColorEditFlags_NoInputs);

	static bool tree = false;
	static float dest_col[4] = { 1.f, 1.f, 0.f, 1.f };

	if (tree) {
		ImGui::SameLine();
		ImGui::ColorEdit4("dest##lsysDestColor", dest_col, ImGuiColorEditFlags_NoInputs);
	}

	static float th = 2.f;
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10);
	ImGui::SliderFloat("##changeLsysTh", &th, 1.f, 10.f, "th = %.1f");

	ImGui::SameLine();
	ImGui::Checkbox("tree?", &tree);
	HelpPrevItem("Changing the thickness, color and length of lines");


	static ready_l_system* selected = NULL;

	if (ImGui::Button("Select.."))
		ImGui::OpenPopup("select_popup_ready_lsys");
	ImGui::SameLine();
	ImGui::TextUnformatted(selected == NULL ? "<None>" : selected->name.c_str());
	if (ImGui::BeginPopup("select_popup_ready_lsys"))
	{
		ImGui::Text("Ready L-systems");
		ImGui::Separator();
		for (size_t i = 0; i < ready_l_systems.size(); i++)
			if (ImGui::Selectable(ready_l_systems[i]->name.c_str())) {
				selected = ready_l_systems[i];
				//axiom
				strcpy(axiom, selected->axiom.c_str());
				//forward atoms
				strcpy(additional, selected->fwd_atoms.c_str());
				//rules
				rules.clear();
				for (size_t r = 0; r < selected->rules.size(); r++) {
					rules.push_back(std::pair<char, char*>(selected->rules[r].first, (char*)malloc(255 * sizeof(char))));
					strcpy(rules.back().second, selected->rules[r].second.c_str());
				}
				//angle
				angle = selected->angle;
			}
		ImGui::EndPopup();
	}

	static size_t state = 0;

	if (ImGui::Button("Add##lsysaddingconfirm")) {
		auto t = new Lsystem(
			std::string(axiom),
			std::vector<std::pair<char, std::string>>(rules.begin(), rules.end()),
			angle,
			iters,
			GetColorFlV4(src_col),
			th,
			GetColorFlV4(dest_col),
			std::string(additional),
			tree);
		if (t->is_legal()) {
			state = 1;
			_data.fractals.push_back(t);
		}
		else {
			state = 2;
		}
	}

	ImGui::SameLine();
	if (state == 1) {
		ImGui::TextColored(ImVec4(0, 255, 0, 255), "Added");
	}
	if (state == 2) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "Wrong format");
	}
}
void BLEV::Interface::F_Classify() {
	static std::string feedback;
	static ImVec4 feedback_color;

	static int chosenType = 0;
	ImGui::Combo("##", &chosenType, _data.classificationType, _data.classificationTypeSize);

	if (ImGui::Button("Classify point position")) {
		Point* point = nullptr;
		Edge* edge = nullptr;
		Primitive* polygon = nullptr;
		bool success, onTheLeft, isInside;

		switch (chosenType)
		{
		case 0:
			success = checkPointAndEdgeConditions(_data.chosen_prims, feedback, point, edge);
			if (success)
			{
				feedback_color = ImVec4(0, 255, 0, 255);
				pointPositionWithEdge(*point, *edge, onTheLeft);
				if (onTheLeft)
					feedback = "Point on the left";
				else
					feedback = "Point on the right";
			}
			else
				feedback_color = ImVec4(255, 0, 0, 255);
			break;
		case 1:
			success = checkPointAndPolygonConditions(_data.chosen_prims, feedback, point, polygon);
			if (success)
			{
				feedback_color = ImVec4(0, 255, 0, 255);
				pointPositionWithConvexPolygon(*point, *polygon, isInside);
				if (isInside)
					feedback = "Point is inside";
				else
					feedback = "Point is outside";
			}
			else
				feedback_color = ImVec4(255, 0, 0, 255);
			break;
		case 2:
			success = checkPointAndPolygonConditions(_data.chosen_prims, feedback, point, polygon);
			if (success)
			{
				feedback_color = ImVec4(0, 255, 0, 255);
				pointPositionWithPolygon(*point, *polygon, isInside, canvas.size.x);
				if (isInside)
					feedback = "Point is inside";
				else
					feedback = "Point is outside";
			}
			else
				feedback_color = ImVec4(255, 0, 0, 255);
			break;
		default:
			break;
		}
	}

	if (!feedback.empty()) {
		ImGui::TextColored(feedback_color, feedback.c_str());
	}
}

void BLEV::Interface::ShowExternalWindows()
{
	if (bmo.b_rotate_open) {
		ImGui::Begin("Rotate", &bmo.b_rotate_open);
		F_Rotate();
		ImGui::End();
	}
	if (bmo.b_translate_open) {
		ImGui::Begin("Translate", &bmo.b_translate_open);
		F_Translate();
		ImGui::End();
	}
	if (bmo.b_scale_open) {
		ImGui::Begin("Scale", &bmo.b_scale_open);
		F_Scale();
		ImGui::End();
	}
	if (bmo.b_displace_open) {
		ImGui::Begin("Displace", &bmo.b_displace_open);
		F_Displace();
		ImGui::End();
	}
	if (bmo.b_lsys_open) {
		ImGui::Begin("Lsystem", &bmo.b_lsys_open);
		F_Lsystem();
		ImGui::End();
	}
	if (bmo.b_classify_open) {
		ImGui::Begin("Classify", &bmo.b_classify_open);
		F_Classify();
		ImGui::End();
	}
}

void BLEV::Interface::ShowContent()
{
	static bool use_work_area = true;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	if (ImGui::Begin("CringeCAD", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		menu.Show(bmo, global_vp);

		//obj_table.Show();

		//ImGui::SameLine();
		canvas.Show();

		ImGui::End();
	}
}

void BLEV::Interface::Menu::ShowModesMenu()
{
	if (ImGui::BeginMenu("Mode")) {
		for (size_t i = 0; i < _data.modesSize; i++) {
			if (ImGui::Selectable(_data.modesList[i], _data.chosenMode == (Mode)i)) {
				_data.chosenMode = (Mode)i;
			}
		}
		ImGui::EndMenu();
	}
}
void BLEV::Interface::Menu::ShowMethodsMenu(B_method_open& bmo)
{
	if (ImGui::BeginMenu("Rotate")) {

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			bmo.b_rotate_open = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Translate")) {

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			bmo.b_translate_open = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Scale")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			bmo.b_scale_open = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Displace")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			bmo.b_displace_open = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("L-system")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			bmo.b_lsys_open = true;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Classify")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			bmo.b_classify_open = true;
		}
		ImGui::EndMenu();
	}
}

void BLEV::Interface::Menu::Show(B_method_open& bmo, Global_visual_params& gvp)
{
	if (ImGui::BeginMainMenuBar())
	{
		HelpMarker("Hotkeys for multiple tools:\n> Select: P\n> Free move: M\n> Point: P\n> Edge: E\n> Polygon: G\n> Bezier cruve: B");

		ImGui::SameLine();

		ShowModesMenu();

		ImGui::Separator();

		if (ImGui::BeginMenu("Edit"))
		{
			ShowMethodsMenu(bmo);
			ImGui::EndMenu();
		}

		ImGui::SetNextItemWidth(200.f);

		ImGui::DragFloat("##globalThickness", &gvp.vp.thickness, 0.05f, 1.f, 10.f, "thickness = %.1f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::ColorEdit4("Line color", gvp.color4f, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);

		ImGui::Separator();

		ImGui::Text("Mode: %s", _data.chosenPrimEditMode == PrimEditMode::None ? _data.modesList[(uint32_t)_data.chosenMode] : _data.primEditModesList[(uint32_t)_data.chosenPrimEditMode]);

		if (ImGui::Button("Add cube")) {
			_data.meshes.push_back(new Cube(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::Button("Add tetrahedr")) {
			_data.meshes.push_back(new Tetrahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::Button("Add octahedr")) {
			_data.meshes.push_back(new Octahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::Button("Add Dodecahedron")) {
			_data.meshes.push_back(new Dodecahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::Button("Add Icosahedron")) {
			_data.meshes.push_back(new Icosahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		ImGui::EndMainMenuBar();
	}
}

void BLEV::Interface::ObjectTable::ShowPrimTable(Primitive* prim, size_t idx)
{
	{
		ImGui::PushID(prim);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode("Prim", "prim%d", idx);
		ImGui::TableSetColumnIndex(1);

		if (_data.chosen_prims.find(prim) != _data.chosen_prims.end()) {
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "%d-gon figure", prim->size());
		}
		else {
			ImGui::Text("%d-gon figure", prim->size());
		}

		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopup("context")) {

			if (ImGui::MenuItem("Delete object", NULL, false, true)) {
				_data.chosen_prims.erase(prim);
				_data.primitives.erase(std::remove(_data.primitives.begin(), _data.primitives.end(), prim), _data.primitives.end());
				if (_data.chosen_prims.size() == 0) {
					_data.chosenPrimEditMode = PrimEditMode::None;
					_data.chosen_prim_points.clear();
					_data.chosen_prim_edges.clear();
				}
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			if (_data.chosen_prims.find(prim) == _data.chosen_prims.end()) {
				_data.chosen_prims.insert(prim);
			}
			else {
				_data.chosen_prims.erase(prim);
				if (_data.chosen_prims.size() == 0) {
					_data.chosenPrimEditMode = PrimEditMode::None;
					_data.chosen_prim_points.clear();
					_data.chosen_prim_edges.clear();
				}
			}
		}

		ImGui::SameLine();
		ImGui::Checkbox(" ", &prim->show());

		if (node_open)
		{
			for (size_t i = 0; i < prim->size(); i++) {
				ImGui::PushID(&(prim->operator[](i)));

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);


				ImGui::AlignTextToFramePadding();
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
				ImGui::Text("Point %d", i);

				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
				if (ImGui::BeginPopup("context")) {

					if (ImGui::MenuItem("Delete point", NULL, false, true)) {
						if (prim->size() > 1) {
							prim->pop(&prim->at(i));
						}
						else {
							_data.chosen_prims.erase(prim);
							_data.primitives.erase(_data.primitives.begin() + idx);
							if (_data.chosen_prims.size() == 0) {
								_data.chosenPrimEditMode = PrimEditMode::None;
								_data.chosen_prim_points.clear();
								_data.chosen_prim_edges.clear();
							}
						}
					}
					ImGui::EndPopup();
				}

				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat("x", &(prim->operator[](i).x), 1.f, -1000.f, 1000.f, "%.0f");
				ImGui::DragFloat("y", &(prim->operator[](i).y), 1.f, -1000.f, 1000.f, "%.0f");

				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}
void BLEV::Interface::ObjectTable::ShowLsysTable(Lsystem* lsys, size_t idx)
{
	ImGui::PushID(lsys);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("Lsystem", "lsys%d", idx);
	ImGui::TableSetColumnIndex(1);

	if (_data.chosen_lsys.find(lsys) != _data.chosen_lsys.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), lsys->is_tree() ? "tree" : "frac");
	}
	else {
		ImGui::Text(lsys->is_tree() ? "tree" : "frac");
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (_data.chosen_lsys.find(lsys) == _data.chosen_lsys.end()) {
			_data.chosen_lsys.insert(lsys);
		}
		else {
			_data.chosen_lsys.erase(lsys);
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox(" ", &lsys->show());

	if (node_open)
	{
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.7);
		ImGui::SliderFloat("##changeLsysTh", &lsys->thickness(), 1.f, 10.f, "th = %.1f");

		ImGui::Checkbox("alive?", &lsys->is_alive());

		ImGui::TreePop();
	}

	ImGui::PopID();
}
void BLEV::Interface::ObjectTable::ShowMeshTable(Mesh* mesh, size_t idx)
{
	ImGui::PushID(mesh);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("Prim", "prim%d", idx);
	ImGui::TableSetColumnIndex(1);

	ImGui::Text("%d-hedr figure", mesh->polygons_size());
	ImGui::Checkbox(" ", &mesh->show);

	ImGui::SameLine();

	if (node_open)
	{
		//ImGui::TableNextRow();
		//ImGui::TableSetColumnIndex(0);
		//ImGui::SetNextItemWidth(-FLT_MAX);
		//ImGui::DragFloat(" ", &(mesh->thickness), 0.05f, 1.f, 10.f, "th %.2f");

		for (size_t i = 0; i < mesh->points_size(); i++) {
			ImGui::PushID(&(mesh->getPoint(i)));

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);

			ImGui::AlignTextToFramePadding();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
			ImGui::Text("Point %d", i);

			ImGui::TableSetColumnIndex(1);
			ImGui::DragFloat("x", &(mesh->getPoint(i).x), 1.f, -1000.f, 1000.f, "%.0f");
			ImGui::DragFloat("y", &(mesh->getPoint(i).y), 1.f, -1000.f, 1000.f, "%.0f");
			ImGui::DragFloat("z", &(mesh->getPoint(i).z), 1.f, -1000.f, 1000.f, "%.0f");

			ImGui::PopID();
		}

		ImGui::TreePop();
	}

	ImGui::PopID();
}

void BLEV::Interface::ObjectTable::Show()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("Objects##blev_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit, ImVec2(200.f, 0.f))) // ImGuiTableFlags_NoHostExtendX
	{
		// Iterate placeholder objects (all the same data)
		for (size_t i = 0; i < _data.primitives.size(); i++)
		{
			ShowPrimTable(_data.primitives[i], i);
			//ImGui::Separator();
		}

		for (size_t i = 0; i < _data.fractals.size(); i++)
		{
			ShowLsysTable(_data.fractals[i], i);
			//ImGui::Separator();
		}

		for (size_t i = 0; i < _data.meshes.size(); i++)
		{
			ShowMeshTable(_data.meshes[i], i);
			//ImGui::Separator();
		}
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
}

void BLEV::Interface::Canvas::ProcessCamKeyboardInput(ImGuiIO& io, Camera& cam, float& deltaTime) {
	auto x = (float)io.MouseWheel;
	if (x != 0.f) {
		cam.altPerspectiveScale(x < 0 ? -0.5f : 0.5f);
		//return;
	}
	float speed = cam.speed() * deltaTime;
	if (ImGui::IsKeyPressed(ImGuiKey_W)) {
		cam.eye() += speed * Linal::normalize(cam.direction());
		//return;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_S)) {
		cam.eye() += -speed * Linal::normalize(cam.direction());
		//return;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_A)) {
		cam.eye() += -speed * Linal::normalize(Linal::cross(cam.direction(), cam.up()));
		//return;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_D)) {
		cam.eye() += speed * Linal::normalize(Linal::cross(cam.direction(), cam.up()));
		//return;
	}
}
void BLEV::Interface::Canvas::ProcessCamMouseInput(ImVec2& deltaMouse, Camera& cam) {
	if (ImGui::IsKeyDown(ImGuiKey_C)) {
		ImVec2 offset = cam.sensitivity() * deltaMouse;

		cam.rotation().x += offset.x; // yaw
		cam.rotation().y -= offset.y; // pitch

		cam.rotation().y = std::min(cam.rotation().y, 89.0f);
		cam.rotation().y = std::max(cam.rotation().y, -89.0f);

		cam.updateDirection();
	}
}

void BLEV::Interface::Canvas::SwitchModes() {
	if (_data.chosenPrimEditMode != PrimEditMode::None) {
		switch (_data.chosenPrimEditMode) {
		case PrimEditMode::SelectPoints:
			if (is_hovered && !_data.adding_line && _data.chosen_prims.size() == 1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				Primitive* prim = *_data.chosen_prims.begin();
				size_t p_ind = prim->find_point(mouse_pos);
				if (p_ind != prim->size()) {
					if (_data.chosen_prim_points.find(&prim->at(p_ind)) == _data.chosen_prim_points.end()) {
						_data.chosen_prim_points.insert(&prim->at(p_ind));
					}
					else {
						_data.chosen_prim_points.erase(&prim->at(p_ind));
					}
					break;
				}
			}
			break;
		case PrimEditMode::MovePoints:
			if (is_hovered && !_data.adding_line && _data.chosen_prims.size() == 1 && _data.chosen_prim_points.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				Primitive* prim = *_data.chosen_prims.begin();
				size_t p_ind = prim->find_point(mouse_pos);
				if (p_ind != prim->size()) {
					point_of_transformation = &prim->at(p_ind);
					_data.adding_line = FirstClick;
					break;
				}
			}
			if (_data.adding_line == FirstClick && point_of_transformation != nullptr) {
				ImVec2 d = *point_of_transformation - mouse_pos;
				std::for_each(_data.chosen_prim_points.begin(), _data.chosen_prim_points.end(), [&d](ImVec2* iv) { auto newPos = *iv - d; (*iv).x = newPos.x; (*iv).y = newPos.y; });
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					point_of_transformation = nullptr;
					_data.adding_line = None;
				}
			}
			break;
		case PrimEditMode::SelectEdges:
			if (is_hovered && !_data.adding_line && _data.chosen_prims.size() == 1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				Primitive* prim = *_data.chosen_prims.begin();
				size_t e_ind = prim->find_edge(mouse_pos);
				if (e_ind != prim->size()) {
					auto pr = std::pair<ImVec2*, ImVec2*>(&prim->at(e_ind), &prim->at((e_ind + 1) % prim->size())); // e_ind == prim->size() - 1 if we found "_connect_bounds" edge
					if (_data.chosen_prim_edges.find(pr) == _data.chosen_prim_edges.end()) {
						_data.chosen_prim_edges.insert(pr);
					}
					else {
						_data.chosen_prim_edges.erase(pr);
					}
					break;
				}
			}
			break;
		case PrimEditMode::MoveEdges:
			if (is_hovered && !_data.adding_line && _data.chosen_prims.size() == 1 && _data.chosen_prim_edges.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				Primitive* prim = *_data.chosen_prims.begin();
				size_t e_ind = prim->find_edge(mouse_pos);
				if (e_ind != prim->size()) {
					point_of_transformation = &prim->at(e_ind);
					prev_point = mouse_pos;
					_data.adding_line = FirstClick;
					break;
				}
			}
			if (_data.adding_line == FirstClick && point_of_transformation != nullptr) {
				ImVec2 d = prev_point - mouse_pos;
				prev_point = mouse_pos;
				auto touched_prim = *_data.chosen_prims.begin();
				std::unordered_set<ImVec2*> vertices;
				for (auto e_it = _data.chosen_prim_edges.begin(); e_it != _data.chosen_prim_edges.end(); e_it++) {
					if (vertices.find((*e_it).first) == vertices.end()) {
						auto newStart = *(*e_it).first - d;
						(*(*e_it).first).x = newStart.x; (*(*e_it).first).y = newStart.y;
						vertices.insert((*e_it).first);
					}
					if (vertices.find((*e_it).second) == vertices.end()) {
						auto newStart = *(*e_it).second - d;
						(*(*e_it).second).x = newStart.x; (*(*e_it).second).y = newStart.y;
						vertices.insert((*e_it).second);
					}
				}
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					prev_point = ImVec2();
					point_of_transformation = nullptr;
					_data.adding_line = None;
				}
			}
			break;
		default:
			break;
		}
	}
	else
		switch (_data.chosenMode)
		{
		case Mode::Point:
			if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				_data.primitives.push_back(new Point(mouse_pos, GetColorFlU32(curr_color), thickness));
			}
			break;
		case Mode::Edge:
			if (is_hovered && !_data.adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				_data.new_prim = new Edge(mouse_pos, mouse_pos, GetColorFlU32(curr_color), thickness);
				_data.adding_line = FirstClick;
			}
			if (_data.adding_line == FirstClick) {
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					_data.adding_line = ReleasedState;
				}
			}
			if (_data.adding_line == ReleasedState)
			{
				(*_data.new_prim)[1] = mouse_pos;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					_data.adding_line = None;
					_data.primitives.push_back(_data.new_prim);
					_data.new_prim = NULL;
				}
			}
			break;
		case Mode::Polygon:
			if (is_hovered && !_data.adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				_data.new_prim = new Primitive(GetColorFlU32(curr_color), thickness);
				_data.new_prim->push_back(mouse_pos);
				_data.new_prim->push_back(mouse_pos);
				_data.adding_line = FirstClick;
			}
			if (_data.adding_line == FirstClick) {
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					_data.adding_line = ReleasedState;
				}
			}
			if (_data.adding_line == ReleasedState)
			{
				(*_data.new_prim).back() = mouse_pos;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					_data.adding_line = FirstClick;
					_data.new_prim->push_back(mouse_pos);
				}
			}
			if (_data.adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				_data.adding_line = None;
				_data.new_prim->pop_back();
				_data.primitives.push_back(_data.new_prim->size() == 1 ? new Point(_data.new_prim->at(0), _data.new_prim->color(), _data.new_prim->thickness()) : _data.new_prim);
				_data.new_prim = NULL;
			}
			break;
		case Mode::BezierCurve:
			if (is_hovered && !_data.adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				_data.new_prim = new BezierCurve(GetColorFlU32(curr_color), thickness);
				_data.new_prim->push_back(mouse_pos);
				_data.new_prim->push_back(mouse_pos);
				_data.adding_line = FirstClick;
			}
			if (_data.adding_line == FirstClick) {
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					_data.adding_line = ReleasedState;
				}
			}
			if (_data.adding_line == ReleasedState)
			{
				(*_data.new_prim).back() = mouse_pos;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					_data.adding_line = FirstClick;
					_data.new_prim->push_back(mouse_pos);
				}
			}
			if (_data.adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				_data.adding_line = None;
				_data.new_prim->pop_back();
				_data.primitives.push_back(_data.new_prim);
				_data.new_prim = NULL;
			}
			break;
		case Mode::Select:
			if (is_hovered && !_data.adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				for (int i = 0; i < _data.primitives.size(); i++)
				{
					size_t e_ind = _data.primitives[i]->find_edge(mouse_pos);
					if (e_ind != _data.primitives[i]->size()) {
						if (_data.chosen_prims.find(_data.primitives[i]) == _data.chosen_prims.end()) {
							_data.chosen_prims.insert(_data.primitives[i]);
						}
						else {
							_data.chosen_prims.erase(_data.primitives[i]);
						}
						break;
					}
				}
			}
			break;
		case Mode::FreeMove:
			if (is_hovered && !_data.adding_line && _data.chosen_prims.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				for (auto prim_it = _data.chosen_prims.begin(); prim_it != _data.chosen_prims.end(); prim_it++) {
					Primitive* prim = *prim_it;
					size_t e_ind = prim->find_edge(mouse_pos);
					if (e_ind != prim->size()) {
						point_of_transformation = &prim->at(e_ind);
						prev_point = mouse_pos;
						_data.adding_line = FirstClick;
						break;
					}
				}
			}
			if (_data.adding_line == FirstClick && point_of_transformation != nullptr) {
				ImVec2 d = prev_point - mouse_pos;
				prev_point = mouse_pos;
				std::for_each(_data.chosen_prims.begin(), _data.chosen_prims.end(), [&d](Primitive* prim) { prim->translate(&d); });
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					prev_point = ImVec2();
					point_of_transformation = nullptr;
					_data.adding_line = None;
				}
			}
			break;
		default:
			break;
		}

}
void BLEV::Interface::Canvas::DrawObjects() {
	for (size_t i = 0; i < _data.primitives.size(); i++) {
		_data.primitives[i]->draw(draw_list,
			origin,
			_data.chosen_prims.find(_data.primitives[i]) == _data.chosen_prims.end() ? _data.primitives[i]->color() : IM_COL32(255, 0, 0, 255));
	}

	for (size_t i = 0; i < _data.fractals.size(); i++) {
		_data.fractals[i]->draw(draw_list, origin);
	}

	if (_data.new_prim) {
		_data.new_prim->draw_previe(draw_list, origin);
	}

	for (auto che_it = _data.chosen_prim_edges.begin(); che_it != _data.chosen_prim_edges.end(); ++che_it) {
		auto ch_e = *che_it;
		draw_list->AddLine(*ch_e.first + origin, *ch_e.second + origin, IM_COL32(0, 255, 0, 255), (*_data.chosen_prims.begin())->thickness());
	}

	// thickness 3.f doesnt git with very thick lines obviously. would be useful if chosen point knew its prim thickness at least
	for (auto chp_it = _data.chosen_prim_points.begin(); chp_it != _data.chosen_prim_points.end(); ++chp_it) {
		ImVec2* ch_p = *chp_it;
		draw_list->AddCircleFilled(*ch_p + origin, (*_data.chosen_prims.begin())->thickness() + 2.f, IM_COL32(0, 255, 0, 255), 10);
	}

	for (auto mesh : _data.meshes) {
		mesh->draw(draw_list, origin, vp);
	}
}
void BLEV::Interface::Canvas::PollCallbacks() {
	for (size_t i = 0; i < hotkeysSize; i++) {
		if (funcs::IsLegacyNativeDupe(hotkeys[i])) continue;
		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			if (_data.chosen_prim_points.size() > 0 || _data.chosen_prim_edges.size() > 0) { // not sure whether it is comfortable for user. considering your suggestions on the ux
				_data.chosen_prim_points.clear();
				_data.chosen_prim_edges.clear();
				break;
			}
			if (_data.chosen_prim_points.size() == 0 || _data.chosen_prim_edges.size() == 0) { // not sure whether it is comfortable for user. considering your suggestions on the ux
				_data.chosenPrimEditMode = PrimEditMode::None;
				break;
			}
			_data.chosen_prims.clear();
			_data.chosenPrimEditMode = PrimEditMode::None;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_M)) {
			_data.chosenMode = Mode::FreeMove;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_P)) {
			_data.chosenMode = Mode::Point;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_E)) {
			_data.chosenMode = Mode::Edge;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_G)) {
			_data.chosenMode = Mode::Polygon;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_B)) {
			_data.chosenMode = Mode::BezierCurve;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_P)) {
			_data.chosenMode = Mode::Select;
			break;
		}
	}
}

void BLEV::Interface::Canvas::ShowContextMenu()
{
	if (!_data.adding_line && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopup("context")) {
			if (_data.chosen_prims.size() == 1) {
				Primitive* prim = *_data.chosen_prims.begin();
				ImGui::TextColored(ImVec4(100.f, 100.f, 100.f, 255), "%d-gon figure", prim->size());

				ImGui::Separator();

				if (ImGui::MenuItem("Cancel selection", NULL, false, _data.chosen_prims.size() == 1)) {
					_data.chosen_prims.erase(prim);
					_data.chosenPrimEditMode = PrimEditMode::None;
					_data.chosen_prim_edges.clear();
					_data.chosen_prim_points.clear();
				}

				if (ImGui::MenuItem("Delete object", NULL, false, _data.chosen_prims.size() == 1)) {
					_data.chosen_prims.erase(prim);
					_data.primitives.erase(std::remove(_data.primitives.begin(), _data.primitives.end(), prim), _data.primitives.end());
					_data.chosenPrimEditMode = PrimEditMode::None;
					_data.chosen_prim_edges.clear();
					_data.chosen_prim_points.clear();

				}

				if (ImGui::BeginMenu("Points...")) {
					if (ImGui::MenuItem("Select", NULL, false, _data.chosen_prims.size() == 1)) {
						if (_data.chosenPrimEditMode == PrimEditMode::SelectPoints) {
							_data.chosenPrimEditMode = PrimEditMode::None;
						}
						else {
							_data.chosenPrimEditMode = PrimEditMode::SelectPoints;
						}
					}
					if (ImGui::MenuItem("Move", NULL, false, _data.chosen_prims.size() == 1 && _data.chosen_prim_points.size() > 0)) {
						if (_data.chosenPrimEditMode == PrimEditMode::MovePoints) {
							_data.chosenPrimEditMode = PrimEditMode::None;
						}
						else {
							_data.chosenPrimEditMode = PrimEditMode::MovePoints;
						}
					}
					if (ImGui::MenuItem("Delete", NULL, false, _data.chosen_prims.size() == 1 && _data.chosen_prim_points.size() > 0)) {
						Primitive* prim = *_data.chosen_prims.begin();
						if (prim->size() == _data.chosen_prim_points.size()) {
							_data.chosen_prim_points.clear();
							_data.chosen_prims.erase(prim);
							_data.primitives.erase(std::remove(_data.primitives.begin(), _data.primitives.end(), prim), _data.primitives.end());
							if (_data.chosen_prims.size() == 0) {
								_data.chosenPrimEditMode = PrimEditMode::None;
							}
						}
						int offset = 0;
						while (_data.chosen_prim_points.size() > 0) {
							ImVec2* iv = *_data.chosen_prim_points.begin();
							_data.chosen_prim_points.erase(_data.chosen_prim_points.begin());

							try {
								prim->pop(iv - offset++);
							}
							catch (std::exception e) {
								//console[0]->feedback = e.what();
								//console[0]->feedback_color = ImVec4(255, 0, 0, 255);
							}
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edges...")) {
					if (ImGui::MenuItem("Select", NULL, false, _data.chosen_prims.size() == 1)) {
						if (_data.chosenPrimEditMode == PrimEditMode::SelectEdges) {
							_data.chosenPrimEditMode = PrimEditMode::None;
						}
						else {
							_data.chosenPrimEditMode = PrimEditMode::SelectEdges;
						}
					}
					if (ImGui::MenuItem("Move", NULL, false, _data.chosen_prims.size() == 1 && _data.chosen_prim_edges.size() > 0)) {
						if (_data.chosenPrimEditMode == PrimEditMode::MoveEdges) {
							_data.chosenPrimEditMode = PrimEditMode::None;
						}
						else {
							_data.chosenPrimEditMode = PrimEditMode::MoveEdges;
						}
					}
					if (ImGui::MenuItem("Delete", NULL, false, false/* chosen_prims.size() == 1 && chosen_prim_edges.size() > 0*/)) {
						//std::cout << "(deleting edges ... not implemented yet)\n";
						/*
						Primitive* prim = *chosen_prims.begin();
						if (prim->size() == chosen_prim_points.size()) {
							chosen_prim_points.clear();
							chosen_prims.erase(prim);
							primitives.erase(std::remove(primitives.begin(), primitives.end(), prim), primitives.end());
							if (chosen_prims.size() == 0) {
								chosenPrimEditMode = (int)PrimEditMode::None;
							}
						}
						int offset = 0;
						while (chosen_prim_points.size() > 0) {
							ImVec2* iv = *chosen_prim_points.begin();
							chosen_prim_points.erase(chosen_prim_points.begin());

							try {
								prim->pop(iv - offset++);
							}
							catch (std::exception e) {
								console[0]->feedback = e.what();
								console[0]->feedback_color = ImVec4(255, 0, 0, 255);
							}
						}*/
					}
					ImGui::EndMenu();
				}

				ImGui::Separator();
			}

			if (ImGui::MenuItem("Remove last prim", NULL, false, _data.primitives.size() > 0)) {
				_data.chosen_prims.erase(_data.primitives.back());
				if (_data.primitives.back() == &_data.prev_displacement)
				{
					_data.prev_displacement.clear();
					_data.curr_displacement.clear();
				}
				_data.primitives.pop_back();
				if (_data.chosen_prims.size() == 0) {
					_data.chosenPrimEditMode = PrimEditMode::None;
					_data.chosen_prim_points.clear();
					_data.chosen_prim_edges.clear();
				}
			}
			if (ImGui::MenuItem("Remove last fractal", NULL, false, _data.fractals.size() > 0)) {
				_data.chosen_lsys.erase(_data.fractals.back());
				_data.fractals.pop_back();
			}
			if (ImGui::MenuItem("Remove all", NULL, false, _data.primitives.size() + _data.fractals.size() + _data.meshes.size() > 0)) {
				_data.chosen_prims.clear();
				_data.chosen_prim_points.clear();
				_data.chosen_prim_edges.clear();
				_data.primitives.clear();
				_data.fractals.clear();
				_data.chosen_lsys.clear();
				_data.prev_displacement.clear();
				_data.curr_displacement.clear();
				_data.meshes.clear();
				_data.chosenPrimEditMode = PrimEditMode::None;
			}
			ImGui::EndPopup();
		}
	}
}

void BLEV::Interface::Canvas::Draw2dGrid()
{
	for (float x = fmodf(scrolling.x, GRID_STEP); x < size.x; x += GRID_STEP)
		draw_list->AddLine(ImVec2(p[0].x + x, p[0].y), ImVec2(p[0].x + x, p[1].y), IM_COL32(200, 200, 200, 40));
	for (float y = fmodf(scrolling.y, GRID_STEP); y < size.y; y += GRID_STEP)
		draw_list->AddLine(ImVec2(p[0].x, p[0].y + y), ImVec2(p[1].x, p[0].y + y), IM_COL32(200, 200, 200, 40));
}
void BLEV::Interface::Canvas::Draw3dGrid()
{
	auto border = 300.f;
	Line3d::draw(draw_list, ImVec3(0, 0.f, -border), ImVec3(0, 0.f, border), origin, vp, vis_p);
	Line3d::draw(draw_list, ImVec3(-border, 0.f, 0), ImVec3(border, 0.f, 0), origin, vp, vis_p);
	for (int i = 1; i < border / GRID_STEP; i++) {
		auto next = i * GRID_STEP;
		Line3d::draw(draw_list, ImVec3(next, 0.f, -border), ImVec3(next, 0.f, border), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-next, 0.f, -border), ImVec3(-next, 0.f, border), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-border, 0.f, next), ImVec3(border, 0.f, next), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-border, 0.f, -next), ImVec3(border, 0.f, -next), origin, vp, vis_p);
	}
}
void BLEV::Interface::Canvas::DrawAxis()
{
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(GRID_STEP * 3.f, 0.f, 0.f), origin, vp, VisualParams(IM_COL32(0, 255, 0, 255), 3.f, true));
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(0.f, GRID_STEP * 3.f, 0.f), origin, vp, VisualParams(IM_COL32(0, 0, 255, 255), 3.f, true));
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(0.f, 0.f, GRID_STEP * 3.f), origin, vp, VisualParams(IM_COL32(255, 0, 0, 255), 3.f, true));
}

/*
void BLEV::Interface::Canvas::Update() {
	size = ImGui::GetContentRegionAvail();
	if (size.x < MIN_WIDTH) size.x = MIN_WIDTH;
	if (size.y < MIN_HEIGHT) size.y = MIN_HEIGHT;

	ImGuiIO& io = ImGui::GetIO();
	draw_list = ImGui::GetWindowDrawList();

	vp = main_camera.viewProjecion(); //auto vp = main_camera.getProjection(); //auto vp = main_camera.getView();

	//is_hovered = true;
	is_hovered = ImGui::IsItemHovered();
	//is_active = true;
	is_active = ImGui::IsItemActive();

	p[0] = ImGui::GetCursorScreenPos();
	p[1] = ImVec2(p[0].x + size.x, p[0].y + size.y);

	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, b_context_menu_enabled ? -1.0f : 0.0f))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
		printf("x: %f, y: %f\n", io.MouseDelta.x, io.MouseDelta.y);
	}

	origin = ImVec2(p[0].x + scrolling.x, p[0].y + scrolling.y);

	PollCallbacks();

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	SwitchModes();

	if (is_hovered) {
		if (ImGui::IsKeyPressed(ImGuiKey_R)) {
			main_camera.resetFlightSettings();
			main_camera.resetCamPosition();
			main_camera.setPerspective();
		}
		ProcessCamKeyboardInput(io, main_camera, deltaTime);

		if (!main_camera.dirtiness())
		{
			prev_point = mouse_pos;
			main_camera.dirtiness() = true;
		}
		deltaMouse = mouse_pos - prev_point;
		prev_point = mouse_pos;
		ProcessCamMouseInput(deltaMouse, main_camera);

		main_camera.updateLook();
	}

	drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
}
*/

void BLEV::Interface::Canvas::Body() {
	if (ImGui::BeginChild("Canvas")) {

		p[0] = ImGui::GetCursorScreenPos();
		size = ImGui::GetContentRegionAvail();
		if (size.x < MIN_WIDTH) size.x = MIN_WIDTH;
		if (size.y < MIN_HEIGHT) size.y = MIN_HEIGHT;
		p[1] = ImVec2(p[0].x + size.x, p[0].y + size.y);
		
		ImGuiIO& io = ImGui::GetIO();
		draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(p[0], p[1], IM_COL32(50, 50, 50, 255));
		draw_list->AddRect(p[0], p[1], IM_COL32(255, 255, 255, 255));
		
		ImGui::InvisibleButton("canvas", size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		is_hovered = ImGui::IsItemHovered();
		is_active = ImGui::IsItemActive();
		origin = ImVec2(p[0].x + scrolling.x, p[0].y + scrolling.y);
		mouse_pos = ImVec2(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

		PollCallbacks();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		if (is_hovered) {
			if (ImGui::IsKeyPressed(ImGuiKey_R)) {
				main_camera.resetFlightSettings();
				main_camera.resetCamPosition();
				main_camera.setPerspective();
			}
			ProcessCamKeyboardInput(io, main_camera, deltaTime);

			if (!main_camera.dirtiness())
			{
				prev_point = mouse_pos;
				main_camera.dirtiness() = true;
			}
			deltaMouse = mouse_pos - prev_point;
			prev_point = mouse_pos;
			ProcessCamMouseInput(deltaMouse, main_camera);

			main_camera.updateLook();
		}

		SwitchModes();

		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, b_context_menu_enabled ? -1.0f : 0.0f))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}

		drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		
		if (b_context_menu_enabled) ShowContextMenu();

		draw_list->PushClipRect(p[0], p[1], true);

		vp = main_camera.viewProjecion(); //auto vp = main_camera.getProjection(); //auto vp = main_camera.getView();
		
		if(b_grid_2d_enabled) Draw2dGrid();
		if(b_grid_3d_enabled) Draw3dGrid();
		DrawObjects();
		if (b_grid_3d_enabled) DrawAxis();

		/* intersections
		if (chosen_prims.size() > 0) {
				intersections = get_intersections(chosen_prims);
				for (size_t i = 0; i < intersections.Size; i++)
				{
					draw_list->AddCircleFilled(*intersections[i] + origin, 2.f, IM_COL32(255, 0, 0, 255), 10);
				}
			}
			if (new_prim != NULL && primitives.size() > 0) {
				intersections = get_intersections(new_prim, primitives);
				for (size_t i = 0; i < intersections.Size; i++)
				{
					draw_list->AddCircleFilled(*intersections[i] + origin, 2.f, IM_COL32(0, 0, 255, 255), 10);
				}
			}
		*/

		draw_list->PopClipRect();

		ImGui::EndChild();
	}
}

void BLEV::Interface::Canvas::Show()
{
	Body();
}
