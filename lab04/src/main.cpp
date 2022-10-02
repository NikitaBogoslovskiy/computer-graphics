//���������� ��� �������� � �������� ����, �������� OpenGL ��������� � ���������� ������
#define _CRT_SECURE_NO_WARNINGS
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <string>
#include "../headers/main.h"
#include "../headers/funcs.h"
#include <vector>

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

ImU32 GetCurrentColor(const float* curr_color) {
    return (IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
}

Primitive* current_prim;

void ShowPrimitiveTableRow(Primitive* prim, size_t idx)
{
    ImGui::PushID(prim);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Prim", "prim%d", idx);
    ImGui::TableSetColumnIndex(1);
    
    if (prim == current_prim) {
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "%d-gon figure", prim->size());
    }
    else {
        ImGui::Text("%d-gon figure", prim->size());
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        if (current_prim != prim) {
            current_prim = current_prim = prim;
        }
        else {
            current_prim = NULL;
        }
    }
    
    if (node_open)
    {
        for (size_t i = 0; i < prim->size(); i++) {
            ImGui::PushID(&(prim->operator[](i)));

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
            ImGui::Text("Point %d", i);

            ImGui::TableSetColumnIndex(1);
            //ImGui::SliderFloat("x", &(prim->operator[](i).x), 0.f, 1000.f, ".1f", 0.5f);
            ImGui::InputFloat("x", &(prim->operator[](i).x));
            //ImGui::SameLine();
            //ImGui::SliderFloat("y", &(prim->operator[](i).y), 0.f, 1000.f, ".1f", 0.5f);
            ImGui::InputFloat("y", &(prim->operator[](i).y));

            ImGui::PopID();
        }

        ImGui::TreePop();
    }
    
    ImGui::PopID();
}

int main(void)
{
	GLFWwindow* window;
    CurrentState state;
    std::vector<Primitive*> primitives;
    ImVec2 canvas_sz;
    current_prim = NULL;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Affine transformations", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	bool p_open = true;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


        static bool use_work_area = true;
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

        static enum AddingLine {
            None,
            FirstClick,
            ReleasedState,
            FinalClick
        } adding_line;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        /* */
        
        static int chosenMode = 0;
        static float thickness = 1.0f;
        static float curr_color[4] = { 1.f, 1.f, 0.f, 1.f };
        
        if (ImGui::Begin("Affine transformaitons", &p_open, flags))
        {
            if (ImGui::BeginTable("mode & thickness & color", 3)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                //Toolbar for choosing the mode of program
                if (ImGui::Combo("Modes", &chosenMode, modesList, modesSize)) {
       
                }
                
                ImGui::TableNextColumn();

                ImGui::SliderFloat(" ", &thickness, 1.f, 10.f, "thickness = %.1f");

                ImGui::TableNextColumn();

                ImGui::ColorEdit4("Line color", curr_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs); //ImGuiColorEditFlags_NoInputs

                ImGui::EndTable();
            }
            //ImGui::SameLine();
            //ImGui::Text("Number of prims: %d", primitives.size());
            //ImGui::Text("Current prim: %d", current_prim);

            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            if (ImGui::BeginTable("prims", 2, ImGuiTableFlags_Borders |  ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit, ImVec2(200.f, canvas_sz.y))) // ImGuiTableFlags_NoHostExtendX
            {
                // Iterate placeholder objects (all the same data)
                for (size_t i = 0; i < primitives.size(); i++)
                {
                    ShowPrimitiveTableRow(primitives[i], i);
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

                static Primitive* new_prim;

                switch ((Mode)chosenMode)
                {
                case Mode::Point:          
                    if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        primitives.push_back((Primitive*)new Point(mouse_pos_in_canvas, GetCurrentColor(curr_color), thickness));
                    }                
                    break;
                case Mode::Edge:
                    if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        new_prim = new Edge(mouse_pos_in_canvas, mouse_pos_in_canvas, GetCurrentColor(curr_color), thickness);
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
                        new_prim = new Primitive(GetCurrentColor(curr_color), thickness);
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
                    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                        adding_line = None;    
                        new_prim->pop_back();
                        primitives.push_back(new_prim);
                        new_prim = NULL;
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
                        if (ImGui::MenuItem("Remove last", NULL, false, primitives.size() > 0)) { primitives.pop_back(); }
                        if (ImGui::MenuItem("Remove all", NULL, false, primitives.size() > 0)) { primitives.clear(); }
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
                    primitives[i]->draw(draw_list, origin);
                }
                if (new_prim) {
                    new_prim->draw_previe(draw_list, origin);               
                }

                
                draw_list->PopClipRect();

            ImGui::EndChild();
            }
            

        }
        ImGui::End();
       


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}