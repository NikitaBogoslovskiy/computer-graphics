#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include <vector>
#include <set>
#include <math.h>
#include <time.h>  
#include <iostream>
#include <unordered_set>

#include "BLEV.h"

int main(void)
{
	srand(time(NULL));
	GLFWwindow* window;

	BLEV app;

	ImVec2 canvas_sz;
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "CringeCAD", NULL, NULL);
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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app.ShowMenuBar();
		app.ShowAdditionalWindows();
		app.ShowContent();
		//ImGui::ShowDemoWindow();

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