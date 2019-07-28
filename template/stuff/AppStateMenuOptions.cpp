#include "AppStateMenuOptions.hpp"

void AppStateMenuOptions::processButtonClick(int id) {
	switch (id) {
	case Back:
		app->popState();
		break;
	}
}

void AppStateMenuOptions::input() {
	mousePos = sf::Mouse::getPosition(app->window.getWindowContext());

	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::MouseButtonPressed) {
			if (chkboxFullscreen.isHighlighted(mousePos)) {
				chkboxFullscreen.toggle();
				app->window.toggleFullscreen();
			}
			else if (buttonBack.isHighlighted(mousePos)) {
				processButtonClick(buttonBack.getId());
			}
		}
	}
}

void AppStateMenuOptions::update() {
	chkboxFullscreen.update(mousePos);
	buttonBack.update(mousePos);
}

void AppStateMenuOptions::draw() {
	app->window.beginDraw();

	app->window.draw(title);
	app->window.draw(labelFullscreen);
	chkboxFullscreen.draw(app->window);
	buttonBack.draw(app->window);

	app->window.endDraw();
}

bool AppStateMenuOptions::init() {
	sf::Font &font = resmgr.get<sf::Font>("cruft.ttf");

	title.setCharacterSize(72);
	title.setFillColor(sf::Color::White);
	title.setFont(font);
	title.setString("Options");

	auto bounds = title.getGlobalBounds();
	sf::Vector2f textSize = { bounds.width, bounds.height };
	auto windowSize = app->window.getSize();
	title.setPosition((sf::Vector2f(windowSize / 2u) - (textSize / 2.f)).x, 20);

	labelFullscreen.setFont(font);
	labelFullscreen.setString("Fullscreen");
	labelFullscreen.setCharacterSize(32);
	labelFullscreen.setPosition(50.f, 200.f);
	labelFullscreen.setFillColor(sf::Color::White);

	chkboxFullscreen = gui::Checkbox({ 1000.f, 200.f }, { 32.f, 32.f }, app->window.isFullscreen(), 0);

	buttonBack = gui::Button({1050, 630}, { 180, 40 }, "Back", font, Back);

	return true;
}
