#pragma once

#include <DGM\dgm.hpp>

#include <stack>

namespace dgm {
	class AppState;

	class App {
	protected:
		std::stack<AppState*> states;

	public:
		Window window;
		Time time;

		void PushState(dgm::AppState *state);
		void PopState();
		dgm::AppState *TopState();
		void Init();
		void Deinit();
		void Run();
	};
};