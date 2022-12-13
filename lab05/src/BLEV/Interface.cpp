#define _CRT_SECURE_NO_WARNINGS
#include "BLEV/Interface.h"
#include <unordered_set>
#include <windows.h>
#include <commdlg.h>
#include "stuff/NFD/nfd.h"
#include <algorithm>
#include "BLEV/Display.h"

const std::vector<BLEV::Interface::ready_l_system*> BLEV::Interface::ready_l_systems{
	//Кривая Коха
	new ready_l_system("Koch curve", "F", PI / 3.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F-F++F-F") }, ""),
	//Квадратный остров Коха
	new ready_l_system("Koch island", "F+F+F+F", PI / 2.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F+F-F-FF+F+F-F") }, ""),
	//Ковёр Серпинского
	new ready_l_system("Sierpinski carpet", "FXF--FF--FF", PI / 3.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "FF"), std::make_pair('X', "--FXF++FXF++FXF--") }, ""),
	//Наконечник Серпинского(треугольник)
	new ready_l_system("Sierpinski triangle", "YF", PI / 3.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F"), std::make_pair('X', "YF+XF+Y"), std::make_pair('Y', "XF-YF-X") }, ""),
	//Кривая Гильберта
	new ready_l_system("Hilbert curve", "X", PI / 2.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F"), std::make_pair('X', "-YF+XFX+FY-"), std::make_pair('Y', "+XF-YFY-FX+") }, ""),
	//Кривая дракона Хартера - Хейтуэя
	new ready_l_system("Dragon curve", "X", PI / 2.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F"), std::make_pair('X', "X+YF+"), std::make_pair('Y', "-FX-Y") }, ""),
	//Шестиугольная кривая Госпера
	new ready_l_system("Gosper curve", "XF", PI / 3.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F"), std::make_pair('X', "X+YF++YF-FX--FXFX-YF+"), std::make_pair('Y', "-FX+YFYF++YF+FX--FX-Y") }, ""),
	new ready_l_system("Tree 1", "X", PI / 9.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "FF"), std::make_pair('X', "F[+X]F[-X]+X") }, ""),
	new ready_l_system("Shrub 1", "F", PI / 90.f * 11.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "FF-[-F+F+F]+[+F-F-F]") }, ""),
	new ready_l_system("Shrub 2", "X", PI / 9.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "FF"), std::make_pair('X', "F[+X]F[-X]+X") }, ""),
	new ready_l_system("Shrub 3", "X", PI / 8.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "FF"), std::make_pair('X', "F-[[X]+X]+F[+FX]-X") }, ""),
	//Шестиугольная мозаика
	new ready_l_system("Hexagonal Mosaic", "X", PI / 3.f, std::deque<std::pair<char, std::string>>{ std::make_pair('F', "F"), std::make_pair('X', "[-F+F[Y]+F][+F-F[X]-F]"), std::make_pair('Y', "[-F+F[Y]+F][+F-F-F]") }, ""),
	new ready_l_system("\"Random\" tree", "X", PI / 4.f, std::deque<std::pair<char, std::string>>{ std::make_pair('X', "F[@[-X]+X]") }, "F"),
};

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
static void HelpPrevItem(const char* desc)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

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

	ImGui::SameLine();

	if (ImGui::Button("X")) {
		try {
			char* nstr = console[0]->pseudo_console; float angle;
			if (sscanf(nstr, "%f", &angle) != 1) throw std::invalid_argument("Incorrect arguments format for rotate N");
			console[0]->feedback = "";
			auto lammy = [&angle](Mesh* m) { m->rotateX(angle); };
			std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Y")) {
		try {
			char* nstr = console[0]->pseudo_console; float angle;
			if (sscanf(nstr, "%f", &angle) != 1) throw std::invalid_argument("Incorrect arguments format for rotate N");
			console[0]->feedback = "";
			auto lammy = [&angle](Mesh* m) { m->rotateY(angle); };
			std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Z")) {
		try {
			char* nstr = console[0]->pseudo_console; float angle;
			if (sscanf(nstr, "%f", &angle) != 1) throw std::invalid_argument("Incorrect arguments format for rotate N");
			console[0]->feedback = "";
			auto lammy = [&angle](Mesh* m) { m->rotateZ(angle); };
			std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("U")) {
		try {
			char* nstr = console[0]->pseudo_console; float x0, y0, z0, x1, y1, z1, angle;
			if (sscanf(nstr, "%f%*c%f%*c%f%*c%f%*c%f%*c%f%*c%f%", &x0, &y0, &z0, &x1, &y1, &z1, &angle) != 7) throw std::invalid_argument("Incorrect arguments format for rotate N");
			console[0]->feedback = "";
			auto p0 = ImVec3(x0, y0, z0);
			auto p1 = ImVec3(x1, y1, z1);
			_data.rotate_axis = new Line3d(std::move(p0), std::move(p1), true);
			auto lammy = [&p0, &p1, &angle](Mesh* m) { m->rotateU(p0, p1, angle); };
			std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
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
	HelpPrevItem("dx dy [dz]");

	if (ImGui::Button("Translate")) {
		try {
			char* nstr = console[1]->pseudo_console; float dx, dy, dz;
			if (sscanf(nstr, "%f%*c%f%*c%f", &dx, &dy, &dz) == 3)
			{
				console[1]->feedback = "";
				auto d = ImVec2(-1 * dx, -1 * dy);
				auto lammy = [&dx, &dy, &dz](Mesh* m) { m->translate(dx, dy, dz); };

				if (_data.chosen_meshes.size() == 0) {
					throw std::invalid_argument("The number of selected objects must be at least 1");
				}
				std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
			}
			else if (sscanf(nstr, "%f%*c%f", &dx, &dy) == 2)
			{
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
			else
				throw std::invalid_argument("Incorrect arguments format for translate");
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
	HelpPrevItem("sX sY [sZ]");

	if (ImGui::Button("Scale")) {
		try {
			char* nstr = console[2]->pseudo_console; float scaleX, scaleY, scaleZ;
			if (sscanf(nstr, "%f%*c%f%*c%f", &scaleX, &scaleY, &scaleZ) == 3)
			{
				if (scaleX <= 0 || scaleY <= 0 || scaleZ <= 0) throw std::invalid_argument("Incorrect arguments format for scale");
				console[2]->feedback = "";
				auto lammy = [&scaleX, &scaleY, &scaleZ](Mesh* m) { m->scale(scaleX, scaleY, scaleZ); };
				std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
			}
			else if (sscanf(nstr, "%f%*c%f", &scaleX, &scaleY) == 2)
			{
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
			else
				throw std::invalid_argument("Incorrect arguments format for scale");
		}
		catch (std::exception& e) {
			console[2]->feedback = e.what();
			console[2]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}
void BLEV::Interface::F_Reflect() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	if (ImGui::Button("YZ"))
	{
		auto lammy = [](Mesh* m) { m->reflectX(); };
		std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
	}
	ImGui::SameLine();
	if (ImGui::Button("XZ"))
	{
		auto lammy = [](Mesh* m) { m->reflectY(); };
		std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
	}
	ImGui::SameLine();
	if (ImGui::Button("XY"))
	{
		auto lammy = [](Mesh* m) { m->reflectZ(); };
		std::for_each(_data.chosen_meshes.begin(), _data.chosen_meshes.end(), lammy);
	}
	ImGui::EndGroup();
}

void BLEV::Interface::F_Edit()
{
	F_Rotate();
	ImGui::Separator();
	F_Translate();
	ImGui::Separator();
	F_Scale();
	ImGui::Separator();
	F_Reflect();
	//ImGui::Separator();
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

void BLEV::Interface::F_Union() {
	ImGui::Text("Convex Polygons Union");
	if (ImGui::Button("Make union")) {
		try {
			if (_data.chosen_prims.size() != 2)
				throw std::invalid_argument("You should choose 2 polygons");
			std::vector<Primitive*> polygons;
			for (auto it = _data.chosen_prims.begin(); it != _data.chosen_prims.end(); ++it)
			{
				if ((*it)->size() < 3)
					throw std::invalid_argument("You can choose only polygons");
				polygons.push_back(*it);
			}
			auto vec = unionPolygons(polygons[0], polygons[1]);
			if (vec != nullptr)
			{
				Primitive* unionPrim = new Primitive(vec, polygons[0]->color(), polygons[0]->thickness());
				for (auto poly : polygons)
				{
					_data.chosen_prims.erase(poly);
					//_data.primitives.erase(std::find(_data.primitives.begin(), _data.primitives.end(), poly));
				}
				_data.chosen_prims.insert(unionPrim);
				_data.primitives.push_back(unionPrim);
			}
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

void BLEV::Interface::F_Camera() {
	if (ImGui::BeginTable("MainCamera##info_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit, ImVec2(400.f, 0.f)))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Camera position");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Camera rotation");
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("Camera target");

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("X: %f", canvas.main_camera.eye().x);
		ImGui::Text("Y: %f", canvas.main_camera.eye().y);
		ImGui::Text("Z: %f", canvas.main_camera.eye().z);

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("Pitch: %f", canvas.main_camera.pitchYawRoll().x);
		ImGui::Text("Yaw: %f", canvas.main_camera.pitchYawRoll().y);
		ImGui::Text("Roll: %f", canvas.main_camera.pitchYawRoll().z);

		ImGui::TableSetColumnIndex(2);
		ImGui::Text("X: %f", canvas.main_camera.target().x);
		ImGui::Text("Y: %f", canvas.main_camera.target().y);
		ImGui::Text("Z: %f", canvas.main_camera.target().z);

		ImGui::EndTable();
	}

	if (ImGui::Combo("projection", &canvas.main_camera.mode(), _data.cameraModes, _data.cameraModesSize)) {}
	switch ((Camera::CamMode)canvas.main_camera.mode()) {
	case Camera::Perspective:
		ImGui::DragFloat("zFocus", &canvas.main_camera.zFocus(), 1.f, 400.f, 1000.f, "%.0f");
		break;
	case Camera::Axonometry:
		ImGui::DragFloat("angleX", &canvas.main_camera.angleX(), 1.f, 0.f, 180.f, "%.0f");
		ImGui::DragFloat("angleY", &canvas.main_camera.angleY(), 1.f, 0.f, 360.f, "%.0f");
		break;
	default:
		break;
	}
}

void _quickhull_rec(Primitive& out, const std::vector<ImVec2>& vecs, const std::pair<ImVec2, ImVec2>& mm) {
	if (vecs.size() == 1) {
		out.push_back(vecs[0]);
		return;
	}
	ImVec2 m = vecs[0];
	float dist = distance_from_line(mm.first, mm.second, m);
	for (auto& vec : vecs) {
		float temp = distance_from_line(mm.first, mm.second, vec);
		if (temp > dist) {
			m = vec;
			dist = std::move(temp);
		}
	}
	auto mm1 = std::make_pair(m, mm.second);
	std::vector<ImVec2> rhs;
	std::for_each(std::begin(vecs), std::end(vecs), [&](const ImVec2& p) { if (pointPositionWithEdge(mm1.first, mm1.second, p)) rhs.push_back(p); });
	if (!rhs.empty()) _quickhull_rec(out, rhs, mm1);
	out.push_back(mm1.first);

	auto mm2 = std::make_pair(mm.first, m);
	std::vector<ImVec2> lhs;
	std::for_each(std::begin(vecs), std::end(vecs), [&](const ImVec2& p) { if (pointPositionWithEdge(mm2.first, mm2.second, p)) lhs.push_back(p); });
	if (!lhs.empty()) _quickhull_rec(out, lhs, mm2);
	//out.push_back(mm2.first);
}

void BLEV::Interface::F_QuickHull()
{
	ImGui::Text("QuickHull");
	if (ImGui::Button("Build a shell")) {
		Primitive prim(VisualParams().color, VisualParams().thickness);
		std::vector<Primitive*> points;
		std::vector<ImVec2> vecs;
		std::for_each(_data.chosen_prims.begin(), _data.chosen_prims.end(), [&](Primitive* pr) {
			if (dynamic_cast<Point*>(pr) != NULL) {
				vecs.push_back(dynamic_cast<Point*>(pr)->get());
			}
			});

		auto mm = std::make_pair(vecs[0], vecs[0]);
		for (auto& vec : vecs) {
			if (mm.first.x > vec.x) mm.first = vec;
			if (mm.second.x < vec.x) mm.second = vec;
		}

		std::vector<ImVec2> lhs;
		std::vector<ImVec2> rhs;
		std::for_each(std::begin(vecs), std::end(vecs), [&](const ImVec2& p) {
			if (pointPositionWithEdge(mm.first, mm.second, p))
			lhs.push_back(p);
			else
				rhs.push_back(p);
			});

		prim.push_back(mm.second);
		if (!lhs.empty()) _quickhull_rec(prim, lhs, mm);
		prim.push_back(mm.first);
		if (!rhs.empty()) _quickhull_rec(prim, rhs, mm);

		_data.primitives.push_back(new Primitive(std::move(prim)));
	}
}

void BLEV::Interface::F_Present() {
	ImGui::Text("Jarvis (Present packing)");
	if (ImGui::Button("Pack a present!")) {
		try {
			if (_data.chosen_prims.size() < 2) throw std::invalid_argument("You should choose no less than 2 points");

			Primitive* current_point = *_data.chosen_prims.begin();
			if (dynamic_cast<Point*>(current_point) == nullptr) throw std::invalid_argument("You should only choose points");

			for (auto it = std::next(_data.chosen_prims.begin()); it != _data.chosen_prims.end(); ++it)
			{
				Primitive* prim = *it;
				if (dynamic_cast<Point*>(prim) == nullptr) throw std::invalid_argument("You should only choose points");

				if ((prim->front().y > current_point->front().y) || (prim->front().y == current_point->front().y) && (prim->front().x >= current_point->front().x)) {
					current_point = prim;
				}
			}
			Primitive* present = packPresent(dynamic_cast<Point*>(current_point), _data.chosen_prims, VisualParams().color, VisualParams().thickness);
			_data.chosen_prims.clear();
			_data.primitives.push_back(present);
		}
		catch (std::exception& e) {
			//console[0]->feedback = e.what();
			//console[0]->feedback_color = ImVec4(255, 0, 0, 255);
			//best error output in the world.
		}
	}
}

void BLEV::Interface::F_Shells()
{
	F_QuickHull();
	ImGui::Separator();
	F_Present();
	ImGui::Separator();
	F_Union();
}

void BLEV::Interface::F_RotationBody() {
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleDisplace", console[3]->pseudo_console, 100);
	ImGui::EndGroup();
	HelpPrevItem("Number of partitions");

	if (ImGui::Button("X")) {
		try {
			if (_data.chosen_prims.size() != 1)
				throw std::invalid_argument("You should choose one polygon");
			auto prim = *_data.chosen_prims.begin();
			if (prim->size() < 2)
				throw std::invalid_argument("Polygon should contain at least 2 points");
			char* nstr = console[3]->pseudo_console;
			int iterNumber;
			if (sscanf(nstr, "%d", &iterNumber) != 1)
				throw std::invalid_argument("Incorrect arguments format");
			if (iterNumber < 1)
				throw std::invalid_argument("Number of partitions must be positive");
			console[3]->feedback = "";
			auto body = new RotationBody(prim, iterNumber, Axis::X);
			_data.chosen_prims.clear();
			_data.primitives.erase(std::find(_data.primitives.begin(), _data.primitives.end(), prim));
			_data.meshes.push_back(body);
			_data.chosen_meshes.insert(body);
		}
		catch (std::exception& e) {
			console[3]->feedback = e.what();
			console[3]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Y")) {
		try {
			if (_data.chosen_prims.size() != 1)
				throw std::invalid_argument("You should choose one polygon");
			auto prim = *_data.chosen_prims.begin();
			if (prim->size() < 2)
				throw std::invalid_argument("Polygon should contain at least 2 points");
			char* nstr = console[3]->pseudo_console;
			int iterNumber;
			if (sscanf(nstr, "%d", &iterNumber) != 1)
				throw std::invalid_argument("Incorrect arguments format");
			if (iterNumber < 1)
				throw std::invalid_argument("Number of partitions must be positive");
			console[3]->feedback = "";
			auto body = new RotationBody(prim, iterNumber, Axis::Y);
			_data.chosen_prims.clear();
			_data.primitives.erase(std::find(_data.primitives.begin(), _data.primitives.end(), prim));
			_data.meshes.push_back(body);
			_data.chosen_meshes.insert(body);
		}
		catch (std::exception& e) {
			console[3]->feedback = e.what();
			console[3]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Z")) {
		try {
			if (_data.chosen_prims.size() != 1)
				throw std::invalid_argument("You should choose one polygon");
			auto prim = *_data.chosen_prims.begin();
			if (prim->size() < 2)
				throw std::invalid_argument("Polygon should contain at least 2 points");
			char* nstr = console[3]->pseudo_console;
			int iterNumber;
			if (sscanf(nstr, "%d", &iterNumber) != 1)
				throw std::invalid_argument("Incorrect arguments format");
			if (iterNumber < 1)
				throw std::invalid_argument("Number of partitions must be positive");
			console[3]->feedback = "";
			auto body = new RotationBody(prim, iterNumber, Axis::Z);
			_data.chosen_prims.clear();
			_data.primitives.erase(std::find(_data.primitives.begin(), _data.primitives.end(), prim));
			_data.meshes.push_back(body);
			_data.chosen_meshes.insert(body);
		}
		catch (std::exception& e) {
			console[3]->feedback = e.what();
			console[3]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	if (!console[3]->feedback.empty()) {
		ImGui::TextColored(console[3]->feedback_color, console[3]->feedback.c_str());
	}
}

void BLEV::Interface::F_MeshGraph() {
	float x0, x1, z0, z1; int x_parts, z_parts;
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleRanges", console[4]->pseudo_console, 100);
	HelpPrevItem("x0 x1 z0 z1 x_parts z_parts");

	static int chosen3dFuncType = 0;
	ImGui::Combo("##", &chosen3dFuncType, _data.funcs3dTypes, _data.funcs3dSize);

	ImGui::EndGroup();


	if (ImGui::Button("Create mesh graph")) {
		try
		{
			char* nstr = console[4]->pseudo_console;
			Validator::ValidateMeshGraphArgs(nstr, x0, x1, z0, z1, x_parts, z_parts);
			_data.meshes.push_back(new MeshGraph(x0, x1, z0, z1, x_parts, z_parts, _data.meshGraphFuncs[chosen3dFuncType]));
			//console[4]->feedback = ""; // left it be cuz dont wanna to retype the params over and over for multiple meshgraphs
		}
		catch (const std::exception& e)
		{
			console[4]->feedback = e.what();
			console[4]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	if (!console[4]->feedback.empty()) {
		ImGui::TextColored(console[4]->feedback_color, console[4]->feedback.c_str());
	}
}

void BLEV::Interface::F_FloatingHorizon() {
	float x0, x1, z0, z1; int x_parts, z_parts;
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleRanges", console[5]->pseudo_console, 100);
	HelpPrevItem("x0 x1 z0 z1 x_parts z_parts");

	static int chosenHorizonType = 0;
	ImGui::Combo("##", &chosenHorizonType, _data.funcs3dTypes, _data.funcs3dSize);

	ImGui::EndGroup();


	if (ImGui::Button("Create horizon")) {
		try
		{
			char* nstr = console[5]->pseudo_console;
			Validator::ValidateMeshGraphArgs(nstr, x0, x1, z0, z1, x_parts, z_parts);
			_data.horizons.push_back(new FloatingHorizon(x0, x1, z0, z1, x_parts, z_parts, _data.meshGraphFuncs[chosenHorizonType]));
		}
		catch (const std::exception& e)
		{
			console[5]->feedback = e.what();
			console[5]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	if (!console[5]->feedback.empty()) {
		ImGui::TextColored(console[5]->feedback_color, console[5]->feedback.c_str());
	}
}

void BLEV::Interface::F_Scene() {
	double x0, y0, z0, radius;
	ImGui::BeginGroup();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##ConsoleRanges", console[6]->pseudo_console, 100);
	HelpPrevItem("x0 y0 z0 radius");

	static ImVec3 sphereColor{ 0.4f, 0.6f, 0.8f };
	bool colorHasChanged = ImGui::ColorEdit3("", (float*)&sphereColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
	//auto lowerResult = sphereColor * 255;
	//if (colorHasChanged)
	//{
		//horizon->lowerColor = lowerResult;
		//needRefresh = true;
	//}

	ImGui::EndGroup();

	if (ImGui::Button("Create sphere")) {
		try
		{
			char* nstr = console[6]->pseudo_console;
			Validator::ValidateSphereArgs(nstr, x0, y0, z0, radius);
			_data.cringulik.scene.bodies.push_back(new Sphere(x0, y0, z0, radius, sphereColor));
			needRefresh = true;
			//printf("%lf %lf %lf %lf", x0, y0, z0, radius);
		}
		catch (const std::exception& e)
		{
			console[6]->feedback = e.what();
			console[6]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
	if (!console[6]->feedback.empty()) {
		ImGui::TextColored(console[6]->feedback_color, console[6]->feedback.c_str());
	}
}

void BLEV::Interface::ShowExternalWindows()
{
	if (bmo.b_edit_open) {
		if (ImGui::Begin("Rotate", &bmo.b_edit_open)) {
			F_Edit();
			ImGui::End();
		}
	}

	if (bmo.b_displace_open) {
		if (ImGui::Begin("Displace", &bmo.b_displace_open)) {
			F_Displace();
			ImGui::End();
		}
	}
	if (bmo.b_lsys_open) {
		if (ImGui::Begin("Lsystem", &bmo.b_lsys_open)) {
			F_Lsystem();
			ImGui::End();
		}
	}
	if (bmo.b_classify_open) {
		if (ImGui::Begin("Classify", &bmo.b_classify_open)) {
			F_Classify();
			ImGui::End();
		}
	}
	if (bmo.b_camera_open) {
		if (ImGui::Begin("Camera", &bmo.b_camera_open)) {
			F_Camera();
			ImGui::End();
		}
	}
	if (bmo.b_shells_open) {
		if (ImGui::Begin("Shells", &bmo.b_shells_open)) {
			F_Shells();
			ImGui::End();
		}
	}
	if (bmo.b_rotation_body_open) {
		if (ImGui::Begin("Rotation Body", &bmo.b_rotation_body_open)) {
			F_RotationBody();
			ImGui::End();
		}
	}
	if (bmo.b_mesh_graph_open) {
		if (ImGui::Begin("Mesh graph", &bmo.b_mesh_graph_open)) {
			F_MeshGraph();
			ImGui::End();
		}
	}
	if (bmo.b_floating_horizon_open) {
		if (ImGui::Begin("Floating Horizon", &bmo.b_floating_horizon_open)) {
			F_FloatingHorizon();
			ImGui::End();
		}
	}
	if (bmo.b_scene_open) {
		if (ImGui::Begin("Scene", &bmo.b_scene_open)) {
			F_Scene();
			ImGui::End();
		}
	}
}

void BLEV::Interface::PrepareCringetracer() {
	//_data.gbodies = &canvas.cringulik.scene.bodies;
	_data.cringulik.SetCamera(&canvas.main_camera);
	_data.cringulik.SetOMPThreads();
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

		obj_table.Show();

		ImGui::SameLine();
		canvas.Show();

		ImGui::End();
	}
}

void BLEV::Interface::Menu::ShowFileManagerMenu()
{
	if (ImGui::BeginMenu("File")) {
		if (ImGui::BeginMenu("Open")) {

			if (ImGui::MenuItem("Scene...", NULL, (bool*)0, false)) {
			}

			if (ImGui::MenuItem("Mesh...")) {
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPath);

				if (result == NFD_OKAY) {
					Mesh* m = new Mesh();
					m->open(outPath);
					_data.meshes.push_back(m);

					free(outPath);
				}
				else if (result == NFD_CANCEL) {
					puts("User pressed cancel.");
				}
				else {
					printf("Error: %s\n", NFD_GetError());
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Save")) {

			if (ImGui::MenuItem("Chosen meshes...")) {
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_SaveDialog("obj", NULL, &outPath);

				if (result == NFD_OKAY) {
					for (auto m : _data.chosen_meshes) {
						//m->save(outPath);
						Mesh::save(outPath, _data.chosen_meshes);
					}
					free(outPath);
				}
				else if (result == NFD_CANCEL) {
					puts("User pressed cancel.");
				}
				else {
					printf("Error: %s\n", NFD_GetError());
				}
			}
			if (ImGui::MenuItem("All...", NULL, (bool*)0, false)) {

			}

			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Load texture", NULL, (bool*)0, !_data.chosen_meshes.empty())) {
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("jpg", NULL, &outPath);

			if (result == NFD_OKAY) {
				for (auto m : _data.chosen_meshes) {
					m->loadImage(outPath);
				}
				needRefresh = true;
				free(outPath);
			}
			else if (result == NFD_CANCEL) {
				puts("User pressed cancel.");
			}
			else {
				printf("Error: %s\n", NFD_GetError());
			}

			//ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}


void BLEV::Interface::Menu::ShowModesMenu()
{
	static const char* shortcuts[6]{ "Ctrl+P", "Ctrl+E", "Ctrl+G", "Ctrl+B", "Ctrl+S", "Ctrl+M" };
	if (ImGui::BeginMenu("Mode")) {
		for (size_t i = 0; i < _data.modesSize; i++) {
			if (ImGui::MenuItem(_data.modesList[i], shortcuts[i], _data.chosenMode == (Mode)i)) {
				_data.chosenMode = (Mode)i;
			}
		}
		ImGui::EndMenu();
	}
}
void BLEV::Interface::Menu::ShowMethodsMenu(B_method_open& bmo)
{
	if (ImGui::BeginMenu("View..."))
	{
		if (ImGui::MenuItem("Edit", NULL, bmo.b_edit_open)) {
			bmo.b_edit_open = true;
		}
		if (ImGui::MenuItem("Displace", NULL, bmo.b_displace_open)) {

			bmo.b_displace_open = true;
		}
		if (ImGui::MenuItem("L-system", NULL, bmo.b_lsys_open)) {
			bmo.b_lsys_open = true;
		}
		if (ImGui::MenuItem("Classify", NULL, bmo.b_classify_open)) {

			bmo.b_classify_open = true;
		}
		if (ImGui::MenuItem("Camera", NULL, bmo.b_camera_open)) {
			bmo.b_camera_open = true;
		}
		if (ImGui::MenuItem("Shells", NULL, bmo.b_shells_open)) {
			bmo.b_shells_open = true;
		}
		if (ImGui::MenuItem("Rotation Body", NULL, bmo.b_rotation_body_open)) {
			bmo.b_rotation_body_open = true;
		}
		if (ImGui::MenuItem("Mesh Graph", NULL, bmo.b_mesh_graph_open)) {
			bmo.b_mesh_graph_open = true;
		}
		if (ImGui::MenuItem("Floating Horizon", NULL, bmo.b_floating_horizon_open)) {
			bmo.b_floating_horizon_open = true;
		}
		if (ImGui::MenuItem("Scene", NULL, bmo.b_scene_open)) {
			bmo.b_scene_open = true;
		}
		ImGui::EndMenu();
	}
}

void BLEV::Interface::Menu::ShowAddingMenu()
{
	if (ImGui::BeginMenu("Add.."))
	{
		if (ImGui::MenuItem("Point", NULL, (bool*)0, false)) {
			//_data.push_back(new Point3d(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Line", NULL, (bool*)0, false)) {
			//_data.meshes.push_back(new Line3d(ImVec3(-35.f, 30.f, -35.f), ImVec3(35.f, 30.f, 35.f)));
		}
		if (ImGui::MenuItem("Cube")) {
			_data.meshes.push_back(new Cube(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Tetrahedr")) {
			_data.meshes.push_back(new Tetrahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Octahedr")) {
			_data.meshes.push_back(new Octahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Dodecahedron")) {
			_data.meshes.push_back(new Dodecahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Icosahedron")) {
			_data.meshes.push_back(new Icosahedron(ImVec3(0.f, 30.f, 0.f)));
		}
		if (ImGui::MenuItem("Torch", "", false, _data.torch == nullptr)) {
			_data.torch = new Torch();
			_data.meshes.push_back(_data.torch);
		}
		ImGui::EndMenu();
	}
}

void BLEV::Interface::Menu::Show(B_method_open& bmo, Global_visual_params& gvp)
{
	if (ImGui::BeginMainMenuBar())
	{
		ShowFileManagerMenu();

		ShowModesMenu();

		ImGui::Separator();
		ShowMethodsMenu(bmo);

		ImGui::Separator();
		ShowAddingMenu();

		ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat("##globalThickness", &gvp.vp.thickness, 0.05f, 1.f, 10.f, "thickness = %.1f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::ColorEdit4("Line color", gvp.color4f, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);

		ImGui::Separator();
		ImGui::Text("Mode: %s", _data.chosenPrimEditMode == PrimEditMode::None ? _data.modesList[(uint32_t)_data.chosenMode] : _data.primEditModesList[(uint32_t)_data.chosenPrimEditMode]);

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

	auto t = dynamic_cast<Torch*>(mesh);
	char primName[32];
	strcpy(primName, "%d-hedr figure");
	if (t != nullptr)
		strcpy(primName, "torch");
	if (_data.chosen_meshes.find(mesh) != _data.chosen_meshes.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), primName, mesh->polygons_size());
	}
	else {
		ImGui::Text(primName, mesh->polygons_size());
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (_data.chosen_meshes.find(mesh) == _data.chosen_meshes.end()) {
			_data.chosen_meshes.insert(mesh);
		}
		else {
			_data.chosen_meshes.erase(mesh);
		}
	}
	ImGui::SameLine();

	if (node_open)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		if (t == nullptr)
		{
			ImGui::Text("Color");
			ImGui::TableSetColumnIndex(1);
			auto normColor = mesh->getFaceColor() / 255.;
			bool hasChanged = ImGui::ColorEdit4("", (float*)&normColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
			auto result = normColor * 255;
			if (hasChanged)
			{
				mesh->setFaceColor(result);
				needRefresh = true;
			}

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Albedo");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::DragFloat("##", &(mesh->getAlbedo()), 0.005f, 0.f, 1.f, "%.3f"))
				needRefresh = true;

			for (size_t i = 0; i < mesh->points_size(); i++) {
				ImGui::PushID(&(mesh->getPoint(i)));

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);

				ImGui::AlignTextToFramePadding();
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
				ImGui::Text("Point %d", i);

				ImGui::TableSetColumnIndex(1);
				float x = mesh->getPoint(i).x;
				float y = mesh->getPoint(i).y;
				float z = mesh->getPoint(i).z;
				ImGui::PopID();
			}
		}
		else
		{
			ImGui::Text("Color");
			ImGui::TableSetColumnIndex(1);
			auto normColor = t->getColor() / 255.;
			bool hasChanged = ImGui::ColorEdit4("", (float*)&normColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
			auto result = normColor * 255;
			if (hasChanged)
			{
				t->setColor(result);
				needRefresh = true;
			}
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Intensity");
			ImGui::TableSetColumnIndex(1);
			//auto intensity = t->getIntensity();
			if (ImGui::DragFloat("##", &(t->getIntensity()), 0.005f, 0.f, 1.f, "%.3f"))
			{
				//t->setIntensity(intensity);
				needRefresh = true;
			}
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
}

void BLEV::Interface::ObjectTable::ShowHorizonTable(FloatingHorizon* horizon, size_t idx)
{
	ImGui::PushID(horizon);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("Horizon", "horizon%d", idx);
	ImGui::TableSetColumnIndex(1);

	char primName[32];
	strcpy(primName, "Horizon %d");
	if (_data.chosen_horizons.find(horizon) != _data.chosen_horizons.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), primName, idx);
	}
	else {
		ImGui::Text(primName, idx);
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (_data.chosen_horizons.find(horizon) == _data.chosen_horizons.end()) {
			_data.chosen_horizons.insert(horizon);
		}
		else {
			_data.chosen_horizons.erase(horizon);
		}
	}
	ImGui::SameLine();

	if (node_open)
	{
		ImGui::PushID(&(horizon->upperColor));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::Text("Upper Color");
		ImGui::TableSetColumnIndex(1);
		auto upperColor = horizon->upperColor / 255.;
		bool upperHasChanged = ImGui::ColorEdit4("", (float*)&upperColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
		auto upperResult = upperColor * 255;
		if (upperHasChanged)
		{
			horizon->upperColor = upperResult;
			//needRefresh = true;
		}
		ImGui::PopID();

		ImGui::PushID(&(horizon->lowerColor));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Lower Color");
		ImGui::TableSetColumnIndex(1);
		auto lowerColor = horizon->lowerColor / 255.;
		bool lowerHasChanged = ImGui::ColorEdit4("", (float*)&lowerColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
		auto lowerResult = lowerColor * 255;
		if (lowerHasChanged)
		{
			horizon->lowerColor = lowerResult;
			//needRefresh = true;
		}
		ImGui::PopID();

		ImGui::TreePop();
	}

	ImGui::PopID();
}

void BLEV::Interface::ObjectTable::ShowGBodyTable(GeometricBody* gb, size_t idx)
{
	ImGui::PushID(gb);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	bool node_open = ImGui::TreeNode("GBody", "%d", idx);
	ImGui::TableSetColumnIndex(1);

	char primName[32];
	strcpy(primName, PGBodyToChar(gb));
	if (_data.chosen_gbodies.find(gb) != _data.chosen_gbodies.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), primName, idx);
	}
	else {
		ImGui::Text(primName, idx);
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (_data.chosen_gbodies.find(gb) == _data.chosen_gbodies.end()) {
			_data.chosen_gbodies.insert(gb);
		}
		else {
			_data.chosen_gbodies.erase(gb);
		}
	}
	ImGui::SameLine();

	if (node_open)
	{
		// =================================== Color
		ImGui::PushID(&(gb->GetColor()));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Color");
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(70);
		ImVec3 newColor = { (float)gb->GetColor().At(0), (float)gb->GetColor().At(1), (float)gb->GetColor().At(2) };
		bool colorHasChanged = ImGui::ColorEdit3("", (float*)&newColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
		if (colorHasChanged)
		{
			gb->SetColor(HVec<double>{ (double)newColor.x, (double)newColor.y, (double)newColor.z });
			needRefresh = true;
		}
		ImGui::PopID();

		// =================================== Origin
		ImGui::PushID(&(gb->Origin));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Origin");
		// x
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(70);
		float x = (float)gb->Origin.At(0);
		bool xHasChanged = ImGui::DragFloat("X", &x, 0.5f, -10.f, 10.f, "%.1f");
		if (xHasChanged)
			gb->Origin.SetAt(0, (double)x);

		// y
		float y = (float)gb->Origin.At(1);
		bool yHasChanged = ImGui::DragFloat("Y", &y, 0.5f, -10.f, 10.f, "%.1f");
		if (yHasChanged)
			gb->Origin.SetAt(1, (double)y);

		// z
		float z = (float)gb->Origin.At(2);
		bool zHasChanged = ImGui::DragFloat("Z", &z, 0.5f, -10.f, 10.f, "%.1f");
		if (zHasChanged)
			gb->Origin.SetAt(2, (double)z);
		ImGui::PopID();

		// =================================== Rotation
		ImGui::PushID(&(gb->Rotation));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Rotation");
		// x
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(70);
		float pitch = (float)gb->Rotation.At(0);
		bool pitchHasChanged = ImGui::DragFloat("Pitch", &pitch, 5.f, -360.f, 360.f, "%.1f");
		if (pitchHasChanged)
			gb->Rotation.SetAt(0, (double)pitch);

		// y
		float yaw = (float)gb->Rotation.At(1);
		bool yawHasChanged = ImGui::DragFloat("Yaw", &yaw, 5.f, -360.f, 360.f, "%.1f");
		if (yawHasChanged)
			gb->Rotation.SetAt(1, (double)yaw);

		// z
		float roll = (float)gb->Rotation.At(2);
		bool rollHasChanged = ImGui::DragFloat("Roll", &roll, 5.f, -360.f, 360.f, "%.1f");
		if (rollHasChanged)
			gb->Rotation.SetAt(2, (double)roll);
		ImGui::PopID();

		//// =================================== Scale
		//ImGui::PushID(&(gb->Scale));
		//ImGui::TableNextRow();
		//ImGui::TableSetColumnIndex(0);
		//ImGui::Text("Scale");
		//// x
		//ImGui::TableSetColumnIndex(1);
		//ImGui::PushItemWidth(70);
		//float sX = (float)gb->Scale.At(0);
		//bool sxHasChanged = ImGui::DragFloat("X", &sX, 0.1f, 0.f, 10.f, "%.1f");
		//if (sxHasChanged)
		//	gb->Scale.SetAt(0, (double)sX);

		//// y
		//float sY = (float)gb->Scale.At(1);
		//bool syHasChanged = ImGui::DragFloat("Y", &sY, 0.1f, 0.f, 10.f, "%.1f");
		//if (syHasChanged)
		//	gb->Scale.SetAt(1, (double)sY);

		//// z
		//float sZ = (float)gb->Scale.At(2);
		//bool szHasChanged = ImGui::DragFloat("Z", &sZ, 0.1f, 0.f, 10.f, "%.1f");
		//if (szHasChanged)
		//	gb->Scale.SetAt(2, (double)sZ);

		if (
			xHasChanged || yHasChanged || zHasChanged 
			|| pitchHasChanged || yawHasChanged || rollHasChanged 
			//|| sxHasChanged || syHasChanged || szHasChanged
			) {
			gb->SetTransform();
			needRefresh = true;
		}
		ImGui::PopID();

		ImGui::TreePop();
	}

	ImGui::PopID();
}

void BLEV::Interface::ObjectTable::ShowLightTable(Light* light, size_t idx)
{
	ImGui::PushID(light);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	bool node_open = ImGui::TreeNode("Light", "%d", idx);
	ImGui::TableSetColumnIndex(1);
	ImGui::Text(PLightToChar(light));

	if (node_open)
	{
		// =================================== pitchYaw edit
		ImGui::PushID(&(light->pitchYaw));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Position");
		// pitch
		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(70);
		float pitch = (float)light->pitchYaw.At(0);
		bool pitchHasChanged = ImGui::DragFloat("Pitch", &pitch, 5.f, -89.f, 89.f, "%.0f");
		if (pitchHasChanged)
		{
			light->updatePitchYaw(HVec<double>{ (double)pitch, light->pitchYaw.At(1)});
			needRefresh = true;
		}
		//yaw
		float yaw = (float)light->pitchYaw.At(1);
		bool yawHasChanged = ImGui::DragFloat("Yaw", &yaw, 5.f, 0.f, 3600.f, "%.0f");
		if (yawHasChanged)
		{
			light->updatePitchYaw(HVec<double>{ light->pitchYaw.At(0), (double)yaw});
			needRefresh = true;
		}
		// R 
		float r = (float)light->r;
		bool rHasChanged = ImGui::DragFloat("R", &r, 1.f, 0.f, 100.f, "%.0f");
		if (rHasChanged)
		{
			light->updateOriginDistance((double)r);
			needRefresh = true;
		}
		ImGui::PopID();

		// =================================== color edit
		ImGui::PushID(&(light->color));

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Color");

		ImGui::TableSetColumnIndex(1);
		ImGui::PushItemWidth(70);
		ImVec3 lightColor = { (float)light->color.At(0), (float)light->color.At(1), (float)light->color.At(2) };
		bool colorHasChanged = ImGui::ColorEdit3("", (float*)&lightColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);
		if (colorHasChanged)
		{
			light->color = HVec<double>{ lightColor.x, lightColor.y, lightColor.z };
			needRefresh = true;
		}
		// intensity
		float fint = (float)light->intensity;
		bool intensityHasChanged = ImGui::DragFloat("Intensity", &fint, 0.01f, 0.f, 1.f, "%.2f");
		if (intensityHasChanged && (fint >= 0.f) && fint <= 1.f)
		{
			light->intensity = (double)fint;
			needRefresh = true;
		}
		ImGui::PopID();


		ImGui::TreePop();
	}

	ImGui::PopID();
}

const char* BLEV::Interface::ObjectTable::PGBodyToChar(GeometricBody* gb)
{
	if (dynamic_cast<Sphere*>(gb) != nullptr) return "Sphere";
	if (dynamic_cast<Ellipsoid*>(gb) != nullptr) return "Ellipsoid";
	if (dynamic_cast<Plane*>(gb) != nullptr) return "Plane";
	if (dynamic_cast<Cylinder*>(gb) != nullptr) return "Cylinder";
	if (dynamic_cast<Cone*>(gb) != nullptr) return "Cone";
	return "Undefined";
}

const char* BLEV::Interface::ObjectTable::PLightToChar(Light* l)
{
	if (dynamic_cast<PointLight*>(l) != nullptr) return "Point light";
	return "Light";
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

		for (size_t i = 0; i < _data.horizons.size(); i++)
		{
			ShowHorizonTable(_data.horizons[i], i);
			//ImGui::Separator();
		}

		for (size_t i = 0; i < _data.cringulik.scene.bodies.size(); i++)
		{
			ShowGBodyTable(_data.cringulik.scene.bodies.at(i), i);
			//ImGui::Separator();
		}
		for (size_t i = 0; i < _data.cringulik.scene.lights.size(); i++)
		{
			ShowLightTable(_data.cringulik.scene.lights.at(i), i);
			//ImGui::Separator();
		}

		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
}

void BLEV::Interface::Canvas::ProcessCamKeyboardInput(Camera& cam, const float& deltaTime) {
	if (ImGui::IsKeyPressed(ImGuiKey_W)) {
		main_camera.ProcessKeyboard(Camera::Forward, deltaTime);
		needRefresh = true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_A)) {
		main_camera.ProcessKeyboard(Camera::Left, deltaTime);
		needRefresh = true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_S)) {
		main_camera.ProcessKeyboard(Camera::Backward, deltaTime);
		needRefresh = true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_D)) {
		main_camera.ProcessKeyboard(Camera::Right, deltaTime);
		needRefresh = true;
	}
}

void BLEV::Interface::Canvas::ProcessCamMouseInput(Camera& cam, const ImVec2& deltaMouse) {
	if (ImGui::IsKeyDown(ImGuiKey_C)) {
		main_camera.ProcessMouseMovement(deltaMouse);
		needRefresh = true;
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
			if (_data.adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
				_data.adding_line = None;
				_data.new_prim->pop_back();
				if (_data.new_prim->size() == 1)
				{
					_data.primitives.push_back(new Point(_data.new_prim->at(0), _data.new_prim->color(), _data.new_prim->thickness()));
				}
				else
				{
					_data.new_prim->set_connect_bounds(2);
					_data.primitives.push_back(_data.new_prim);
				}
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

	if (_data.chosenView == ViewMode::Wireframe)
	{
		for (auto& bbody : _data.cringulik.scene.bodies) {
			bbody->Draw(draw_list, origin, vp);
		}
		for (auto mesh : _data.meshes) {
			mesh->draw(draw_list, origin, vp, main_camera.direction());
		}
		for (auto& horizon : _data.horizons) {
			horizon->draw(draw_list, main_camera, size, origin, p[0]);
		}
	}
	else if (_data.chosenView == ViewMode::FlatColor)
	{
		if (needShift) {
			zbuf.setOffset(scrolling);
			needShift = false;
		}
		if (needResize) {
			zbuf.resize(size.x, size.y);
			needResize = false;
		}
		if (needRefresh) {
			zbuf.clear();
			zbuf.fillBuffers(_data.meshes, vp, main_camera.direction());
			needRefresh = false;
		}
		zbuf.draw(draw_list, p[0]);
	}
	else if (_data.chosenView == ViewMode::GouraudShading)
	{
		if (needShift) {
			lbuf.setOffset(scrolling);
			needShift = false;
		}
		if (needResize) {
			lbuf.resize(size.x, size.y);
			needResize = false;
		}
		if (needRefresh) {
			lbuf.clear();
			lbuf.fillBuffers(_data.meshes, _data.torch, vp, main_camera.direction());
			needRefresh = false;
		}
		lbuf.draw(draw_list, p[0]);
	}
	else if (_data.chosenView == ViewMode::Cringetracer)
	{
		if (needResize) {
			_data.cringulik.img.Clear();
			_data.cringulik.img.Resize(size.x, size.y);
			needResize = false;
		}
		if (needRefresh) {
			_data.cringulik.Update();
			_data.cringulik.img.Clear();
			_data.cringulik.Render();
			needRefresh = false;
		}
		_data.cringulik.img.Draw(draw_list, p[0]);
	}

	if (_data.rotate_axis != nullptr)
		_data.rotate_axis->draw(draw_list, origin, vp);

	if (show_fps) {
		char fps_c[10];
		sprintf(fps_c, "%.0f", ImGui::GetIO().Framerate);
		draw_list->AddText(p[0], 0xFFFFFFFF, fps_c);
		//ImGui::ShowDemoWindow
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
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_M)) {
			_data.chosenMode = Mode::FreeMove;
			break;
		}
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_P)) {
			_data.chosenMode = Mode::Point;
			break;
		}
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_E)) {
			_data.chosenMode = Mode::Edge;
			break;
		}
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_G)) {
			_data.chosenMode = Mode::Polygon;
			break;
		}
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_B)) {
			_data.chosenMode = Mode::BezierCurve;
			break;
		}
		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
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


			if (ImGui::BeginMenu("View Mode")) {

				for (size_t i = 0; i < _data.viewModesSize; ++i) {
					if (ImGui::MenuItem(_data.viewModes[i], "", _data.chosenView == (ViewMode)i)) {
						_data.chosenView = (ViewMode)i;
						needRefresh = true;
						needResize = true;
						needShift = true;
					}
				}
				ImGui::EndMenu();
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
			if (ImGui::MenuItem("Remove selected", NULL, false, _data.chosen_prims.size() + _data.chosen_meshes.size() + _data.chosen_horizons.size() > 0)) {
				for (auto it = _data.chosen_prims.begin(); it != _data.chosen_prims.end();)
				{
					Primitive* prim = *it;
					it = _data.chosen_prims.erase(it);
					_data.primitives.erase(std::find(_data.primitives.begin(), _data.primitives.end(), prim));
				}
				for (auto it = _data.chosen_meshes.begin(); it != _data.chosen_meshes.end();)
				{
					Mesh* m = *it;
					if (dynamic_cast<Torch*>(m) != nullptr)
						_data.torch = nullptr;
					it = _data.chosen_meshes.erase(it);
					_data.meshes.erase(std::find(_data.meshes.begin(), _data.meshes.end(), m));
				}
				needRefresh = true;
			}
			if (ImGui::MenuItem("Remove all", NULL, false, _data.primitives.size() + _data.fractals.size() + _data.meshes.size() + _data.horizons.size() > 0)) {
				_data.chosen_prims.clear();
				_data.chosen_prim_points.clear();
				_data.chosen_prim_edges.clear();
				_data.chosen_meshes.clear();
				zbuf.clear();
				lbuf.clear();
				_data.primitives.clear();
				_data.fractals.clear();
				_data.chosen_lsys.clear();
				_data.prev_displacement.clear();
				_data.curr_displacement.clear();
				_data.meshes.clear();
				_data.horizons.clear();
				_data.chosen_horizons.clear();

				_data.torch = nullptr;
				delete _data.rotate_axis;
				_data.rotate_axis = nullptr;
				_data.chosenPrimEditMode = PrimEditMode::None;
			}
			if (ImGui::MenuItem("Select all", NULL, false, _data.primitives.size() > 0)) {
				std::copy(_data.primitives.begin(), _data.primitives.end(), std::inserter(_data.chosen_prims, _data.chosen_prims.end()));
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
	Line3d::draw(draw_list, ImVec3(0, 0.f, -GRID_BORDER), ImVec3(0, 0.f, GRID_BORDER), origin, vp, vis_p);
	Line3d::draw(draw_list, ImVec3(-GRID_BORDER, 0.f, 0), ImVec3(GRID_BORDER, 0.f, 0), origin, vp, vis_p);
	for (int next = GRID_STEP; next <= GRID_BORDER; next += GRID_STEP) {
		Line3d::draw(draw_list, ImVec3(next, 0.f, -GRID_BORDER), ImVec3(next, 0.f, GRID_BORDER), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-next, 0.f, -GRID_BORDER), ImVec3(-next, 0.f, GRID_BORDER), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-GRID_BORDER, 0.f, next), ImVec3(GRID_BORDER, 0.f, next), origin, vp, vis_p);
		Line3d::draw(draw_list, ImVec3(-GRID_BORDER, 0.f, -next), ImVec3(GRID_BORDER, 0.f, -next), origin, vp, vis_p);
	}
}
void BLEV::Interface::Canvas::DrawAxis()
{
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(GRID_STEP, 0.f, 0.f), origin, vp, VisualParams(IM_COL32(0, 255, 0, 255), 1.f, true));
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(0.f, GRID_STEP, 0.f), origin, vp, VisualParams(IM_COL32(0, 0, 255, 255), 1.f, true));
	Line3d::draw(draw_list, ImVec3(0.f, 0.f, 0.f), ImVec3(0.f, 0.f, GRID_STEP), origin, vp, VisualParams(IM_COL32(255, 0, 0, 255), 1.f, true));
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
		auto s = ImGui::GetContentRegionAvail();
		if (s != size) {
			needResize = true;
			needRefresh = true;
		}
		size = s;
		if (size.x < MIN_WIDTH) size.x = MIN_WIDTH;
		if (size.y < MIN_HEIGHT) size.y = MIN_HEIGHT;
		p[1] = ImVec2(p[0].x + size.x, p[0].y + size.y);

		ImGuiIO& io = ImGui::GetIO();
		draw_list = ImGui::GetWindowDrawList();

		if (_data.chosenView == GouraudShading)
			draw_list->AddRectFilled(p[0], p[1], IM_COL32(13, 13, 13, 255));
		else if (_data.chosenView != Cringetracer)
			draw_list->AddRectFilled(p[0], p[1], IM_COL32(50, 50, 50, 255));
		//else 
			//draw_list->AddRectFilled(p[0], p[1], IM_COL32(0, 0, 0, 255));

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
				main_camera.resetProjectionSettings();
				needRefresh = true;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_X)) {
				//main_camera.resetPosition(ImVec3(400.f, 0.f, 0.f), ImVec3(0.f, 0.f, 0.f));
				main_camera.setEyeAndPYR(ImVec3(10.f, 0.f, 0.f), ImVec3(0.f, 0.f, 0.f));
				needRefresh = true;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Y)) {
				//main_camera.resetPosition(ImVec3(0.f, 400.f, 0.f), ImVec3(89.f, 0.f, 0.f));
				main_camera.setEyeAndPYR(ImVec3(0.f, 10.f, 0.f), ImVec3(89.f, 0.f, 0.f));
				needRefresh = true;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Z)) {
				//main_camera.resetPosition();
				main_camera.setEyeAndPYR();
				needRefresh = true;
			}
			if (!main_camera.dirtiness())
			{
				prev_point = mouse_pos;
				main_camera.dirtiness() = true;
			}
			deltaMouse = mouse_pos - prev_point;
			prev_point = mouse_pos;

			auto x = (float)io.MouseWheel;
			if (x != 0.f) {
				main_camera.ProcessMouseScroll(x);
				needRefresh = true;
			}
			if ((Camera::CamMode)main_camera.mode() == Camera::Perspective) {
				ProcessCamMouseInput(main_camera, deltaMouse);
				ProcessCamKeyboardInput(main_camera, deltaTime);
			}
		}

		SwitchModes();

		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, b_context_menu_enabled ? -1.0f : 0.0f))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
			needShift = true;
			needRefresh = true;
		}

		drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

		if (b_context_menu_enabled) ShowContextMenu();

		draw_list->PushClipRect(p[0], p[1], true);

		vp = main_camera.viewProjecion(); //auto vp = main_camera.getProjection(); //auto vp = main_camera.getView();

		if (_data.chosenView != Cringetracer && (_data.chosenView != GouraudShading || (_data.torch != nullptr)))
		{
			if (b_grid_2d_enabled) Draw2dGrid();
			if (b_grid_3d_enabled) Draw3dGrid();
			if (b_grid_3d_enabled) DrawAxis();
		}
		DrawObjects();

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
