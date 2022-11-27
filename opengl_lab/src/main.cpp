#include "../headers/main.h"

void pollEvents(sf::Window& window, App& app);

int main() {
	sf::Window window(sf::VideoMode(600, 600), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
	App app = App();

	GLenum errorcode = glewInit();
	if (errorcode != GLEW_OK) {
		std::cout << glewGetErrorString(errorcode) << std::endl;
		throw std::runtime_error("Err");
	}

	if (!window.setActive(true)) {
		return 0;
	}

    //Tetrahedron* tetr = new Tetrahedron();
	app.Init();

	glEnable(GL_DEPTH_TEST);

	while (window.isOpen()) {
        pollEvents(window, app);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //tetr->Draw();
		app.Draw();
		window.display();
	}
    //tetr->Release();
	app.Release();

	return 0;
}

void pollEvents(sf::Window& window, App& app) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                app.is_left = true;
                break;

            case sf::Keyboard::Right:
                app.is_right = true;
                break;

            case sf::Keyboard::Up:
                app.is_up = true;
                break;

            case sf::Keyboard::Down:
                app.is_down = true;
                break;

            case sf::Keyboard::Num1:
                app.cur_task = 0;
                break;

            case sf::Keyboard::Num2:
                app.cur_task = 1;
                break;

            case sf::Keyboard::Num3:
                app.cur_task = 2;
                break;

            case sf::Keyboard::Num4:
                app.cur_task = 3;
                break;

            default:
                break;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                app.is_left = false;
                break;

            case sf::Keyboard::Right:
                app.is_right = false;
                break;

            case sf::Keyboard::Up:
                app.is_up = false;
                break;

            case sf::Keyboard::Down:
                app.is_down = false;
                break;

            default:
                break;
            }
        }
        else if (event.type == sf::Event::Resized) {
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }

	if (app.cur_task >= app.entities.size()) return;

	if (app.is_left) app.entities[app.cur_task]->offset[0] = std::max(-1.f, app.entities[app.cur_task]->offset[0] - 0.01f);
	if (app.is_right) app.entities[app.cur_task]->offset[0] = std::min(1.f, app.entities[app.cur_task]->offset[0] + 0.01f);
	if (app.is_up) app.entities[app.cur_task]->offset[1] = std::min(1.f, app.entities[app.cur_task]->offset[1] + 0.01f);
	if (app.is_down) app.entities[app.cur_task]->offset[1] = std::max(-1.f, app.entities[app.cur_task]->offset[1] - 0.01f);
}