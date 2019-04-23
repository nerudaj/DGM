#define NO_AUDIO
#define NO_NETWORK

#include <DGM/dgm.hpp>

class Player {
private:
	sf::Sprite sprite;
	dgm::Animation animation;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time) {
		animation.update(time);
	}

	Player(const std::string &animfile, const sf::Vector &position, const sf::Texture &texture) : animation(animfile) {
		sprite.setPosition(position);
		sprite.setTexture(texture);
		
		animation.bindSprite(&sprite);
		animation.setState("example", true);
	}
};

class NPC {
private:
	sf::Sprite sprite;
	dgm::Animation animation;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time) {
		animation.update(time);
	}

	void init(const dgm::AnimationStates &states, const sf::Vector &position, const sf::Texture &texture) {
		animation = dgm::Animation(states);
		
		sprite.setPosition(position);
		sprite.setTexture(texture);
		
		animation.bindSprite(&sprite);
		animation.setState("example", true);
	}
};

int main() {
	dgm::Window window({1280, 720, "Sandbox", false);
	dgm::Time time;
	
	sf::Texture textureA, textureB;
	if (not textureA.loadFromFile("TODO://path")) {
		return 1;
	}
	if (not textureB.loadFromFile("TODO://path")) {
		return 2;
	}
	
	Player player("animPlayer.json", {10.f, 10.f}, textureA);
	
	dgm::AnimationStates sharedStates = dgm::Animation::loadStatesFromFile("animNPC.json");
	dgm::Buffer<NPC> npcs(3);
	for (unsigned i = 0; i < npcs.capacity(); i++) {
		npcs.add();
		npcs[i].init(sharedStates, {i * 20.f + 30.f, 10.f}, textureB);
	}

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
		for (auto npc : npcs) {
			npc.update(time);
		}
		
		time.reset();

		// Rendering
		window.beginDraw(sf::Color::Black);

		player.draw(window);
		for (auto npc : npcs) {
			npc.draw(window);
		}

		window.endDraw();
	}
	
	return 0;
}