#include <DGM\dgm.hpp>

using dgm::Molded::App;
using dgm::Molded::AppState;

void App::pushState(AppState *state) {
	states.push(state);
	states.top->setApp(this);

	if (not states.top()->init()) {
		popState();
	}
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

void App::init(const sf::VideoMode &videoMode, const std::string &name, const int style, const int fps, const bool multithreaded) {
	window.create(videoMode, name, style);

	if (fps > 0) {
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(fps);
	}
	else {
		window.setVerticalSyncEnabled(true);
	}

	if (not multithreaded) {
		pushState(new DefaultState());
	}
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

bool dgm::Molded::DefaultState::init() {
	return true;
}

// *** DEFAULT STATE IMPLEMENTATION ***
void dgm::Molded::DefaultState::draw() {
}

void dgm::Molded::DefaultState::update() {
	app->window.close();
}

void dgm::Molded::DefaultState::input() {
}

dgm::Molded::DefaultState::DefaultState(App * app) {
	AppState::app = app;
}

void dgm::Molded::AppState::setApp(App * app) {
	AppState::app = app;
}
