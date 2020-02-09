/**
 *  \file example10.cpp
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

const unsigned TILE_SIZE = 32;
const float SPEED = 64.f;

class Level {
protected:
	dgm::Mesh mesh;
	dgm::Tileset tileset;

public:
	void draw(dgm::Window &window) {
		window.draw(tileset);
	}

	const dgm::Mesh &getMesh() const {
		return mesh;
	}

	void loadFromFile(const std::string &filename) {
		LevelD lvld;
		lvld.loadFromFile(filename);

		// Extract collision data from lvld
		mesh = dgm::Mesh(lvld);

		// Extract render data from lvld
		dgm::Clip clip({ int(TILE_SIZE), int(TILE_SIZE) }, { 0, 0, 64, 64 });
		tileset.build(
			clip, 
			{ int(TILE_SIZE), int(TILE_SIZE) },
			std::vector<int>(lvld.mesh.tiles.begin(), lvld.mesh.tiles.end()), 
			{ lvld.mesh.width, lvld.mesh.height }
		);
	}

	Level(sf::Texture &texture) {
		tileset.setTexture(texture);
	}
};

class Player {
private:
	dgm::Circle body;
	sf::CircleShape sprite;
	dgm::Controller input;

public:
	enum {
		Up, Left, Right, Down
	};

	void draw(dgm::Window &window) {
		window.draw(sprite);
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
		sprite.move(forward);
	}

	void spawn(const sf::Vector2f &pos) {
		body.setPosition(pos);
		sprite.setPosition(pos);
	}

	Player() {
		sprite.setRadius(10.f);
		sprite.setOrigin(10.f, 10.f);
		sprite.setFillColor(sf::Color::Yellow);

		body.setRadius(10.f);

		input.setBinding(Up, sf::Keyboard::Up);
		input.setBinding(Left, sf::Keyboard::Left);
		input.setBinding(Right, sf::Keyboard::Right);
		input.setBinding(Down, sf::Keyboard::Down);
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
	lvld.mesh.width = 10;
	lvld.mesh.height = 10;
	// Indices of tiles in tileset
	lvld.mesh.tiles = {
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
	lvld.mesh.blocks = {
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

	lvld.saveToFile("../examples/level.lvd");
}

int main() {
	//exportLevel(); // NOTE: Uncomment this only if you do changes to this method

	dgm::Window window({1280, 720}, "Example 10", false);
	dgm::Time time;

	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false);
	resmgr.loadResourceDir<sf::Texture>("../examples");

	Level level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile("../examples/level.lvd");

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