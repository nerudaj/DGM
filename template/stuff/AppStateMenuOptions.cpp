#include "AppStateMenuOptions.hpp"

void AppStateMenuOptions::buildLayout() {
	tgui::Label::Ptr title = tgui::Label::create("Options");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	title->setTextSize(72);
	title->setSize("50%", "25%");
	title->setPosition("25%", "5%");
	gui.add(title, "LabelOptions");

	tgui::CheckBox::Ptr fullscreenCheckbox = tgui::CheckBox::create("Toggle fullscreen");
	fullscreenCheckbox->setSize("5%", "5%");
	fullscreenCheckbox->setPosition("42.5%", "35%");
	fullscreenCheckbox->setChecked(app->window.isFullscreen());
	gui.add(fullscreenCheckbox, "CheckboxFullscreen");
	fullscreenCheckbox->connect("checked", [this]() {
		app->window.toggleFullscreen();
	});
	fullscreenCheckbox->connect("unchecked", [this]() {
		app->window.toggleFullscreen();
	});

	tgui::Button::Ptr backButton = tgui::Button::create("Back");
	backButton->setSize("15%", "5%");
	backButton->setPosition("84%", "94%");
	backButton->connect("pressed", [this]() { app->popState(); });
	gui.add(backButton, "ButtonExit");
}

void AppStateMenuOptions::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		gui.handleEvent(event);
	}
}

void AppStateMenuOptions::update() {}

void AppStateMenuOptions::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateMenuOptions::init() {
	sf::Font &font = resmgr.get<sf::Font>("cruft.ttf");

	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	/*title.setCharacterSize(72);
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

	buttonBack = gui::Button({1050, 630}, { 180, 40 }, "Back", font, Back);*/

	return true;
}
