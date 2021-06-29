/**
 *  \file 10_tileset.cpp
 *
 *  \details This example showcases dgm::Tileset as a way of drawing tileset based levels.
 *  Since dshlibs are included into DGM installation, it also showcases how to use LevelD
 *  class (and file format) to export and import levels. LevelD can do much more in terms
 *  of levels, it can store player, item and NPC informations, feel free to explore the
 *  data structure.
 *  You can move the yellow dot with the directional keys and test collisions with the 
 *  level for yourself.
 */

#define NO_NETWORK

#include <DGM/dgm.hpp>
#include "Level.hpp"
#include "RootDir.hpp"

const float SPEED = 64.f;

class Player {
private:
	dgm::Circle body;
	dgm::Controller input;

public:
	enum {
		Up, Left, Right, Down
	};

	void draw(dgm::Window &window) {
		// Unless you have textured sprite, you
		// can use debugRender to quicky get started
		body.debugRender(window);
	}

	void update(const dgm::Time &time, const dgm::Mesh &level) {
		sf::Vector2f forward = { 0.f, 0.f };

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

		dgm::Collision::advanced(level, body, forward);

		body.move(forward);
	}

	void spawn(const sf::Vector2f &pos) {
		body.setPosition(pos);
	}

	Player() {
		body.setRadius(10.f);

		input.bindKeyboardKey(Up, sf::Keyboard::Up);
		input.bindKeyboardKey(Left, sf::Keyboard::Left);
		input.bindKeyboardKey(Right, sf::Keyboard::Right);
		input.bindKeyboardKey(Down, sf::Keyboard::Down);
	}
};

void exportLevel() {
	LevelD lvld;

	// Once a certain attribute of lvld is initialized, it will become the part of export
	lvld.metadata.author = "doomista";
	lvld.metadata.description = "Basic level for example10";
	lvld.metadata.timestamp = time(NULL);
	lvld.metadata.id = "EXAMPLE_10";
	lvld.metadata.name = "Example Level";

	lvld.mesh.tileWidth = 32;
	lvld.mesh.tileHeight = 32;
	lvld.mesh.layerWidth = 10;
	lvld.mesh.layerHeight = 10;
	// Indices of tiles in tileset
	LevelD::TileLayer layer;
	layer.tiles = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 2, 2, 0, 0, 0, 0, 1,
		1, 0, 2, 3, 3, 2, 0, 0, 0, 1,
		1, 0, 0, 2, 2, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 2, 1,
		1, 0, 0, 0, 0, 0, 0, 2, 2, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	// Collision information
	layer.blocks = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	lvld.mesh.layers.push_back(layer);

	lvld.saveToFile(rootDir + "/level.lvd");
}

int main() {
	//exportLevel(); // NOTE: Uncomment this only if you do changes to this method

	dgm::Window window({1280, 720}, "Example 10", false);
	dgm::Time time;

	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false);
	resmgr.loadResourceDir<sf::Texture>(rootDir);

	Level level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile(rootDir + "/level.lvd");

	Player player;
	player.spawn({ 64.f, 64.f });

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		/* LOGIC */
		time.reset();

		player.update(time, level.getMesh());
		
		/* DRAW */
		window.beginDraw();
		
		level.draw(window);
		player.draw(window);

		window.endDraw();
	}

	return 0;
}