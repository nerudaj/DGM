#include "AppStateMainMenu.hpp"
#include "AppStateMenuOptions.hpp"
#include "AppStateIngame.hpp"

void AppStateMainMenu::buildLayout() {
	tgui::Label::Ptr title = tgui::Label::create("Project Name");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	title->setTextSize(72);
	title->setSize("100%", "25%");
	title->setPosition("0%", "5%");
	gui.add(title, "LabelGameTitle");

	tgui::Button::Ptr playButton = tgui::Button::create("Play");
	playButton->setSize("15%", "5%");
	playButton->setPosition("42.5%", "35%");
	playButton->connect("pressed", [this]() { app->pushState(new AppStateIngame(resmgr, settings)); });
	gui.add(playButton, "ButtonPlay");

	tgui::Button::Ptr optionsButton = tgui::Button::create("Options");
	optionsButton->setSize("15%", "5%");
	optionsButton->setPosition("42.5%", "41%");
	optionsButton->connect("pressed", [this]() {
		app->pushState(new AppStateMenuOptions(resmgr, settings)); 
		viewShouldBeUpdated = true; // In case resolution had changed in options
	});
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

void AppStateMainMenu::update() {
	if (viewShouldBeUpdated) {
		gui.setView(app->window.getWindowContext().getView());
		viewShouldBeUpdated = false;
	}
}

void AppStateMainMenu::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateMainMenu::init() {
	viewShouldBeUpdated = true;

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

	settings.soundVolume = 50.f;
	settings.musicVolume = 50.f;

	gui.setFont(resmgr.get<sf::Font>("cruft.ttf"));
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}