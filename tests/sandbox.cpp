// Disallow linkage of sfml audio and network modules
#define NO_AUDIO
#define NO_NETWORK

#include <DGM/dgm.hpp>

int main() {
	dgm::Window window({1280,  720}, "Sandbox", false);

	//  Center of the window
	sf::Vector2f windowCenter(640.f, 360.f);

	// Main loop
	sf::Event event;
	while (window.isOpen()) {
		// Poll events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Logic

		// Rendering
		window.beginDraw();

		window.endDraw();
	}
	return 0;
}