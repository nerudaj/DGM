/**
 *  \file example07.cpp
 *
 *  \details This example showcases usage of dgm::Controller and
 *  collisions between dgm::Rect and dgm::Circle. You can move the
 *  ball using directional keys and when it collides with the blue
 *  rectangle, the ball turn red.
 */

#define NO_NETWORK

#include <DGM\dgm.hpp>
#include <ctime>

enum {
	Up, Down, Left, Right
};

const float SPEED = 96.f;

class Square {
private:
	dgm::Rect body;
	sf::RectangleShape sprite;
	sf::Vector2f forward;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void spawn(const sf::Vector2u &windowSize) {
		sf::Vector2f pos = sf::Vector2f(windowSize / 2u) - sf::Vector2f(50.f, 50.f);

		sprite.setPosition(pos);
		body.setPosition(pos);
	}

	const dgm::Rect &getBody() const {
		return body;
	}

	Square() {
		sprite.setSize({ 100.f, 100.f });
		sprite.setFillColor(sf::Color::Blue);

		body.setSize(100.f, 100.f);
	}
};

class Ball {
private:
	dgm::Circle body;
	sf::CircleShape sprite;
	sf::Vector2f forward;
	dgm::Controller input;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time) {
		sprite.setFillColor(sf::Color::Green);

		if (input.keyPressed(Left)) {
			forward.x = -SPEED;
		}
		else if (input.keyPressed(Right)) {
			forward.x = SPEED;
		}
		if (input.keyPressed(Up)) {
			forward.y = -SPEED;
		}
		else if (input.keyPressed(Down)) {
			forward.y = SPEED;
		}

		auto currentForward = forward * time.getDeltaTime();

		body.move(currentForward);
		sprite.move(currentForward);

		forward = { 0.f, 0.f };
	}

	void collision(const Square &sq) {
		if (dgm::Collision::basic(sq.getBody(), body)) {
			sprite.setFillColor(sf::Color::Red);
		}
	}

	void spawn() {
		sf::Vector2f pos(100.f, 100.f);

		forward = { 0.f, 0.f };

		sprite.setPosition(pos);
		body.setPosition(pos);
	}

	Ball() {
		input.bindKeyboardKey(Left, sf::Keyboard::Left);
		input.bindKeyboardKey(Right, sf::Keyboard::Right);
		input.bindKeyboardKey(Up, sf::Keyboard::Up);
		input.bindKeyboardKey(Down, sf::Keyboard::Down);

		sprite.setRadius(50.f);
		sprite.setFillColor(sf::Color::Green);
		sprite.setOrigin(50.f, 50.f);

		body.setRadius(50.f);
	}
};

int main(int argc, char *argv[]) {
	sf::Vector2u windowSize(1280, 720);
	dgm::Window window(windowSize, "Sandbox", false);
	dgm::Time time;

	Ball ball;
	ball.spawn();
	Square square;
	square.spawn(windowSize);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		/* LOGIC */
		time.reset();

		ball.update(time);
		ball.collision(square);

		/* RENDER */
		window.beginDraw();

		square.draw(window);
		ball.draw(window);

		window.endDraw();
	}

	return 0;
}