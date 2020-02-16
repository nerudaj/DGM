#pragma once

#include <DGM\dgm.hpp>
#include "Soundbox.hpp"
#include "Globals.hpp"

class AppStateIngame : public dgm::AppState {
private:
	dgm::ResourceManager &resmgr;
	Settings settings;

	Soundbox soundbox = Soundbox(resmgr);
	sf::Text text;

	class Actor {
	private:
		sf::Sprite sprite;
		dgm::Animation animation;

	public:
		void draw(dgm::Window& window) {
			window.draw(sprite);
		}

		void update(const dgm::Time& time) {
			animation.update(time);
		}

		void setAnimationState(const std::string& state) {
			animation.setState(state);
		}

		void setTexture(sf::Texture& texture) {
			sprite.setTexture(texture);
		}

		void setAnimationStates(std::shared_ptr<dgm::AnimationStates>& states) {
			animation = dgm::Animation(states, 15);
			animation.bindSprite(&sprite);
			animation.setState("idle-left");
		}

		Actor() {
			sprite.setPosition(100.f, 300.f);
		}
	} actor;

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateIngame(dgm::ResourceManager &resmgr, Settings &settings) : resmgr(resmgr), settings(settings) {}
};