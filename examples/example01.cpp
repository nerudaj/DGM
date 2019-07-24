/**
 *  \file example01.cpp
 *  
 *  \details This example shows basic workflow with both SFML and DGM.
 *  It opens up a window which can be closed with close button and tracks
 *  the mouse as it travels over the window.
 *  
 *  It tracks the mouse using line which aims at mouse and a dot placed 
 *  under the mouse. For this polar<->cartesian conversion functions are
 *  used (only for demonstration, of course it can be programmed more effectively).
 */

// Disallow linkage of sfml network module
#define NO_NETWORK

#include <DGM/dgm.hpp>

int main() {
	dgm::Window window({1280,  720}, "Sandbox", false);

	//  Center of the window
	sf::Vector2f windowCenter(640.f, 360.f);

	// Create rectangle which will aim to where mouse is positioned
	sf::RectangleShape rect({ 200.f, 5.f });
	rect.setFillColor(sf::Color::White);
	rect.setPosition(windowCenter);
	rect.setOrigin({ 0.f, 2.5f });

	// Circle shape which follows the mouse
	sf::CircleShape circ(5.f);
	circ.setFillColor(sf::Color::Green);

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
		auto mousePos = sf::Mouse::getPosition(window.getWindowContext());

		// Compute polar coordinates to mouse relative to center of window
		auto toMouseAsPolar = dgm::Conversion::cartesianToPolar(sf::Vector2f(mousePos) - windowCenter);

		// Set rotation of rect to face the mouse
		rect.setRotation(toMouseAsPolar.x);

		// Do reverse computation and place dot under the mouse
		circ.setPosition(dgm::Conversion::polarToCartesian(toMouseAsPolar) + windowCenter);

		// Rendering
		window.beginDraw();

		window.draw(rect);
		window.draw(circ);

		window.endDraw();
	}
	return 0;
}