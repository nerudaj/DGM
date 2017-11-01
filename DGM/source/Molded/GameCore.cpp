#include <DGM\dgm.hpp>

using dgm::Molded::App;
using dgm::Molded::AppState;

void App::pushState(AppState *state) {
	states.push(state);
}

void App::popState() {
	delete states.top();
	states.pop();
}

AppState *App::peekState() {
	if (states.empty()) {
		return nullptr;
	}

	return states.top();
}

void App::changeState(AppState *state) {
	if (not states.empty()) {
		popState();
	}

	pushState(state);
}

void App::loop() {
	while (window.isOpen()) {
		if (peekState() == nullptr) continue;

		peekState()->input();
		peekState()->update();

		window.clear();
		peekState()->draw();
		window.display();
	}
}

void App::init(const int APP_WIDTH, const int APP_HEIGHT, const std::string &APP_NAME, const int APP_STYLE, const int APP_FPS) {
	window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_NAME, APP_STYLE);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(APP_FPS);
}

void App::deinit() {
	while (not states.empty()) {
		popState();
	}
}

App::App() {}
App::~App() {
	deinit();
}