//библиотека для создания и открытия окон, создания OpenGL контекста и управления вводом
#define _CRT_SECURE_NO_WARNINGS
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <math.h>
#include "../headers/main.h"
#include "../headers/geometry.h"

#include <iostream>

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

bool intersected(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImVec2* out) {
    ImVec2 n = (d - c);
    n = ImVec2(-n.y, n.x);
    float buf = (n * (b - a));
    if (!buf) return false;
    float t = -(n * (a - c)) / buf;
    *out = a + (t * (b-a));
    if ((a.x < b.x && a.x <= out->x && b.x >= out->x || a.x > b.x && b.x <= out->x && a.x >= out->x || a.y < b.y && a.y <= out->y && b.y >= out->y || a.y > b.y && b.y <= out->y && a.y >= out->y) &&
        (c.x < d.x && c.x <= out->x && d.x >= out->x || c.x > d.x && d.x <= out->x && c.x >= out->x || c.y < d.y && c.y <= out->y && d.y >= out->y || c.y > d.y && d.y <= out->y && c.y >= out->y)) return true;
    return false;
}

//do u like ladders? i don`t
ImVector<ImVec2*> get_intersections(std::set<Primitive*> prims) {
    ImVector<ImVec2*> points = ImVector<ImVec2*>();
    ImVec2 out = ImVec2();
    for (auto src = prims.cbegin(); src != prims.cend(); src++) {
        auto src1 = src;
        for (auto dest = ++src1; dest != prims.cend(); dest++) {
            if (src != dest) {
                auto src_size = (*src)->size();
                auto dest_size = (*dest)->size();
                for (size_t j = 0; j < src_size; j++) {
                    for (size_t i = 0; i < dest_size; i++) {
                        if (intersected((*src)->at(j % src_size), (*src)->at((j + 1) % src_size), (*dest)->at(i % dest_size), (*dest)->at((i + 1) % dest_size), &out)) {
                            points.push_back(new ImVec2(out));
                        }
                        /*
                        if (intersected((*src)->at(j), (*src)->at((j + 1)), (*dest)->at(i), (*dest)->at((i + 1)), &out)) {
                            points.push_back(new ImVec2(out));
                        }
                        */
                    }
                }
            }
        }
    }
    return points;
}

ImVector<ImVec2*> get_intersections(Primitive* curr, std::vector<Primitive*> prims) {
    ImVector<ImVec2*> points = ImVector<ImVec2*>();
    ImVec2 out = ImVec2();
    for (auto dest = prims.cbegin(); dest != prims.cend(); dest++) {
        if (curr != *dest) {
            for (size_t j = 0; j < curr->size() - 1; j++) {
                for (size_t i = 0; i < (*dest)->size() - 1; i++) {
                    if (intersected(curr->at(j), curr->at(j + 1), (*dest)->at(i), (*dest)->at(i + 1), &out)) {
                        points.push_back(new ImVec2(out));
                    }
                }
            }
        }
    }
    return points;
}

void pointPositionWithPolygon(Point& point, Primitive& polygon, bool& isInside, float canvas_width)
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
    int intersections_number = result.Size / 2 - correction;
    isInside = intersections_number % 2 == 1;
}

ImU32 GetCurrentColor(const float* curr_color) {
    return (IM_COL32((int)(curr_color[0] * 255), (int)(curr_color[1] * 255), (int)(curr_color[2] * 255), (int)(curr_color[3] * 255)));
}


char pseudo_console[] = "Command arguments go here...";
//char* pseudo_console = "";

bool checkPointAndEdgeConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Edge*& edge)
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

bool checkPointAndPolygonConditions(std::set<Primitive*>& primitives, std::string& answer, Point*& point, Primitive*& polygon)
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

std::set<Primitive*> chosen_prims = std::set<Primitive*>();

ImVector<ImVec2*> intersections;

void ShowPrimitiveTableRow(Primitive* prim, size_t idx)
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

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        if (chosen_prims.find(prim) == chosen_prims.end()) {
            chosen_prims.insert(prim);
        }
        else {
            chosen_prims.erase(prim);
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

float DegreesToRadians(const float& degrees) {
    return degrees * (2 * acos(0.0) / 180);
}

//TODO make normal foreach with callback and struct of args
void rotate_chosen_prims(const float& angle) {
    if (chosen_prims.size() > 0) {
        auto cpit = chosen_prims.begin();
        if (dynamic_cast<Point*>(*cpit) == NULL) { // if first picked prim is not Point
            //std::cout << "dynamic_cast<Point*>(*cpit) == NULL\n";
            for (auto prim : chosen_prims) {
                if (dynamic_cast<Point*>(prim) == NULL) {
                    prim->rotate(angle, prim->center());
                }
            }
        }
        else { // we wanna rotate relatively to the first picked point
            //std::cout << "dynamic_cast<Point*>(*cpit) != NULL\n";
            auto origin = dynamic_cast<Point*>(*cpit)->at(0);
            cpit++;
            while (1) {
                if (cpit == chosen_prims.end()) { break; }
                if (dynamic_cast<Point*>(*cpit) == NULL) {
                    (*cpit)->rotate(angle, origin);
                }
                cpit++;
            }

        }
    }
}

int main(void)
{
	GLFWwindow* window;
    CurrentState state;
    std::vector<Primitive*> primitives;
    ImVec2 canvas_sz;
    std::string feedback;
    ImVec4 feedback_color;
    float canvas_width;
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
            
            if (ImGui::Button("rotate 90")) {
                rotate_chosen_prims(DegreesToRadians(90.f));
            }

            ImGui::SameLine();

            if (ImGui::Button("rotate N")) {
                char* nstr = pseudo_console;
                float angle;
                if (sscanf(nstr, "%f", &angle)) {
                    rotate_chosen_prims(DegreesToRadians(angle));
                }
                else {
                    std::cout << "Incorrect arguments format for rotate N" << std::endl;
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("translate")) {
                char* nstr = pseudo_console;
                float dx, dy;
                if (sscanf(nstr, "%f%*c%f", &dx, &dy)) {
                    for (auto prim : chosen_prims) {
                        prim->translate(ImVec2(-1 * dx, -1 * dy));
                    }
                }
                else {
                    std::cout << "Incorrect arguments format for translate" << std::endl;
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("scale")) {
                //TODO make normal foreach with callback and struct of args
                if (chosen_prims.size() > 0) {
                    char* nstr = pseudo_console;
                    float scaleX, scaleY;
                    if (sscanf(nstr,"%f%*c%f", &scaleX, &scaleY)) {
                        if (scaleX <= 0 || scaleY <= 0) {
                            std::cout << "Incorrect arguments format for scale" << std::endl;
                        }
                        else {
                            auto cpit = chosen_prims.begin();
                            if (dynamic_cast<Point*>(*cpit) == NULL) { // if first picked prim is not Point
                                //std::cout << "dynamic_cast<Point*>(*cpit) == NULL\n";
                                for (auto prim : chosen_prims) {
                                    if (dynamic_cast<Point*>(prim) == NULL) {
                                        prim->scale(scaleX, scaleY, prim->center());
                                    }
                                }
                            }
                            else { // we wanna rotate relatively to the first picked point
                                //std::cout << "dynamic_cast<Point*>(*cpit) != NULL\n";
                                auto origin = dynamic_cast<Point*>(*cpit)->at(0);
                                cpit++;
                                while (1) {
                                    if (cpit == chosen_prims.end()) { break; }
                                    if (dynamic_cast<Point*>(*cpit) == NULL) {
                                        (*cpit)->scale(scaleX, scaleY, origin);
                                    }
                                    cpit++;
                                }
                            } 
                        }
                    }
                    else {
                        std::cout << "Incorrect arguments format for scale" << std::endl;
                    }
                }
            }

            int chosenType;
            ImGui::Combo("##", &chosenType, classificationType, classificationTypeSize);
            ImGui::SameLine();
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
            
            //const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
            ImGui::InputText("console", pseudo_console, IM_ARRAYSIZE(pseudo_console));

            ImGui::Text("Output: ");
            ImGui::SameLine();
            ImGui::TextColored(feedback_color, feedback.c_str());
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            if (ImGui::BeginTable("prims", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit, ImVec2(200.f, canvas_sz.y))) // ImGuiTableFlags_NoHostExtendX
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

                static Primitive* new_prim;

                switch ((Mode)chosenMode)
                {
                case Mode::Point:          
                    if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        primitives.push_back(new Point(mouse_pos_in_canvas, GetCurrentColor(curr_color), thickness));
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
                    if (adding_line != None && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                        adding_line = None;    
                        new_prim->pop_back();
                        primitives.push_back(new_prim->size() == 1 ? new Point(new_prim->at(0), new_prim->color(), new_prim->thickness()) : new_prim);
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
                        if (ImGui::MenuItem("Remove last", NULL, false, primitives.size() > 0)) { 
                            chosen_prims.erase(primitives.back());
                            primitives.pop_back(); 
                        }
                        if (ImGui::MenuItem("Remove all", NULL, false, primitives.size() > 0)) { 
                            primitives.clear();
                            chosen_prims.clear();
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
                    primitives[i]->draw(draw_list, origin);
                }
                if (new_prim) {
                    new_prim->draw_previe(draw_list, origin);               
                }

                //пересечение выбранных примитивов
                if (chosen_prims.size() > 0) {
                    intersections = get_intersections(chosen_prims);
                    for (size_t i = 0; i < intersections.Size; i++)
                    {
                        draw_list->AddCircleFilled(*intersections[i] + origin, 2.f, IM_COL32(255, 0, 0, 255), 10);
                    }
                }

                //пересечение одного со множеством в динамике
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