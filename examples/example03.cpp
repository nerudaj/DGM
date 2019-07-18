/**
 *  \file example03.cpp
 *
 *  \details This example builds off previous one,
 *  using AppState pattern and AbstractController.
 *
 *  \note You can exit this one only by pressing Esc!
 */

// Disallow linkage of sfml network module
#define NO_NETWORK

#include <DGM/dgm.hpp>

// Numeric codes for controll keys
// These will be bind to concrete keys in Car ctor
enum {
	GAS = 0,
	BRAKE = 1,
	LEFT = 2,
	RIGHT = 3
};

class Car {
private:
	sf::ConvexShape          car;  // Graphic of car
	dgm::AbstractController *ctrl; // User input, abstract this time, try to write AI!
	float                    speed;// Current speed of car
	float                    angle;// Current angle of car

	float Acceleration; // Pixels per second
	float Drag;         // Pixels per second
	float MaxSpeed;     // Pixels per second
	float TurnSpeed;    // Degrees per second

public:
	void draw(dgm::Window &window) {
		window.draw(car);
	}

	void update(dgm::Time &time) {
		// Compute new speed based on inputs or their lack thereof
		float newSpeed = 0.f;
		if (ctrl->keyPressed(GAS)) {
			newSpeed = Acceleration * time.getDeltaTime();
		}
		else if (ctrl->keyPressed(BRAKE)) {
			newSpeed = -Acceleration * time.getDeltaTime();
		}
		else { // If car does nothing, it will slow down by itself
			newSpeed = -Drag * time.getDeltaTime();
		}

		speed = dgm::Math::clamp(speed + newSpeed, 0.f, MaxSpeed);

		// Turn the card as needed
		if (ctrl->keyPressed(LEFT)) {
			angle -= TurnSpeed * time.getDeltaTime();
		}
		else if (ctrl->keyPressed(RIGHT)) {
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

	// Unlike previous example, now we need to define setup method and pass settings that way
	void setup (float Acceleration, float Drag, float MaxSpeed, float TurnSpeed, dgm::AbstractController *input) {
		Car::Acceleration = Acceleration;
		Car::Drag = Drag;
		Car::MaxSpeed = MaxSpeed;
		Car::TurnSpeed = TurnSpeed;
		Car::ctrl = input;
	}

	Car() {
		// This creates just shape of our car
		car.setPointCount(5);
		car.setPoint(0, { 0.f, 0.f });
		car.setPoint(1, { 20.f, 0.f });
		car.setPoint(2, { 25.f, 5.f });
		car.setPoint(3, { 20.f, 10.f });
		car.setPoint(4, { 0.f, 10.f });
		car.setOrigin({ 12.5f, 5.f });
		car.setFillColor(sf::Color::Green);
	}
};

// Main state of the application
// There is only single state in this example, but in real app
// you want to have multiple states, which then will be pushed/poped
// from state stack as need
class StateIngame : public dgm::AppState {
private:
	Car car;
	dgm::Controller carInput;

public:
	// Inherited from AppState
	virtual void input() override {
		sf::Event event;

		// Apart from method definitions, pointer to app is passed too
		// App contains window and time
		while (app->window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					app->popState(); // Destroy this state and restore anything that was before it
					// Before it could have been main menu but in this example
					// the stack contain no more items meaning the app will finish
				}
			}
		}
	}

	virtual void update() override {
		car.update(app->time);
		// No need to update timer manually this time
	}

	virtual void draw() override {
		app->window.beginDraw();

		car.draw(app->window);

		app->window.endDraw();
	}

	virtual bool init() override {
		// This will bind control codes to keyboard keys
		// Third argument for X360 controller is optional, but recommended
		carInput.setBinding(GAS, sf::Keyboard::W, dgm::X360::RTrigger);
		carInput.setBinding(BRAKE, sf::Keyboard::S, dgm::X360::LTrigger);
		carInput.setBinding(LEFT, sf::Keyboard::A, dgm::X360::LStick_Left);
		carInput.setBinding(RIGHT, sf::Keyboard::D, dgm::X360::LStick_Right);

		car.setup(32.f, 24.f, 128.f, 45.f, &carInput);
		car.spawn(sf::Vector2f(app->window.getSize() / unsigned(2)));
		return true;
	}
};

int main() {
	dgm::App app;
	app.window.open({ 1280,  720 }, "Sandbox", false);

	// Each state should push new states to stack dynamically
	// You need to initialize only the bottom most one
	app.pushState(new StateIngame());

	// When no more states are on the stack, this method will exit
	app.run();

	// By explicitly closing window, we can save the used settings into
	// file which can be loaded on startup and window can be initialized
	// using config it previously ended up with
	cfg::Ini cfg;
	app.window.close(cfg);
	cfg.saveToFile("window.ini");

	// You can also add other settings to this config if you wish

	return 0;
}