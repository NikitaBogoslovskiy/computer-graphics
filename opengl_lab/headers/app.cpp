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
	le->SetPlayer(&player);
	le->LoadModels({
		// i wont even care 'bout passing models properly. 
			{"MirTanka/Tanks.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "MirTanka/Tank.png"},

			{"MirTanka/Field.obj", 0, 0, "MirTanka/Field.png"},
			{"MirTanka/ChristmasTree.obj", 0, 0, "MirTanka/ChristmasTree.png"},
			{"MirTanka/Stone-1.obj", 0, 0, "MirTanka/Stone-1.png"},

			{"MirTanka/Barrel.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "MirTanka/Barrel.png"},
			{"MirTanka/Barrel.obj", "shaders/mesh/default_l.vert", "shaders/mesh/Toon.frag", "MirTanka/Barrel.png"},

			{"MirTanka/Barrel.obj", 0, 0, "MirTanka/Barrel.png"},
			{"MirTanka/Tree.obj", 0, 0, "MirTanka/Tree.png"},
			{"MirTanka/Stone-2.obj", 0, 0, "MirTanka/Stone-1.png"},
		},
		{
			{"dummy_obj.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "dummy_wood.jpg"},
			{"dummy_obj.obj", "shaders/mesh/default_l.vert", "shaders/mesh/CookTorrance.frag", "dummy_wood.jpg"},
		},

		{
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
			{"MirTanka/Stone-1.obj", 0, 0, "vodica.jpg"},
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
		currScene()->Draw(elapsedTime, *camera);
	}
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
				player.MakeShot();
				break;

			case sf::Keyboard::R:
				player.ResetVelocity();
				break;

			case sf::Keyboard::F: {
				player.SwitchHeadlights();
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
			camera->ProcessMouseScroll(event.mouseWheel.delta);
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
	ProcessUserInput();
	if (settings.is_arrow_up)    player.IncVelocity();
	if (settings.is_arrow_down)  player.DecVelocity();
	if (settings.is_cam_active) camera->ProcessMouseMovement(mouseDelta);
	player.UpdateBullets(deltaTime);
}

void App::ProcessUserInput()
{
	if (settings.is_left)  player.ProcessKeyboard(Player::LEFT, deltaTime);
	if (settings.is_right) player.ProcessKeyboard(Player::RIGHT, deltaTime);
	if (settings.is_up)    player.ProcessKeyboard(Player::FORWARD, deltaTime);
	if (settings.is_down)  player.ProcessKeyboard(Player::BACKWARD, deltaTime);
}
