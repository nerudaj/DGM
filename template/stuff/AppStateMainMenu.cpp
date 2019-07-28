#include "AppStateMainMenu.hpp"
#include "AppStateMenuOptions.hpp"
#include "AppStateIngame.hpp"

void AppStateMainMenu::processButtonClick(int id) {
	switch (id) {
	case Play:
		app->pushState(new AppStateIngame(resmgr));
		break;

	case Options:
		app->pushState(new AppStateMenuOptions(resmgr));
		break;

	case Quit:
		app->popState();
		break;
	}
}

void AppStateMainMenu::input() {
	mousePos = sf::Mouse::getPosition(app->window.getWindowContext());

	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::MouseButtonPressed) {
			for (auto &button : buttons) {
				if (button.isHighlighted(mousePos)) {
					processButtonClick(button.getId());
				}
			}
		}
	}
}

void AppStateMainMenu::update() {
	for (auto &button : buttons) {
		button.update(mousePos);
	}
}

void AppStateMainMenu::draw() {
	text.setString("Project Title");
	text.setCharacterSize(72);
	text.setFillColor(sf::Color::White);

	auto bounds = text.getGlobalBounds();
	sf::Vector2f textSize = { bounds.width, bounds.height };
	auto windowSize = app->window.getSize();
	text.setPosition((sf::Vector2f(windowSize / 2u) - (textSize / 2.f)).x, 20);

	app->window.beginDraw();

	app->window.draw(text);

	for (auto &button : buttons) {
		button.draw(app->window);
	}

	app->window.endDraw();
}

bool AppStateMainMenu::init() {
	try {
		resmgr.loadResourceDir<sf::Texture>(rootDir + "/graphics/textures");
		resmgr.loadResourceDir<sf::Font>(rootDir + "/graphics/fonts");
		resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(rootDir + "/graphics/configs");
		resmgr.loadResourceDir<sf::SoundBuffer>(rootDir + "/audio/sounds");
	}
	catch (std::exception &e) {
		std::cerr << "error:AppStateMainMenu: " << e.what() << std::endl;
		return false;
	}

	sf::Font &font = resmgr.get<sf::Font>("cruft.ttf");
	text.setFont(font);

	buttons.push_back(gui::Button(
		{ 550, 360 }, { 180, 40 }, "Play", font, Play
	));
	buttons.push_back(gui::Button(
		{ 550, 420 }, { 180, 40 }, "Options", font, Options
	));
	buttons.push_back(gui::Button(
		{ 550, 480 }, { 180, 40 }, "Quit", font, Quit
	));

	return true;
}