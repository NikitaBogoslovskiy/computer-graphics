//библиотека для создания и открытия окон, создания OpenGL контекста и управления вводом
#include <GLFW/glfw3.h>

//в glew же поддержка opengl 4.6
// однако без надобности использовать не будем пока что :)
//#include <GL/glew.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        glVertex2f(0.23f, 0.63f);
        glVertex2f(0.338f, 0.15f);
        glVertex2f(0.29f, 0.27f);
        glVertex2f(0.15f, 0.27f);
        glVertex2f(0.105f, 0.15f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}