/**
 *  \file example08.cpp
 *
 *  \details This example showcases dgm::Buffer and advanced collisions.
 *  dgm::Mesh is used to define solid borders for edges of the screen.
 *  Then many balls and squares is spawned and launched into space. Squares
 *  will stop when touching borders (mesh) due to basic collision used.
 *  Balls use advanced collisions to bounce from the borders.
 *  You can also delete balls by pointing at them and pressing left mouse
 *  button and spawn them back in the middle of the screen by pressing
 *  right mouse button. This showcases deleting from/adding to dgm::Buffer
 *  which happens in O(1) time.
 */

#define NO_NETWORK

#include <DGM\dgm.hpp>
#include <ctime>

const float SPEED_BALL = 64.f;
const float SPEED_SQUARE = 32.f;
const int BALL_COUNT = 32;
const int SQUARE_COUNT = 32;

class Ball {
private:
	dgm::Circle body;
	sf::CircleShape sprite;
	sf::Vector2f forward;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	bool clicked(const sf::Vector2i &cursorPos) const {
		auto diff = sf::Vector2f(cursorPos) - body.getPosition();
		float distance = diff.x * diff.x + diff.y * diff.y;
		return distance <= (body.getRadius() * body.getRadius());
	}

	void update(const dgm::Time &time, const dgm::Mesh &level) {
		auto currentForward = forward * time.getDeltaTime();
		
		if (dgm::Collision::advanced(level, body, currentForward)) {
			// Bounce
			if (currentForward.x == 0.f) forward.x *= -1.f;
			if (currentForward.y == 0.f) forward.y *= -1.f;
		}

		body.move(currentForward);
		sprite.move(currentForward);
	}

	void spawn(const sf::Vector2u &windowSize) {
		sf::Vector2f pos = sf::Vector2f(windowSize / 2u);

		forward = dgm::Conversion::polarToCartesian(float(std::rand() % 360), SPEED_BALL);

		sprite.setPosition(pos);
		body.setPosition(pos);
	}

	Ball() {
		sprite.setRadius(10.f);
		sprite.setFillColor(sf::Color::Green);
		sprite.setOrigin(10.f, 10.f);

		body.setRadius(10.f);
	}
};

class Square {
private:
	dgm::Rect body;
	sf::RectangleShape sprite;
	sf::Vector2f forward;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time, const dgm::Mesh &level) {
		auto currentForward = forward * time.getDeltaTime();

		// Stop moving upon collision
		if (!dgm::Collision::basic(level, body)) {
			body.move(currentForward);
			sprite.move(currentForward);
		}
	}

	void spawn(const sf::Vector2u &windowSize) {
		sf::Vector2f pos = sf::Vector2f(windowSize / 2u);

		forward = dgm::Conversion::polarToCartesian(float(std::rand() % 360), 32.f);

		sprite.setPosition(pos);
		body.setPosition(pos);
	}

	Square() {
		sprite.setSize({ 20.f, 20.f });
		sprite.setFillColor(sf::Color::Red);
		
		body.setSize(20.f, 20.f);
	}
};

int main(int argc, char *argv[]) {
	srand(unsigned(time(NULL)));

	sf::Vector2u windowSize(1280, 720);
	dgm::Window window(windowSize, "Sandbox", false);
	dgm::Time time;

	dgm::Buffer<Ball> balls(BALL_COUNT);
	for (unsigned i = 0; i < balls.capacity(); i++) {
		balls.expand(); // At this point, it will succeed
		balls[i].spawn(windowSize);
	}

	dgm::Buffer<Square> squares(SQUARE_COUNT);
	for (unsigned i = 0; i < squares.capacity(); i++) {
		// We can do following two lines in reverse order, because all
		// items in the buffer exist all the time and we can directly
		// index them.
		// But when range loops or Buffer::size() is used, those
		// items will not be iterated unless "allocated" via
		// Buffer::expand
		squares[i].spawn(windowSize);
		squares.expand(); // At this point, it will succeed
	}

	dgm::Mesh level;
	level.setDataSize({ 3, 3 });
	level.setVoxelSize(windowSize);
	level.setPosition(-sf::Vector2f(windowSize));
	std::vector<int> levelData = { 1, 1, 1, 
					               1, 0, 1, 
		                           1, 1, 1 };

	for (unsigned i = 0; i < levelData.size(); i++) {
		level[i] = levelData[i];
	}

	sf::Vector2i mousePos;
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					mousePos = sf::Mouse::getPosition(window.getWindowContext());

					/* TWO WAYS OF WRITING THE LOOP
					for (unsigned i = 0; i < balls.end(); i++) {
						if (balls[i].clicked(mousePos)) {
							balls.remove(i);
							i--;
						}
					}*/
					for (auto ball = balls.begin(); ball != balls.end(); ball++) {
						if ((*ball).clicked(mousePos)) {
							balls.remove(ball);
							ball--;
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					if (balls.expand()) { // Only succeeds when size < capacity
						balls.last().spawn(windowSize); // Last is newly added item
					}
				}
			}
		}

		/* LOGIC */
		time.reset();

		for (auto &ball : balls) {
			ball.update(time, level);
		}

		for (auto &square : squares) {
			square.update(time, level);
		}

		/* RENDER */
		window.beginDraw();

		sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Red);
		sf::Vector2f size(level.getVoxelSize());
		shape.setSize(size);
		for (unsigned y = 0; y < level.getDataSize().y; y++) {
			for (unsigned x = 0; x < level.getDataSize().x; x++) {
				if (level[y * 3 + x] > 0) {
					shape.setPosition(sf::Vector2f(x * size.x, y * size.y) + level.getPosition());
					window.draw(shape);
				}
			}
		}

		for (auto &square : squares) {
			square.draw(window);
		}

		for (auto &ball : balls) {
			ball.draw(window);
		}

		window.endDraw();
	}

	return 0;
}