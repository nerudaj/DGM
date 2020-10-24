#include "AppStateBootstrap.hpp"
#include "AppStateMainMenu.hpp"

void AppStateBootstrap::input() {}

void AppStateBootstrap::update() {
	if (state == State::Enter) {
		app->pushState(new AppStateMainMenu(resmgr, settings));
		state = State::Exit;
	} else if (state == State::Exit) {
		app->popState();
	}
}

void AppStateBootstrap::draw() {}

bool AppStateBootstrap::init() {
	try {
		resmgr.loadResourceDir<sf::Texture>(rootDir + "/graphics/textures");
		resmgr.loadResourceDir<sf::Font>(rootDir + "/graphics/fonts");
		resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(rootDir + "/graphics/configs");
		resmgr.loadResourceDir<sf::SoundBuffer>(rootDir + "/audio/sounds");
	}
	catch (std::exception & e) {
		std::cerr << "error:AppStateMainMenu: " << e.what() << std::endl;
		return false;
	}

	return true;
}
