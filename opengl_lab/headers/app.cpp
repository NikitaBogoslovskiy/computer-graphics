#include "app.h"
void App::ResetClock()
{
	deltaTime = clock.getElapsedTime().asSeconds();
	elapsedTime += deltaTime;
	clock.restart();
}

void App::Init()
{
	/*
	skybox = new Skybox();
	auto ss = new SolarSystem(50);
	ss->LoadModels("tree.obj", "vodica.jpg", "krosh.obj", "krosh.png");
	ss->PrepareData(glm::vec3(0.0f, -1.0f, 0.0f));
	ss->Music().openFromFile("smesharikiBASSBOOSTED.wav");
	ss->Music().setVolume(100.0f);
	scenes.push_back(ss);
	cur_scene = 0;
	*/

	auto le = new LightExhibition();
	le->LoadModels({
			{"MirTanka/Field.obj", 0, 0, "MirTanka/Field.png"},
			{"MirTanka/ChristmasTree.obj", 0, 0, "MirTanka/ChristmasTree.png"},
			{"MirTanka/Stone-1.obj", 0, 0, "MirTanka/Stone-1.png"},
			{"MirTanka/Tanks.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "MirTanka/Tank.png"},

			{"MirTanka/Barrel.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "MirTanka/Barrel.png"},
			{"MirTanka/Barrel.obj", "shaders/mesh/default_l.vert", "shaders/mesh/Toon.frag", "MirTanka/Barrel.png"},

			{"MirTanka/Barrel.obj", 0, 0, "MirTanka/Barrel.png"},
			{"MirTanka/Tree.obj", 0, 0, "MirTanka/Tree.png"},
			{"MirTanka/Stone-2.obj", 0, 0, "MirTanka/Stone-1.png"},
		});
	le->PrepareData();
	scenes.push_back(le);
	cur_scene = 0;
	/*
	*/
}

void App::Draw()
{
	if (!scenes.empty()) {
		currScene()->Draw(elapsedTime, camera);
	}

	//if (skybox) {
	//	// skybox should be rendered last for optimization
	//	skybox->Draw(glm::mat4(1.0f), camera);
	//}
}

void App::Release()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void App::PollEvents(sf::Window& window)
{
	sf::Event event;
	mouseDelta.x = 0;
	mouseDelta.y = 0;

	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			LightExhibition* le = dynamic_cast<LightExhibition*>(currScene());
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				settings.is_up = true;
				break;

			case sf::Keyboard::A:
				settings.is_left = true;
				break;

			case sf::Keyboard::S:
				settings.is_down = true;
				break;

			case sf::Keyboard::D:
				settings.is_right = true;
				break;

			case sf::Keyboard::C:
				window.setMouseCursorVisible(settings.is_cam_active);
				settings.is_cam_active = !settings.is_cam_active;
				window.setMouseCursorGrabbed(settings.is_cam_active);
				break;

			case sf::Keyboard::Q:
				is_playing = true;
				break;

			case sf::Keyboard::F: {
				if (!le) break;
				le->SwitchFlashlight();
			} break;

			case sf::Keyboard::G: {
				if (!le) break;
				le->SwitchSun();
			} break;

			case sf::Keyboard::H: {
				if (!le) break;
				le->SwitchLamp();
			} break;

			case sf::Keyboard::Up:
				settings.is_arrow_up = true;
				break;

			case sf::Keyboard::Down:
				settings.is_arrow_down = true;
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				settings.is_up = false;
				break;

			case sf::Keyboard::A:
				settings.is_left = false;
				break;

			case sf::Keyboard::S:
				settings.is_down = false;
				break;

			case sf::Keyboard::D:
				settings.is_right = false;
				break;

			case sf::Keyboard::Up:
				settings.is_arrow_up = false;
				break;

			case sf::Keyboard::Down:
				settings.is_arrow_down = false;
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			camera.ProcessMouseScroll(event.mouseWheel.delta);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (!isCamDirty) {
				mousePos.x = event.mouseMove.x;
				mousePos.y = event.mouseMove.y;
				isCamDirty = true;
			}
			mouseDelta.x = event.mouseMove.x - mousePos.x;
			mouseDelta.y = event.mouseMove.y - mousePos.y;

			if (settings.is_cam_active) {
				const sf::Vector2i& halfsize = { (int)window.getSize().x / 2 , (int)window.getSize().y / 2 };
				sf::Mouse::setPosition(halfsize, window);
				mousePos.x = halfsize.x;
				mousePos.y = halfsize.y;
			}
		}
		else if (event.type == sf::Event::Resized) {
			glViewport(0, 0, event.size.width, event.size.height);
		}
	}

	if (settings.is_left)  camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (settings.is_right) camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
	if (settings.is_up)    camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	if (settings.is_down)  camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	if (settings.is_arrow_up)    camera.IncVelocity();
	if (settings.is_arrow_down)  camera.DecVelocity();

	if (settings.is_cam_active) camera.ProcessMouseMovement(mouseDelta);
}
