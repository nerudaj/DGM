#pragma once

#include <DGM/dgm.hpp>

const unsigned TILE_SIZE = 32;

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
		dgm::Clip clip({ TILE_SIZE, TILE_SIZE }, { 0, 0, 64, 64 });

		LevelD lvld;
		lvld.loadFromFile(filename);

		mesh = dgm::Mesh(lvld);
		tileset.rebuild(clip, lvld);
	}

	Level(sf::Texture &texture) {
		tileset.setTexture(texture);
	}
};