#include <DGM\dgm.hpp>

#ifdef _DEBUG
#pragma comment(lib, "dgm-d.lib")
#else
#pragma comment(lib, "dgm.lib")
#endif

const int FPS = 40;
const int WIDTH = 1280;
const int HEIGHT = 720;

/* This is just an internal testing sandbox */
int main(int argc, char *argv[]) {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "ITU", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(40);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		// LOGIC
		// DRAW
	}

	return 0;
}