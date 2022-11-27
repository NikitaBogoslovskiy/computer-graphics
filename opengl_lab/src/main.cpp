#include "main.h"

int main() {
    // Создаём окно
    sf::Window window(sf::VideoMode(600, 600), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    if (!window.setActive(true)) {
        return 0;
    }
    SetIcon(window);
    glewInit();
    Init();

    glEnable(GL_DEPTH_TEST);
    
    while (window.isOpen()) {
        sf::Event event;
       
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    is_left = true;
                    break;

                case sf::Keyboard::Right:
                    is_right = true;
                    break;

                case sf::Keyboard::Up:
                    is_up = true;
                    break;

                case sf::Keyboard::Down:
                    is_down = true;
                    break;

                case sf::Keyboard::Num1:
                    current_task = 1;
                    break;

                case sf::Keyboard::Num2:
                    current_task = 2;
                    break;

                case sf::Keyboard::Num3:
                    current_task = 3;
                    break;

                case sf::Keyboard::Num4:
                    current_task = 4;
                    break;

                default:
                    break;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    is_left = false;
                    break;

                case sf::Keyboard::Right:
                    is_right = false;
                    break;

                case sf::Keyboard::Up:
                    is_up = false;
                    break;

                case sf::Keyboard::Down:
                    is_down = false;
                    break;

                default:
                    break;
                }
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        if (current_task == 1) {
            if (is_left) offset[0] = std::max(-1.f, offset[0] - 0.01f);
            if (is_right) offset[0] = std::min(1.f, offset[0] + 0.01f);
            if (is_up) offset[1] = std::min(1.f, offset[1] + 0.01f);
            if (is_down) offset[1] = std::max(-1.f, offset[1] - 0.01f);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Draw();

        window.display();
    }

    Release();

    return 0;
}
