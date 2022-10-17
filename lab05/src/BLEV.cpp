#define _CRT_SECURE_NO_WARNINGS
#include "BLEV.h"
#include <functional>
#include <iostream>

typedef void (BLEV::* MemberPointerType)();

void BLEV::ShowModes()
{
	for (size_t i = 0; i < modesSize; i++) {
		if (ImGui::Selectable(modesList[i], chosenMode == i)) {
			chosenMode = i;
		}
	}
}

void BLEV::PollCallbacks() {
	for (size_t i = 0; i < hotkeysSize; i++) {
		if (funcs::IsLegacyNativeDupe(hotkeys[i])) continue;
		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			if (chosen_prim_points.size() > 0) {
				chosen_prim_points.clear();
			}
			if (chosen_prim_edges.size() > 0) {
				chosen_prim_edges.clear();
			}
			if (chosen_prim_points.size() == 0 && chosen_prim_edges.size() == 0) { // to make it concise
				chosen_prims.clear();
				chosenPrimEditMode = (int)PrimEditMode::None;
			}
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_M)) {
			chosenMode = (int)Mode::FreeMove;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_P)) {
			chosenMode = (int)Mode::Point;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_E)) {
			chosenMode = (int)Mode::Edge;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_G)) {
			chosenMode = (int)Mode::Polygon;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_B)) {
			chosenMode = (int)Mode::BezierCurve;
			break;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_S)) {
			chosenMode = (int)Mode::Select;
			break;
		}
	}
}

void BLEV::ShowFuncs()
{
	if (ImGui::BeginMenu("Rotate")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			rotate_open = true;
		}

		//if (!rotate_open) {
		F_Rotate();
		//}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Translate")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			translate_open = true;
		}

		//if (!translate_open) {
		F_Translate();
		//}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Scale")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			scale_open = true;
		}

		//if (!scale_open) {
		F_Scale();
		//}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Displace")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			displace_open = true;
		}

		//if (!displace_open) {
		F_Displace();
		//}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("L-system")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			lsys_open = true;
		}

		//if (!lsys_open) {
		F_Lsystem();
		//}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Classify")) {

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			classify_open = true;
		}

		//if (!classify_open) {
		F_Classify();
		//}

		ImGui::EndMenu();
	}
}

void BLEV::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		HelpMarker("Hotkeys for multiple tools:\n> Select: S\n> Free move: M\n> Point: P\n> Edge: E\n> Polygon: G\n> Bezier cruve: B");

		ImGui::SameLine();

		if (ImGui::BeginMenu("Mode")) {
			ShowModes();
			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Edit"))
		{
			ShowFuncs();
			ImGui::EndMenu();
		}

		ImGui::SetNextItemWidth(250.f);

		ImGui::DragFloat("##globalThickness", &thickness, 0.05f, 1.f, 10.f, "thickness = %.1f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::ColorEdit4("Line color", curr_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs);

		ImGui::Separator();

		ImGui::Text("Mode: %s", chosenPrimEditMode == (int)PrimEditMode::None ? modesList[chosenMode] : primEditModesList[chosenPrimEditMode]);

		ImGui::EndMainMenuBar();
	}
}

void BLEV::ShowAdditionalWindows()
{
	if (rotate_open) {
		ImGui::Begin("Rotate", &rotate_open);
		F_Rotate();
		ImGui::End();
	}
	if (translate_open) {
		ImGui::Begin("Translate", &translate_open);
		F_Translate();
		ImGui::End();
	}
	if (scale_open) {
		ImGui::Begin("Scale", &scale_open);
		F_Scale();
		ImGui::End();
	}
	if (displace_open) {
		ImGui::Begin("Displace", &displace_open);
		F_Displace();
		ImGui::End();
	}
	if (lsys_open) {
		ImGui::Begin("Lsystem", &lsys_open);
		F_Lsystem();
		ImGui::End();
	}
	if (classify_open) {
		ImGui::Begin("Classify", &classify_open);
		F_Classify();
		ImGui::End();
	}
}

void BLEV::ShowContent()
{
	static bool use_work_area = true;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	if (ImGui::Begin("CringeCAD", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		if (ImGui::BeginTable("prims", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit, ImVec2(200.f, canvas_sz.y))) // ImGuiTableFlags_NoHostExtendX
		{
			// Iterate placeholder objects (all the same data)
			for (size_t i = 0; i < primitives.size(); i++)
			{
				ShowPrimitiveTableRow(primitives[i], i);
				//ImGui::Separator();
			}

			for (size_t i = 0; i < fractals.size(); i++)
			{
				ShowFractalTableRow(fractals[i], i);
				//ImGui::Separator();
			}

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();

		ImGui::SameLine();

		static bool b_canvas = true;
		if (b_canvas && ImGui::BeginChild("Canvas"))
		{
			static ImVec2 scrolling(0.0f, 0.0f);
			static bool opt_enable_grid = true;
			static bool opt_enable_context_menu = true;

			// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
			canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
			canvas_width = canvas_p1.x;

			// Draw border and background color
			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

			PollCallbacks();

			if (chosenPrimEditMode != (int)PrimEditMode::None) {
				switch ((PrimEditMode)chosenPrimEditMode) {
				case PrimEditMode::SelectPoints:
					if (is_hovered && !adding_line && chosen_prims.size() == 1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						Primitive* prim = *chosen_prims.begin();
						size_t p_ind = prim->find_point(mouse_pos_in_canvas);
						if (p_ind != prim->size()) {
							if (chosen_prim_points.find(&prim->at(p_ind)) == chosen_prim_points.end()) {
								chosen_prim_points.insert(&prim->at(p_ind));
							}
							else {
								chosen_prim_points.erase(&prim->at(p_ind));
							}
							break;
						}
					}
					break;
				case PrimEditMode::MovePoints:
					if (is_hovered && !adding_line && chosen_prims.size() == 1 && chosen_prim_points.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						Primitive* prim = *chosen_prims.begin();
						size_t p_ind = prim->find_point(mouse_pos_in_canvas);
						if (p_ind != prim->size()) {
							setTouchedPrim(prim, p_ind);
							adding_line = FirstClick;
						}
					}
					if (adding_line == FirstClick && point_of_transformation != -1) {
						ImVec2 d = (*touched_prim)[point_of_transformation] - mouse_pos_in_canvas;
						std::for_each(chosen_prim_points.begin(), chosen_prim_points.end(), [&d](ImVec2* iv) { auto newPos = *iv - d; (*iv).x = newPos.x; (*iv).y = newPos.y; });
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							setTouchedPrim(nullptr, -1);
							adding_line = None;
						}
					}
					break;
				case PrimEditMode::SelectEdges:
					if (is_hovered && !adding_line && chosen_prims.size() == 1 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						Primitive* prim = *chosen_prims.begin();
						size_t e_ind = prim->find_edge(mouse_pos_in_canvas);
						std::cout << "e_ind " << e_ind << std::endl;
						if (e_ind != prim->size()) {
							std::cout << "found edge " << e_ind << " " << e_ind + 1 << std::endl;
							auto pr = std::pair<ImVec2*, ImVec2*>(&prim->at(e_ind), &prim->at(e_ind == prim->size() - 1 ? 0 : e_ind + 1)); // e_ind == prim->size() - 1 if we found "_connect_bounds" edge
							if (chosen_prim_edges.find(pr) == chosen_prim_edges.end()) {
								chosen_prim_edges.insert(pr);
							}
							else {
								chosen_prim_edges.erase(pr);
							}
							break;
						}
					}
					break;
				case PrimEditMode::MoveEdges:
					if (is_hovered && !adding_line && chosen_prims.size() == 1 && chosen_prim_points.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						/*
						Primitive* prim = *chosen_prims.begin();
						size_t p_ind = prim->find_point(mouse_pos_in_canvas);
						if (p_ind != prim->size()) {
							setTouchedPrim(prim, p_ind);
							adding_line = FirstClick;
						}
						*/
					}
					if (adding_line == FirstClick && point_of_transformation != -1) {
						/*
						ImVec2 d = (*touched_prim)[point_of_transformation] - mouse_pos_in_canvas;
						std::for_each(chosen_prim_points.begin(), chosen_prim_points.end(), [&d](ImVec2* iv) { auto newPos = *iv - d; (*iv).x = newPos.x; (*iv).y = newPos.y; });
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							setTouchedPrim(nullptr, -1);
							adding_line = None;
						}*/
					}
					break;
				default:
					break;
				}

			}
			else
				switch ((Mode)chosenMode)
				{
				case Mode::Point:
					if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						primitives.push_back(new Point(mouse_pos_in_canvas, GetColorFlU32(curr_color), thickness));
					}
					break;
				case Mode::Edge:
					if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						new_prim = new Edge(mouse_pos_in_canvas, mouse_pos_in_canvas, GetColorFlU32(curr_color), thickness);
						adding_line = FirstClick;
					}
					if (adding_line == FirstClick) {
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							adding_line = ReleasedState;
						}
					}
					if (adding_line == ReleasedState)
					{
						(*new_prim)[1] = mouse_pos_in_canvas;
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							adding_line = None;
							primitives.push_back(new_prim);
							new_prim = NULL;
						}
					}
					break;
				case Mode::Polygon:
					if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						new_prim = new Primitive(GetColorFlU32(curr_color), thickness);
						new_prim->push_back(mouse_pos_in_canvas);
						new_prim->push_back(mouse_pos_in_canvas);
						adding_line = FirstClick;
					}
					if (adding_line == FirstClick) {
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							adding_line = ReleasedState;
						}
					}
					if (adding_line == ReleasedState)
					{
						(*new_prim).back() = mouse_pos_in_canvas;
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							adding_line = FirstClick;
							new_prim->push_back(mouse_pos_in_canvas);
						}
					}
					if (adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
						adding_line = None;
						new_prim->pop_back();
						primitives.push_back(new_prim->size() == 1 ? new Point(new_prim->at(0), new_prim->color(), new_prim->thickness()) : new_prim);
						new_prim = NULL;
					}
					break;
				case Mode::BezierCurve:
					if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						new_prim = new BezierCurve(GetColorFlU32(curr_color), thickness);
						new_prim->push_back(mouse_pos_in_canvas);
						new_prim->push_back(mouse_pos_in_canvas);
						adding_line = FirstClick;
					}
					if (adding_line == FirstClick) {
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							adding_line = ReleasedState;
						}
					}
					if (adding_line == ReleasedState)
					{
						(*new_prim).back() = mouse_pos_in_canvas;
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							adding_line = FirstClick;
							new_prim->push_back(mouse_pos_in_canvas);
						}
					}
					if (adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
						adding_line = None;
						new_prim->pop_back();
						primitives.push_back(new_prim);
						new_prim = NULL;
					}
					break;
				case Mode::Select:
					if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						for (int i = 0; i < primitives.size(); i++)
						{
							size_t p_ind = primitives[i]->find_point(mouse_pos_in_canvas);
							if (p_ind != primitives[i]->size()) {
								if (chosen_prims.find(primitives[i]) == chosen_prims.end()) {
									chosen_prims.insert(primitives[i]);
								}
								else {
									chosen_prims.erase(primitives[i]);
								}
								break;
							}
						}
					}
					break;
				case Mode::FreeMove:
					if (is_hovered && !adding_line && chosen_prims.size() > 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						for (auto prim = chosen_prims.begin(); prim != chosen_prims.end(); prim++) {
							int ind = (*prim)->find_point(mouse_pos_in_canvas);
							if (ind != (*prim)->size()) {
								setTouchedPrim(*prim, ind);
								break;
							}
						}
						if (point_of_transformation != -1) {
							adding_line = FirstClick;
						}
					}
					if (adding_line == FirstClick && point_of_transformation != -1) {
						ImVec2 d = touched_prim->at(point_of_transformation) - mouse_pos_in_canvas;
						std::for_each(chosen_prims.begin(), chosen_prims.end(), [&d](Primitive* prim) { prim->translate(&d); });
						if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
							setTouchedPrim(nullptr, -1);
							adding_line = None;
						}
					}
					break;
				default:
					break;
				}

			// Pan (we use a zero mouse threshold when there's no context menu)
			// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
			const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
			{
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}

			// Context menu (under default mouse threshold)
			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (!adding_line && opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
				if (ImGui::BeginPopup("context")) {
					if (chosen_prims.size() == 1) {
						Primitive* prim = *chosen_prims.begin();
						ImGui::TextColored(ImVec4(100.f, 100.f, 100.f, 255), "%d-gon figure", prim->size());

						ImGui::Separator();

						if (ImGui::MenuItem("Cancel selection", NULL, false, chosen_prims.size() == 1)) {
							chosen_prims.erase(prim);
						}

						if (ImGui::MenuItem("Delete object", NULL, false, chosen_prims.size() == 1)) {
							chosen_prims.erase(prim);
							primitives.erase(std::remove(primitives.begin(), primitives.end(), prim), primitives.end());
						}

						if (ImGui::BeginMenu("Points...")) {
							if (ImGui::MenuItem("Select", NULL, false, chosen_prims.size() == 1)) {
								if (chosenPrimEditMode == (int)PrimEditMode::SelectPoints) {
									chosenPrimEditMode = (int)PrimEditMode::None;
								}
								else {
									chosenPrimEditMode = (int)PrimEditMode::SelectPoints;
								}
							}
							if (ImGui::MenuItem("Move", NULL, false, chosen_prims.size() == 1)) {
								if (chosenPrimEditMode == (int)PrimEditMode::MovePoints) {
									chosenPrimEditMode = (int)PrimEditMode::None;
								}
								else {
									chosenPrimEditMode = (int)PrimEditMode::MovePoints;
								}
							}
							if (ImGui::MenuItem("Delete", NULL, false, chosen_prims.size() == 1 && chosen_prim_points.size() > 0)) {
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
								}
							}
							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Edges...")) {
							if (ImGui::MenuItem("Select", NULL, false, chosen_prims.size() == 1)) {
								if (chosenPrimEditMode == (int)PrimEditMode::SelectEdges) {
									chosenPrimEditMode = (int)PrimEditMode::None;
								}
								else {
									chosenPrimEditMode = (int)PrimEditMode::SelectEdges;
								}
							}
							if (ImGui::MenuItem("Move", NULL, false, chosen_prims.size() == 1)) {
								if (chosenPrimEditMode == (int)PrimEditMode::MoveEdges) {
									chosenPrimEditMode = (int)PrimEditMode::None;
								}
								else {
									chosenPrimEditMode = (int)PrimEditMode::MoveEdges;
								}
							}
							if (ImGui::MenuItem("Delete", NULL, false, chosen_prims.size() == 1 && chosen_prim_points.size() > 0)) {
								std::cout << "deleting points...\n";
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

					if (ImGui::MenuItem("Remove last prim", NULL, false, primitives.size() > 0)) {
						chosen_prims.erase(primitives.back());
						if (primitives.back() == &prev_displacement)
						{
							prev_displacement.clear();
							curr_displacement.clear();
						}
						primitives.pop_back();
						if (chosen_prims.size() == 0) {
							chosenPrimEditMode = (int)PrimEditMode::None;
						}
					}
					if (ImGui::MenuItem("Remove last fractal", NULL, false, fractals.size() > 0)) {
						chosen_lsys.erase(fractals.back());
						fractals.pop_back();
					}
					if (ImGui::MenuItem("Remove all", NULL, false, primitives.size() + fractals.size() > 0)) {
						primitives.clear();
						fractals.clear();
						chosen_prims.clear();
						chosen_lsys.clear();
						prev_displacement.clear();
						curr_displacement.clear();
						chosenPrimEditMode = (int)PrimEditMode::None;
					}
					ImGui::EndPopup();
				}
			}

			// Draw grid + all lines in the canvas
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid)
			{
				const float GRID_STEP = 64.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
			}


			for (size_t i = 0; i < primitives.size(); i++) {
				primitives[i]->draw(draw_list,
					origin,
					chosen_prims.find(primitives[i]) == chosen_prims.end() ? primitives[i]->color() : IM_COL32(255, 0, 0, 255));
			}

			for (size_t i = 0; i < fractals.size(); i++) {
				fractals[i]->draw(draw_list, origin);
			}

			if (new_prim) {
				new_prim->draw_previe(draw_list, origin);
			}

			// thickness 3.f doesnt git with very thick lines obviously. would be useful if chosen point knew its prim thickness at least
			std::for_each(chosen_prim_points.begin(), chosen_prim_points.end(), [&draw_list, &origin](const ImVec2* ch_p) { draw_list->AddCircleFilled(*ch_p + origin, 3.f, IM_COL32(0, 255, 0, 255), 10); });
			std::for_each(chosen_prim_edges.begin(), chosen_prim_edges.end(), [&draw_list, &origin](const std::pair<ImVec2*, ImVec2*> ch_e) { draw_list->AddLine(*ch_e.first + origin, *ch_e.second + origin, IM_COL32(0, 255, 0, 255), 1.f); });

			//ïåðåñå÷åíèå âûáðàííûõ ïðèìèòèâîâ
			if (chosen_prims.size() > 0) {
				intersections = get_intersections(chosen_prims);
				for (size_t i = 0; i < intersections.Size; i++)
				{
					draw_list->AddCircleFilled(*intersections[i] + origin, 2.f, IM_COL32(255, 0, 0, 255), 10);
				}
			}

			//ïåðåñå÷åíèå îäíîãî ñî ìíîæåñòâîì â äèíàìèêå
			if (new_prim != NULL && primitives.size() > 0) {
				intersections = get_intersections(new_prim, primitives);
				for (size_t i = 0; i < intersections.Size; i++)
				{
					draw_list->AddCircleFilled(*intersections[i] + origin, 2.f, IM_COL32(0, 0, 255, 255), 10);
				}
			}

			draw_list->PopClipRect();

			ImGui::EndChild();
		}

		ImGui::End();
	}

}

void BLEV::ShowPrimitiveTableRow(Primitive* prim, size_t idx)
{
	ImGui::PushID(prim);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("Prim", "prim%d", idx);
	ImGui::TableSetColumnIndex(1);

	if (chosen_prims.find(prim) != chosen_prims.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "%d-gon figure", prim->size());
	}
	else {
		ImGui::Text("%d-gon figure", prim->size());
	}

	ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopup("context")) {

		if (ImGui::MenuItem("Delete object", NULL, false, true)) {
			chosen_prims.erase(prim);
			primitives.erase(std::remove(primitives.begin(), primitives.end(), prim), primitives.end());
			if (chosen_prims.size() == 0) {
				chosenPrimEditMode = (int)PrimEditMode::None;
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (chosen_prims.find(prim) == chosen_prims.end()) {
			chosen_prims.insert(prim);
		}
		else {
			chosen_prims.erase(prim);
			if (chosen_prims.size() == 0) {
				chosenPrimEditMode = (int)PrimEditMode::None;
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
						chosen_prims.erase(prim);
						primitives.erase(primitives.begin() + idx);
						if (chosen_prims.size() == 0) {
							chosenPrimEditMode = (int)PrimEditMode::None;
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

void BLEV::ShowFractalTableRow(Lsystem* lsys, size_t idx)
{
	ImGui::PushID(lsys);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("Lsystem", "lsys%d", idx);
	ImGui::TableSetColumnIndex(1);

	if (chosen_lsys.find(lsys) != chosen_lsys.end()) {
		ImGui::TextColored(ImVec4(255, 0, 0, 255), lsys->is_tree() ? "tree" : "frac");
	}
	else {
		ImGui::Text(lsys->is_tree() ? "tree" : "frac");
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		if (chosen_lsys.find(lsys) == chosen_lsys.end()) {
			chosen_lsys.insert(lsys);
		}
		else {
			chosen_lsys.erase(lsys);
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

void BLEV::NewWindow(const char* label, bool* p_open, MemberPointerType func)
{
	ImGui::Begin(label, p_open);
	func;
	ImGui::End();
}

void BLEV::F_Rotate() {
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
			if (chosen_prims.size() != 0) {
				tr_chpr_rtp(chosen_prims, lammy);
			}
			for (auto lsys : chosen_lsys) {
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
			if (chosen_prims.size() + chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}
			if (chosen_prims.size() != 0) {
				tr_chpr_rtp(chosen_prims, lammy);
			}
			for (auto lsys : chosen_lsys) {
				lsys->rotate(DegreesToRadians(angle), nullptr);
			}
		}
		catch (std::exception& e) {
			console[0]->feedback = e.what();
			console[0]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}

void BLEV::F_Translate() {
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

			if (chosen_prims.size() + chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}
			std::for_each(chosen_prims.begin(), chosen_prims.end(), lammy);
			for (auto lsys : chosen_lsys) {
				lsys->translate(&d);
			}
		}
		catch (std::exception& e) {
			console[1]->feedback = e.what();
			console[1]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}

void BLEV::F_Scale() {
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

			if (chosen_prims.size() + chosen_lsys.size() == 0) {
				throw std::invalid_argument("The number of selected objects must be at least 1");
			}

			if (chosen_prims.size() != 0) {
				tr_chpr_rtp(chosen_prims, lammy);
			}

			for (auto lsys : chosen_lsys) {
				lsys->scale(scaleX, scaleY, nullptr);
			}
		}
		catch (std::exception& e) {
			console[2]->feedback = e.what();
			console[2]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}

void BLEV::F_Displace() {
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
			if (chosen_prims.size() != 2)
				throw std::invalid_argument("You should choose 2 points");
			std::vector<Point*> points;
			for (auto it = chosen_prims.begin(); it != chosen_prims.end(); ++it)
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

			curr_displacement = midpointDisplacement(prev_displacement, points[0], points[1], R, I, iter_num);
			if (prev_displacement.size() != 0)
			{
				auto it = std::find(primitives.begin(), primitives.end(), &prev_displacement);
				if (it != primitives.end())
					primitives.erase(it);
			}
			prev_displacement = curr_displacement;
			prev_displacement.set_connect_bounds(2);
			primitives.push_back(&prev_displacement);
		}
		catch (std::exception& e) {
			console[3]->feedback = e.what();
			console[3]->feedback_color = ImVec4(255, 0, 0, 255);
		}
	}
}

void BLEV::F_Lsystem() {
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
	static float src_col[4] = { curr_color[0], curr_color[1], curr_color[2], curr_color[3] };
	ImGui::ColorEdit4("src##lsysSrcColor", src_col, ImGuiColorEditFlags_NoInputs);

	static bool tree = false;
	static float dest_col[4] = { curr_color[0], curr_color[1], curr_color[2], curr_color[3] };

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
			fractals.push_back(t);
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

void BLEV::F_Classify() {
	static std::string feedback;
	static ImVec4 feedback_color;

	static int chosenType = 0;
	ImGui::Combo("##", &chosenType, classificationType, classificationTypeSize);

	if (ImGui::Button("Classify point position")) {
		Point* point = nullptr;
		Edge* edge = nullptr;
		Primitive* polygon = nullptr;
		bool success, onTheLeft, isInside;

		switch (chosenType)
		{
		case 0:
			success = checkPointAndEdgeConditions(chosen_prims, feedback, point, edge);
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
			success = checkPointAndPolygonConditions(chosen_prims, feedback, point, polygon);
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
			success = checkPointAndPolygonConditions(chosen_prims, feedback, point, polygon);
			if (success)
			{
				feedback_color = ImVec4(0, 255, 0, 255);
				pointPositionWithPolygon(*point, *polygon, isInside, canvas_width);
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

template<typename _Container,
	typename _Value,
	typename>
std::tuple<int, ImVec2*> BLEV::detect_point(const _Container& primitives) {
	auto originIt = std::find_if(primitives.begin(), primitives.end(), [](const auto& prim) { return dynamic_cast<Point*>(prim) != NULL; });
	if (originIt == primitives.end()) return std::make_tuple(0, nullptr);
	return std::make_tuple(1, &(dynamic_cast<Point*>(*originIt)->at(0)));
}

//transforms chosen prims relatively to point
template<typename _Container,
	typename _Value,
	typename>
int BLEV::tr_chpr_rtp(const _Container& primitives, std::function<void(Primitive*, ImVec2*)> lammy) {
	if (primitives.size() == 0) throw std::invalid_argument("No primitives picked");
	int pointsCount; ImVec2* origin;
	std::tie(pointsCount, origin) = detect_point(primitives);

	std::for_each(primitives.begin(), primitives.end(), [&lammy, &origin](Primitive* prim) {
		auto test = dynamic_cast<Point*>(prim);
		if (test != NULL && &(test->at(0)) == origin) return;
		lammy(prim, origin);
		});
	return 0;
}

Primitive BLEV::midpointDisplacement(Primitive& displacement, Point* p1, Point* p2, int R, int I, int iter_num)
{
	auto color = p1->color();
	float thikness = 1;
	Primitive result(color, thikness);
	std::vector<ImVec2> points1;
	if (displacement.size() == 0)
	{
		points1.push_back(p1->at(0));
		points1.push_back(p2->at(0));
		if (iter_num == 1)
		{
			result.push_back(points1[0]);
			result.push_back(points1[1]);
			return result;
		}
		--iter_num;
	}
	else
	{
		for (size_t i = 0; i < displacement.size(); ++i)
			points1.push_back(displacement[i]);
	}

	std::vector<ImVec2> points2;
	std::vector<ImVec2>::iterator start_it = points1.begin(), end_it = points1.end();
	std::vector<ImVec2>* current_vec = &points2;
	srand(time(NULL));
	for (size_t i = 0; i < iter_num; ++i)
	{
		auto it = start_it;
		while (it != end_it - 1)
		{
			auto next = it + 1;
			float x = (it->x + next->x) / 2;
			float y = (it->y + next->y) / 2 + (R == 0 || I == 0 ? 0 : rand() % (2 * R * I) - R * I);
			current_vec->push_back(*it);
			current_vec->push_back(ImVec2(x, y));
			it = next;
		}
		current_vec->push_back(*it);

		start_it = current_vec->begin();
		end_it = current_vec->end();
		if (i % 2 == 0)
		{
			points1.clear();
			current_vec = &points1;
		}
		else
		{
			points2.clear();
			current_vec = &points2;
		}
	}

	for (auto it = start_it; it != end_it; ++it)
		result.push_back(*it);

	return result;
}

void BLEV::pointPositionWithPolygon(Point& point, Primitive& polygon, bool& isInside, float canvas_width)
{
	auto edge = Edge(point[0], ImVec2(canvas_width, point[0].y), point.color(), point.thickness());
	std::set<Primitive*> prims = { &polygon, &edge };
	auto result = get_intersections(prims);
	int correction = 0;
	size_t polygon_size = polygon.size();
	for (size_t i = 0; i < polygon_size; ++i)
	{
		if (polygon[i].y == point[0].y && polygon[i].x > point[0].x) //we don`t count below edges
		{
			if (polygon[(i + polygon_size - 1) % polygon_size].y > polygon[i].y)
				++correction;
			if (polygon[(i + 1) % polygon_size].y > polygon[i].y)
				++correction;
		}
	}
	isInside = (result.Size - correction) % 2 == 1;
}

bool BLEV::checkPointAndEdgeConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Edge*& edge)
{
	if (primitives.size() != 2)
	{
		answer = "You should choose 2 primitives";
		return false;
	}
	else
	{
		for (auto it = primitives.begin(); it != primitives.end(); ++it)
		{
			auto p = dynamic_cast<Point*>(*it);
			auto e = dynamic_cast<Edge*>(*it);
			if (p != nullptr)
			{
				if (point == nullptr)
					point = p;
				else
				{
					answer = "You should choose only one point";
					return false;
				}
			}
			else if (e != nullptr)
			{
				if (edge == nullptr)
					edge = e;
				else
				{
					answer = "You should choose only one edge";
					return false;
				}
			}
			else
			{
				answer = "You can choose only edge and point";
				return false;
			}
		}
	}
	return true;
}

bool BLEV::checkPointAndPolygonConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Primitive*& polygon)
{
	if (primitives.size() != 2)
	{
		answer = "You should choose 2 primitives";
		return false;
	}
	else
	{
		for (auto it = primitives.begin(); it != primitives.end(); ++it)
		{
			if ((*it)->size() == 1)
			{
				if (point == nullptr)
					point = dynamic_cast<Point*>(*it);
				else
				{
					answer = "You should choose only one point";
					return false;
				}
			}
			else if ((*it)->size() > 2)
			{
				if (polygon == nullptr)
					polygon = *it;
				else
				{
					answer = "You should choose only one polygon";
					return false;
				}
			}
			else
			{
				answer = "You can choose only point and polygon";
				return false;
			}
		}
	}
	return true;
}

bool BLEV::intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out) {
	ImVec2 n = (d - c);
	n = ImVec2(-n.y, n.x);
	float buf = (n * (b - a));
	if (!buf) return false;
	float t = -(n * (a - c)) / buf;
	*out = a + (t * (b - a));
	if ((a.x < b.x && a.x <= out->x && b.x >= out->x || a.x > b.x && b.x <= out->x && a.x >= out->x || a.y < b.y && a.y <= out->y && b.y >= out->y || a.y > b.y && b.y <= out->y && a.y >= out->y) &&
		(c.x < d.x && c.x <= out->x && d.x >= out->x || c.x > d.x && d.x <= out->x && c.x >= out->x || c.y < d.y && c.y <= out->y && d.y >= out->y || c.y > d.y && d.y <= out->y && c.y >= out->y)) return true;
	return false;
}

ImVector<ImVec2*> BLEV::get_intersections(std::set<Primitive*> prims) {
	ImVector<ImVec2*> points = ImVector<ImVec2*>();
	ImVec2 out = ImVec2();
	for (auto src = prims.cbegin(); src != prims.cend(); src++) {
		auto src1 = src;
		for (auto dest = ++src1; dest != prims.cend(); dest++) {
			if (src != dest) {
				auto src_size = (*src)->size();
				auto dest_size = (*dest)->size();
				for (size_t j = 0; j < src_size - (src_size == 2 ? 1 : 0); j++) {
					for (size_t i = 0; i < dest_size - (dest_size == 2 ? 1 : 0); i++) {
						if (intersected((*src)->at(j % src_size), (*src)->at((j + 1) % src_size), (*dest)->at(i % dest_size), (*dest)->at((i + 1) % dest_size), &out)) {
							points.push_back(new ImVec2(out));
						}
					}
				}
			}
		}
	}
	return points;
}

ImVector<ImVec2*> BLEV::get_intersections(Primitive* curr, std::vector<Primitive*> prims) {
	ImVector<ImVec2*> points = ImVector<ImVec2*>();
	ImVec2 out = ImVec2();
	auto curr_size = curr->size();
	for (auto dest = prims.cbegin(); dest != prims.cend(); dest++) {
		if (curr != *dest) {
			auto dest_size = (*dest)->size();
			for (size_t j = 0; j < curr_size - 1; j++) {
				for (size_t i = 0; i < dest_size - (dest_size == 2 ? 1 : 0); i++) {
					if (intersected(curr->at(j % curr_size), curr->at((j + 1) % curr_size), (*dest)->at(i % dest_size), (*dest)->at((i + 1) % dest_size), &out)) {
						points.push_back(new ImVec2(out));
					}
				}
			}
		}
	}
	return points;
}
