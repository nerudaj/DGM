#include "AppStateIngame.hpp"

void AppStateIngame::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				app->popState();
			}
			else if (event.key.code == sf::Keyboard::B) {
				soundbox.playSound("bounce.wav", 0);
			}
			else if (event.key.code == sf::Keyboard::K) {
				soundbox.playSound("kick.wav", 1);
			}
			else if (event.key.code == sf::Keyboard::A) {
				actor.setAnimationState("run-left");
			}
			else if (event.key.code == sf::Keyboard::D) {
				actor.setAnimationState("run-right");
			}
		}
	}
}

void AppStateIngame::update() {
	actor.update(app->time);
}

void AppStateIngame::draw() {
	app->window.beginDraw();

	app->window.draw(text);
	actor.draw(app->window);

	app->window.endDraw();
}

bool AppStateIngame::init() {
	soundbox.setChannelCount(2);
	soundbox.setVolume(settings.soundVolume);

	text.setFont(resmgr.get<sf::Font>("cruft.ttf"));
	text.setString("Press B to play bounce.wav, press K to play kick.wav\nPress A to toggle run left animation\nPress D to toggle run right animation\nPress ESC to return to Main menu.");
	text.setFillColor(sf::Color::White);

	actor.setTexture(resmgr.get<sf::Texture>("sample_texture.png"));
	actor.setAnimationStates(resmgr.get <std::shared_ptr<dgm::AnimationStates>>("sample_config.json"));

	return true;
}
