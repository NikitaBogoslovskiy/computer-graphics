//библиотека для создания и открытия окон, создания OpenGL контекста и управления вводом
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

int main(void)
{
	GLFWwindow* window;
    CurrentState state;
    std::vector<Primitive> primitives;

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

        // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
        // Based on your use case you may want one of the other.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
        /* */
        if (ImGui::Begin("Example: Fullscreen window", &p_open, flags))
        {
            //Toolbar for choosing the mode of program
            int chosenMode;
            ImGui::Combo("Modes", &chosenMode, modesList, modesSize);
            state.mode = static_cast<Mode>(chosenMode);
            switch (state.mode)
            {
                case Mode::EdgeAndPoint:
                    ImGui::RadioButton("Nothing", &state.edgeAndPointOption, 0); ImGui::SameLine();
                    ImGui::RadioButton("Edge", &state.edgeAndPointOption, 1); ImGui::SameLine();
                    ImGui::RadioButton("Point", &state.edgeAndPointOption, 2); ImGui::SameLine();
                    if (ImGui::Button("Solve"))
                    {
                        state.answer = "Answer";
                    }
                    if (!state.answer.empty())
                    {
                        ImGui::SameLine();
                        ImGui::Text(state.answer.c_str());
                    }
                    ImGui::NewLine();
                default:
                    break;
            }

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

                //ImGui::EndChild();
            //}

            if (ImGui::BeginChild("Canvas"))
            {
                static ImVector<ImVec2> points;
                static ImVector<ImU32> colors;
                static ImVec2 scrolling(0.0f, 0.0f);
                static bool opt_enable_grid = true;
                static bool opt_enable_context_menu = true;
                static bool adding_line = false;
                static float curr_color[4] = { 1.f, 1.f, 0.f, 1.f };

                ImGui::Checkbox("Enable grid", &opt_enable_grid);
                ImGui::ColorEdit4("Line color", curr_color, ImGuiColorEditFlags_DisplayRGB); //ImGuiColorEditFlags_NoInputs
                ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

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

                switch (state.mode)
                {
                case Mode::EdgeAndPoint:
                    if (state.edgeAndPointOption == 1)
                    {
                        if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {
                            points.push_back(mouse_pos_in_canvas);
                            points.push_back(mouse_pos_in_canvas);
                            adding_line = true;
                        }
                        if (adding_line)
                        {
                            points.back() = mouse_pos_in_canvas;
                            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                            {
                                colors.push_back(IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
                                adding_line = false;
                                primitives.push_back(Edge(points.front().x, points.front().y, points.back().x, points.back().y));
                            }
                        }
                    }
                    if (state.edgeAndPointOption == 2)
                    {
                        if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                            primitives.push_back(Point(origin.x + mouse_pos_in_canvas.x, origin.y + mouse_pos_in_canvas.y));
                    }
                    break;
                default:
                    break;
                }

                // Add first and second point
                /*
                if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    points.push_back(mouse_pos_in_canvas);
                    points.push_back(mouse_pos_in_canvas);
                    adding_line = true;
                }
                if (adding_line)
                {
                    points.back() = mouse_pos_in_canvas;
                    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        colors.push_back(IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
                        adding_line = false;
                    }
                }
                */

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
                if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
                    ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
                if (ImGui::BeginPopup("context"))
                {
                    if (adding_line)
                        points.resize(points.size() - 2);
                    adding_line = false;
                    if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
                    if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
                    ImGui::EndPopup();
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
                /*
                for (int n = 0; n < points.Size; n += 2) {
                    int temp = n / 2;
                    ImU32 clr = IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255));
                    if (colors.Size > temp)
                        clr = colors[temp];
                    draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), clr, 2.0f);
                }
                */
                draw_list->PopClipRect();

                ImGui::EndChild();
            }

            //ImGui::SameLine();

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