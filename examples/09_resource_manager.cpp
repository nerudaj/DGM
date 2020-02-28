/**
 *  \file 09_resource_manager.cpp
 *
 *  \details This example showcases loading resources via ResourceManager
 *  and binding those resources to objects in dgm::Buffer, effectively
 *  sharing those resources.
 *
 *  Example will spawn many running soldiers on the screen. All instances
 *  share Texture and AnimationStates, but have their own Animation object.
 */

#define NO_NETWORK

#include <DGM\dgm.hpp>

const unsigned SOLDIER_COUNT = 10;

class Soldier {
private:
	dgm::Animation animation;
	sf::Sprite sprite;

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}
	
	void update(const dgm::Time &time) {
		animation.update(time);
	}
	
	void spawn() {
		sf::Vector2f pos(
			float(rand() % 1280),
			float(rand() % 720)
		);

		sprite.setPosition(pos);

		const std::string STATES[] = {
			"run-left",
			"run-right"
		};
		animation.setState(STATES[rand() % 2]);
	}
	
	void init(const sf::Texture &texture, const std::shared_ptr<dgm::AnimationStates> &animData) {
		animation = dgm::Animation(animData);
		sprite.setTexture(texture);
		animation.bindSprite(&sprite);
		animation.setSpeed(10);
	}
};

int main(int argc, char *argv[]) {
	srand(unsigned(time(NULL)));

	std::string resourceDir = "../examples/resources";
	if (argc == 2) {
		resourceDir = argv[1];
	}

	dgm::Window window({1280, 720}, "Example 09", false);
	dgm::Time time;
	
	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false); // loadResourceDir will only load compatible files and ignore everything else
	resmgr.loadResourceDir<sf::Texture>(resourceDir); // will load soldier.png, image.png and tileset.png
	resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(resourceDir); // will load soldier.json
	
	dgm::Buffer<Soldier> soldiers(SOLDIER_COUNT);
	for (unsigned i = 0; i < soldiers.capacity(); i++) {
		// Each soldier will have their own Animation, but will share Texture and AnimationData
		soldiers[i].init(resmgr.get<sf::Texture>("soldier.png"), resmgr.get<std::shared_ptr<dgm::AnimationStates>>("soldier.json"));
		soldiers.expand();
		soldiers[i].spawn();
	}
	
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		/* LOGIC */
		time.reset();
		
		for (auto &soldier : soldiers) {
			soldier.update(time);
		}
		
		/* RENDER */
		window.beginDraw();
		
		for (auto &soldier : soldiers) {
			soldier.draw(window);
		}
		
		window.endDraw();
	}
	
	return 0;
}