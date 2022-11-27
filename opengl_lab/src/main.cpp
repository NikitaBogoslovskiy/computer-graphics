#include "../headers/main.h"

int main() {
	sf::Window window(sf::VideoMode(600, 600), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
	App app = App();

	GLenum errorcode = glewInit();
	if (errorcode != GLEW_OK) {
		throw std::runtime_error(std::string(reinterpret_cast<const char*>(glewGetErrorString(errorcode))));
	}

	if (!window.setActive(true)) {
		return 0;
	}

	app.Init();

	glEnable(GL_DEPTH_TEST);

	while (window.isOpen()) {
        app.PollEvents(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		app.Draw();
		window.display();
	}
	app.Release();

	return 0;
}
