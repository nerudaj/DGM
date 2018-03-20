#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class AppState {
	protected:
		App *app;

	public:
		virtual void input() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual bool init() = 0;

		void setAppPointer(App *app) {
			AppState::app = app;
		}
	};
};