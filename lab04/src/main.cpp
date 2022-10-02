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

void ShowPrimitiveTableRow(const char* prefix, int uid)
{
    /*
    ImGui::PushID(uid);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("my sailor is rich");

    if (node_open)
    {
        static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
        for (int i = 0; i < 8; i++)
        {
            ImGui::PushID(i); // Use field index as identifier.
            if (i < 2)
            {
                ShowPlaceholderObject("Child", 424242);
            }
            else
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                if (i >= 5)
                    ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
                else
                    ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
                ImGui::NextColumn();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
    */
}

int main(void)
{
	GLFWwindow* window;
    CurrentState state;
    std::vector<Primitive*> primitives;

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

		//ImGui::ShowDemoWindow((bool*)0);


        static bool use_work_area = true;
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

        static enum AddingLine {
            None,
            FirstClick,
            ReleasedState,
            FinalClick
        } adding_line;

        // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
        // Based on your use case you may want one of the other.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        /* */
        if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
        {
            //Toolbar for choosing the mode of program
            static int chosenMode = 0;
            if (ImGui::Combo("Modes", &chosenMode, modesList, modesSize)) {
                //state.mode = (Mode)(chosenMode + 1);            
            }

            ImGui::Text("Current mode: %d", chosenMode);
            ImGui::Text("Number of prims: %d", primitives.size());
            ImGui::Text("Current line mode: %d", adding_line);

            /*
            if (ImGui::TreeNode("Primitives"))
            {
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                const ImVec2 p = ImGui::GetCursorScreenPos();
                float x = p.x + 4.0f;
                float y = p.y + 4.0f;
                float sz = 36.f;
                const float spacing = 5.0f;
                for (int i = 1; i < 5; i++)
                {
                    // Use SetNextItemOpen() so set the default state of a node to be open. We could
                    // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                    if (i == 0)
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                    draw_list->AddNgon(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, IM_COL32(255, 255, 0, 255), i + 2);
                    x += sz + spacing;
                    ImGui::PushID(i);
                    if (ImGui::InvisibleButton(i + "-side polygon", ImVec2(sz, sz), ImGuiButtonFlags_MouseButtonLeft)) {
                        printf("%d button has been clicked", i);
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            };
            */

            static float curr_color[4] = { 1.f, 1.f, 0.f, 1.f };
            ImGui::ColorEdit4("Line color", curr_color, ImGuiColorEditFlags_DisplayRGB); //ImGuiColorEditFlags_NoInputs
            
            

            //ImGui::SameLine();
            
            if (ImGui::BeginChild("Canvas"))
            {
                static ImVec2 scrolling(0.0f, 0.0f);
                static bool opt_enable_grid = true;
                static bool opt_enable_context_menu = true;            

                //ImGui::Checkbox("Enable grid", &opt_enable_grid);
                //ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

                // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
                ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
                ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
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
                        primitives.push_back((Primitive*)new Point(mouse_pos_in_canvas, GetCurrentColor(curr_color), 1.f));
                    }                
                    break;
                case Mode::Edge:
                    if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        new_prim = new Edge(mouse_pos_in_canvas, mouse_pos_in_canvas, GetCurrentColor(curr_color), 1.f);
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
                        new_prim = new Primitive(GetCurrentColor(curr_color), 1.f);
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