#define _CRT_SECURE_NO_WARNINGS

#include "BLEV/Application.h"

int main(void)
{
	srand(time(NULL));
	GLFWwindow* window;
	
	BLEV::Application app;

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

		app.body.ShowExternalWindows();
		app.body.ShowContent();
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

/*
Mesh* mesh = new Mesh();

		mesh->open("bunny.obj");
		//mesh->open("__male.obj");
		//mesh->open("boat.obj");
		//mesh->open("__hand.obj");
		//mesh->open("tree.obj");
		//mesh->open("mill.obj");
		meshes.push_back(mesh);
		//mesh->save("mybunny.obj");
*/