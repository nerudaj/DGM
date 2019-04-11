#include <DGM\dgm.hpp>

void dgm::App::pushState(dgm::AppState * state) {
	states.push(state);

	topState()->setAppPointer(this);
	if (not topState()->init()) {
		std::cerr << "ERROR:App: State was not initialized properly.\n";
		popState();
	}
}

void dgm::App::popState() {
	states.pop();
}

dgm::AppState * dgm::App::topState() {
	if (states.empty()) return nullptr;

	return states.top();
}

void dgm::App::init() {
}

void dgm::App::deinit() {
	while (not states.empty()) {
		popState();
	}
}

void dgm::App::run() {
	while (window.isOpen() && not states.empty()) {
		AppState *top = topState();

		top->input();
		top->update();
		top->draw();
		time.reset();
	}
}

dgm::App::App() {
	init();
	time.reset();
}

dgm::App::~App() {
	deinit();
}