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

		void pushState(dgm::AppState *state);
		void popState();
		dgm::AppState *topState();
		void init();
		void deinit();
		void run();
	};
};