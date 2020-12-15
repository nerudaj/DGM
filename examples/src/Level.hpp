#pragma once

#include <DGM/dgm.hpp>

const unsigned TILE_SIZE = 32;

class Level {
protected:
	dgm::Mesh mesh;
	dgm::TileMap tilemap;

public:
	void draw(dgm::Window &window) {
		window.draw(tilemap);
	}

	const dgm::Mesh &getMesh() const {
		return mesh;
	}

	void changeTileToVoid(unsigned x, unsigned y) {
		tilemap.changeTile(x, y, 0);
		mesh[y * mesh.getDataSize().x + x] = 0;
	}

	void loadFromFile(const std::string &filename) {
		LevelD lvld;
		lvld.loadFromFile(filename);

		mesh = dgm::Mesh(lvld.mesh);
		tilemap.build(lvld.mesh);
	}

	Level(sf::Texture& texture) {
		dgm::Clip clip(
			{ TILE_SIZE, TILE_SIZE },
			{ 0, 0, int(texture.getSize().x), int(texture.getSize().y) }
		);

		tilemap.init(texture, clip);
	}
};