#include <DGM\dgm.hpp>

void dgm::App::PushState(dgm::AppState * state) {
	states.push(state);

	if (not TopState()->Init()) {
		std::cerr << "ERROR:App: State was not initialized properly.\n";
		PopState();
	}
	TopState()->SetAppPointer(this);
}

void dgm::App::PopState() {
	states.pop();
}

dgm::AppState * dgm::App::TopState() {
	if (states.empty()) return nullptr;

	return states.top();
}

void dgm::App::Init() {
}

void dgm::App::Deinit() {
	while (not states.empty()) {
		PopState();
	}
}

void dgm::App::Run() {
	while (window.IsOpen() && not states.empty()) {
		AppState *top = TopState();

		top->Input();
		top->Update();
		top->Draw();
		time.Reset();
	}
}
