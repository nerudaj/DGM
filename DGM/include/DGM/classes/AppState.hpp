#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class AppState {
	protected:
		App *app;

	public:
		virtual void Input() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual bool Init() = 0;

		void SetAppPointer(App *app) {
			AppState::app = app;
		}
	};
};