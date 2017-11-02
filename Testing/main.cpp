#include <DGM\dgm.hpp>

#ifdef _DEBUG
#pragma comment(lib, "dgm-d.lib")
#else
#pragma comment(lib, "dgm.lib")
#endif

const int FPS = 40;
const int WIDTH = 1280;
const int HEIGHT = 720;

class ExceptState : public dgm::Molded::AppState {
protected:
	std::vector<int> values;

public:
	// Inherited via AppState
	virtual bool init() override {
		try {
			values[10] = 11;
		}
		catch (...) {
			return false;
		}

		return true;
	}

	virtual void draw() override {
	}

	virtual void update() override {
	}

	virtual void input() override {
	}

	ExceptState(dgm::Molded::App *app) {
		dgm::Molded::AppState::app = app;
	}
};

/* This is just an internal testing sandbox */
int main(int argc, char *argv[]) {
	dgm::Molded::App app;
	app.init(sf::VideoMode(WIDTH, HEIGHT), "Sandbox", sf::Style::Default);

	app.pushState(new ExceptState(&app));
	app.loop();

	return 0;
}