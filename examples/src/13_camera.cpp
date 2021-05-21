/*
	This example showcases how dgm::Camera can be used to manipulate sf::View with some predefined effects.
	Left view is directly controlled with arrow keys as in regular game.
	Right view demonstrates camera effects:
		* M - Animated transition
		* R - Animated rotation
		* Z - Animated zoom
		* S - small screenshake
		* B - huge screenshake, gradually settling in
*/

#define NO_NETWORK

#include <DGM/dgm.hpp>
#include "Level.hpp"
#include "RootDir.hpp"

sf::View createView(const sf::Vector2f& centerPos, bool leftHalf, const dgm::Window& w) {
	sf::View result;
	result.setViewport(sf::FloatRect(leftHalf ? 0.f : 0.5f, 0.f, 0.5f, 1.f));
	result.setSize(w.getSize().x / 2, w.getSize().y);
	result.setCenter(centerPos);
	return result;
}

class Player {
private:
	dgm::Camera& camera;
	dgm::Controller input;
	const float SPEED = 128.f;

	enum {
		Up, Down, Left, Right
	};

public:
	void update(const dgm::Time& time) {
		sf::Vector2f forward;
		if (input.keyPressed(Up)) {
			forward.y = -SPEED;
		}
		else if (input.keyPressed(Down)) {
			forward.y = SPEED;
		}
		if (input.keyPressed(Left)) {
			forward.x = -SPEED;
		}
		else if (input.keyPressed(Right)) {
			forward.x = SPEED;
		}

		forward *= time.getDeltaTime();
		camera.move(forward);
	}

	Player(dgm::Camera& camera) : camera(camera) {
		input.bindKeyboardKey(Up, sf::Keyboard::Up);
		input.bindKeyboardKey(Down, sf::Keyboard::Down);
		input.bindKeyboardKey(Left, sf::Keyboard::Left);
		input.bindKeyboardKey(Right, sf::Keyboard::Right);
	}
};

int main(int argc, char *argv[]) {
	dgm::Window window({1280, 720}, "Example 13", false);
	dgm::Time time;

	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false);
	resmgr.loadResourceDir<sf::Texture>(rootDir);

	Level level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile(rootDir + "/12_simple_lights.lvd");

	auto viewLeft = createView({ 100.f, 100.f }, true, window);
	auto viewRight = createView({ 200.f, 100.f }, false, window);

	dgm::Camera cam1(viewLeft);
	dgm::Camera cam2(viewRight);

	Player player(cam1);

	// These are fixed positions and zoom levels between which
	// cam2 alternates
	int camControl = 1;
	int zoomControl = 1;
	sf::Vector2f points[] = {
		{200.f, 100.f},
		{1000.f, 500.f},
		{1200.f, 300.f}
	};
	float zooms[] = { 1.f, 2.f, 0.5f };

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::M && !cam2.isMoving()) {
					cam2.moveGradually(points[camControl], sf::seconds(2.3f), [](float x) {
						return x * x;
					});
					camControl = (camControl + 1) % 3;
				}
				else if (event.key.code == sf::Keyboard::Z && !cam2.isZooming()) {
					cam2.zoomGradually(zooms[zoomControl], sf::seconds(1.5f), [](float x) {
						return x * x;
					});
					zoomControl = (zoomControl + 1) % 3;
				}
				else if (event.key.code == sf::Keyboard::R && !cam2.isRotating()) {
					cam2.rotateGradually(359.f, sf::seconds(3.f), [](float x) { return x * x * x; });
				}
				else if (event.key.code == sf::Keyboard::S) {
					cam2.shake(sf::seconds(1.f), 10.f);
				}
				else if (event.key.code == sf::Keyboard::B) {
					cam2.shake(sf::seconds(5.f), 50.f, [](float x) { return 1 - x * x; });
				}
			}
		}

		/* LOGIC */
		time.reset();

		player.update(time);
		cam1.update(time);
		cam2.update(time);

		/* DRAW */
		window.beginDraw();

		// Draw to first view
		window.getWindowContext().setView(viewLeft);
		level.draw(window);

		// Draw to second view
		window.getWindowContext().setView(viewRight);
		level.draw(window);

		window.endDraw();
	}

	return 0;
}
