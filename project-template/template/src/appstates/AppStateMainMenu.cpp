#include "AppStateMainMenu.hpp"
#include "AppStateMenuOptions.hpp"
#include "AppStateIngame.hpp"

void AppStateMainMenu::buildLayout() {
	auto title = createOptionLabel({ "0%", "5%" }, { "100%", "25%" }, "Project Title");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setTextSize(72);
	gui.add(title);

	createButton("Play", { "42.5%", "35%" }, { "15%", "5%" }, [this]() { app->pushState(new AppStateIngame(resmgr, settings)); });
	createButton("Options", { "42.5%", "41%" }, { "15%", "5%" }, [this]() {
		app->pushState(new AppStateMenuOptions(resmgr, settings));
		viewShouldBeUpdated = true; // In case resolution had changed in options
		// Note that AppStateMainMenu::update will be triggered before options
		// will be displayed. Thus, the view has to be updated at the beginning
		// of AppStateMainMenu::input method
	});
	createButton("Exit", { "42.5%", "47%" }, { "15%", "5%" }, [this]() { app->popState(); });
}

void AppStateMainMenu::input() {
	// NOTE: Has to be here, not in update
	if (viewShouldBeUpdated) {
		gui.setView(app->window.getWindowContext().getView());
		viewShouldBeUpdated = false;
	}

	sf::Event event;
	while (app->window.pollEvent(event)) {
		gui.handleEvent(event);
	}
}

void AppStateMainMenu::update() {}

void AppStateMainMenu::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateMainMenu::init() {
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}