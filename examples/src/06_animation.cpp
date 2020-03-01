/**
*  \file example06.cpp
*
*  \details This example showcases the basic usage of the animation object. It uses files
*  soldier.png and soldier.json from the examples folder. soldier.json contains the
*  definitions of animation states, which internally are represented as dgm::Clip.
*
*  Study the json to get a better grasp of how this works. Notice the values in the "defaults"
*  section of the file. This section is completely optional but lets you skip
*  repeated definitions in the "states" section. Notice how state "idle-right"
*  defines everything, even though the "frame" values do not differ from defaults.
*  Thus, other states simply omit the redundant information altogether.
*
*  There are also more advanced things that can be done with animations and they
*  will be handled in the next examples.
*/

#define NO_NETWORK

#include <DGM/dgm.hpp>

#include "RootDir.hpp"

class Player {
private:
	sf::Texture texture;
	sf::Sprite sprite;
	dgm::Animation animation;
	dgm::Controller input;
	int direction;

public:
	enum {
		Left, Right
	};

	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time) {
		animation.update(time);

		if (input.keyPressed(Left)) {
			// You can repeatedly set the same state, it affect the running animation
			// unless you call reset() explicitly
			animation.setState("run-left");
			direction = Left;
		}
		else if (input.keyPressed(Right)) {
			animation.setState("run-right");
			direction = Right;
		}
		else {
			if (direction == Left) {
				animation.setState("idle-left");
			}
			else if (direction == Right) {
				animation.setState("idle-right");
			}
		}
	}

	void spawn(const sf::Vector2f &position) {
		sprite.setPosition(position);
	}

	Player(const std::string &texturepath, const std::string &animpath) : animation(animpath) {
		if (!texture.loadFromFile(texturepath)) {
			throw dgm::GeneralException("Cannot load texture " + texturepath);
		}

		// First, bind texture to sprite
		sprite.setTexture(texture);

		// Second, bind sprite to animation
		animation.bindSprite(&sprite);

		// Tune in default parameters of animation
		animation.setSpeed(4);
		animation.setState("idle-left");
		direction = Left;

		input.setBinding(Left, sf::Keyboard::Left, dgm::X360::LStick_Left);
		input.setBinding(Right, sf::Keyboard::Right, dgm::X360::LStick_Right);
	}
};

int main(int argc, char *argv[]) {
	cfg::Args args("d:");
	try {
		args.parse(argc, argv);
	}
	catch (...) {
		return 1;
	}

	dgm::Window window({ 1280, 720 }, "Animation example", false);
	dgm::Time time;
	
	Player player(rootDir + "/soldier.png", rootDir + "/soldier.json");
	player.spawn({ 1280 / 2.f, 720 / 2.f });

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
		player.update(time);

		time.reset();

		// Rendering
		window.beginDraw(sf::Color::Black);

		player.draw(window);

		window.endDraw();
	}
	
	return 0;
}