#include "AppStateMainMenu.hpp"
#include "AppStateMenuOptions.hpp"
#include "AppStateIngame.hpp"

void AppStateMainMenu::buildLayout() {
	tgui::Label::Ptr label = tgui::Label::create("Project Name");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	label->setTextSize(72);
	label->setSize("50%", "25%");
	label->setPosition("25%", "5%");
	gui.add(label, "LabelGameTitle");

	tgui::Button::Ptr playButton = tgui::Button::create("Play");
	playButton->setSize("15%", "5%");
	playButton->setPosition("42.5%", "35%");
	playButton->connect("pressed", [this]() { app->pushState(new AppStateIngame(resmgr)); });
	gui.add(playButton, "ButtonPlay");

	tgui::Button::Ptr optionsButton = tgui::Button::create("Options");
	optionsButton->setSize("15%", "5%");
	optionsButton->setPosition("42.5%", "41%");
	optionsButton->connect("pressed", [this]() { app->pushState(new AppStateMenuOptions(resmgr)); });
	gui.add(optionsButton, "ButtonOptions");

	tgui::Button::Ptr exitButton = tgui::Button::create("Exit");
	exitButton->setSize("15%", "5%");
	exitButton->setPosition("42.5%", "47%");
	exitButton->connect("pressed", [this]() { app->popState(); });
	gui.add(exitButton, "ButtonExit");
}

void AppStateMainMenu::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		gui.handleEvent(event);
	}
}

void AppStateMainMenu::update() {}

void AppStateMainMenu::draw() {
	app->window.beginDraw();

	gui.draw();

	app->window.endDraw();
}

bool AppStateMainMenu::init() {
	gui.setTarget(app->window.getWindowContext());

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

	gui.setFont(resmgr.get<sf::Font>("cruft.ttf"));

	buildLayout();

	return true;
}