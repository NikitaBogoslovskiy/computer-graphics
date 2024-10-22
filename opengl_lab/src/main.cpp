#include "../headers/main.h"

int main() {
	sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "CringeCAD2", sf::Style::Default, sf::ContextSettings(24, 8, 16));
	window.setFramerateLimit(100);
	App app = App(&window);

	GLenum errorcode = glewInit();
	if (errorcode != GLEW_OK) {
		throw std::runtime_error(std::string(reinterpret_cast<const char*>(glewGetErrorString(errorcode))));
	}

	if (!window.setActive(true)) {
		return 0;
	}

	app.Init();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	sf::Clock clock;
	while (window.isOpen()) {
        app.PollEvents(window);
		glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		app.Draw();
		app.ResetClock();
		window.display();
	}
	app.Release();

	return 0;
}
