#include <DGM\dgm.hpp>
#include <cstdio>

class AppEnvironment {
private:
	bool status;

public:
	bool isLoggingEnabled() const {
		return status;
	}

	AppEnvironment() {
		status = bool(freopen("stdout.txt", "w", stdout));
		status &= bool(freopen("stderr.txt", "w", stderr));
	}
};

const AppEnvironment ENVIRONMENT;

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
	scheduleCleanup = true;
}

dgm::AppState * dgm::App::topState() {
	if (states.empty()) return nullptr;

	return states.top();
}

void dgm::App::init() {
	scheduleCleanup = false;
}

void dgm::App::deinit() {
	while (not states.empty()) {
		delete topState();
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

		if (scheduleCleanup) {
			delete top;
			scheduleCleanup = false;
		}
	}
}

dgm::App::App() {
	if (!ENVIRONMENT.isLoggingEnabled()) {
		exit(1);
	}

	init();
	time.reset();
}

dgm::App::~App() {
	deinit();
}
