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

    while (window.isOpen()) {
        sf::Event event;
       
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        Draw();

        window.display();
    }

    Release();

    return 0;
}
