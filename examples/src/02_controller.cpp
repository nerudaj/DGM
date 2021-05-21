/**
 *  \file 02_controller.cpp
 *
 *  \details This example shows how to work with input
 *  dgm::Controller object and dgm::Time objects for
 *  precise update computation.
 *
 *  This example creates a drivable "car" to show these
 *  concepts.
 *
 *  NOTE: If you parametrize things, you can replace
 *  dgm::Controller with your implementation of
 *  dgm::AbstractController interface and thus create an
 *  AI which will drive the car for you!
 */

// Disallow linkage of sfml network module
#define NO_NETWORK

#include <DGM/dgm.hpp>

// Numeric codes for controll keys
// These will be bind to concrete keys in Car ctor
enum {
	GAS   = 0,
	BRAKE = 1,
	LEFT  = 2,
	RIGHT = 3
};

class Car {
private:
	sf::ConvexShape car;  // Graphic of car
	dgm::Controller ctrl; // User input
	float speed;          // Current speed of car
	float angle;          // Current angle of car

	const float Acceleration; // Pixels per second
	const float Drag;         // Pixels per second
	const float MaxSpeed;     // Pixels per second
	const float TurnSpeed;    // Degrees per second

public:
	void draw(dgm::Window &window) {
		window.draw(car);
	}

	void update(dgm::Time &time) {
		// Compute new speed based on inputs or their lack thereof
		float newSpeed = 0.f;
		if (ctrl.keyPressed(GAS)) {
			newSpeed = Acceleration * time.getDeltaTime();
		}
		else if (ctrl.keyPressed(BRAKE)) {
			newSpeed = -Acceleration * time.getDeltaTime();
		}
		else { // If car does nothing, it will slow down by itself
			newSpeed = -Drag * time.getDeltaTime();
		}

		speed = dgm::Math::clamp(speed + newSpeed, 0.f, MaxSpeed);

		// Turn the card as needed
		if (ctrl.keyPressed(LEFT)) {
			angle -= TurnSpeed * time.getDeltaTime();
		}
		else if (ctrl.keyPressed(RIGHT)) {
			angle += TurnSpeed * time.getDeltaTime();
		}

		car.setRotation(angle);

		// Translate position of car
		car.move(dgm::Conversion::polarToCartesian(angle, speed * time.getDeltaTime()));
	}

	void spawn(const sf::Vector2f &position) {
		car.setPosition(position);
		angle = 0.f;
		speed = 0.f;
	}

	Car(float Acceleration, float Drag, float MaxSpeed, float TurnSpeed) : Acceleration(Acceleration), Drag(Drag), MaxSpeed(MaxSpeed), TurnSpeed(TurnSpeed) {
		// This creates just shape of our car
		car.setPointCount(5);
		car.setPoint(0, { 0.f, 0.f });
		car.setPoint(1, { 20.f, 0.f });
		car.setPoint(2, { 25.f, 5.f });
		car.setPoint(3, { 20.f, 10.f });
		car.setPoint(4, { 0.f, 10.f });
		car.setOrigin({ 12.5f, 5.f });
		car.setFillColor(sf::Color::Green);

		// This will bind control codes to keyboard keys
		ctrl.bindKeyboardKey(GAS,   sf::Keyboard::W);
		ctrl.bindKeyboardKey(BRAKE, sf::Keyboard::S);
		ctrl.bindKeyboardKey(LEFT,  sf::Keyboard::A);
		ctrl.bindKeyboardKey(RIGHT, sf::Keyboard::D);
	}
};

int main() {
	dgm::Window window({1280,  720}, "Sandbox", false);

	//  Center of the window
	sf::Vector2f windowCenter(640.f, 360.f);

	// Create car object. Try to tweak the properties!
	Car car(32.f, 24.f, 128.f, 45.f);
	car.spawn(windowCenter);

	dgm::Time time; // This measures time between frames

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
		car.update(time);
		time.reset(); // Timer reset *must* be the last thing you do in logic

		// Rendering
		window.beginDraw();
		
		car.draw(window);

		window.endDraw();
	}
	return 0;
}