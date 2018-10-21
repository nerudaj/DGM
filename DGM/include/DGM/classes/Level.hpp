#pragma once

#include <DGM\dgm.hpp>
#include <fstream>

namespace dgm {
	class Level {
	protected:
		dgm::TilesetRenderer renderer;
		dgm::Mesh mesh;
		std::vector<int> imageData;

		sf::IntRect findClipBoundaries();
		sf::Vector2i findClipFrameOffset(const sf::IntRect &bounds);

		void loadCompressed(std::ifstream &load);
		void loadUncompressed(std::ifstream &load);

		void saveCompressed(std::ofstream &save);
		void saveUncompressed(std::ofstream &save);

	public:
		void draw(dgm::Window &window) { window.draw(renderer); }

		void changeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue);

		void build(const dgm::Clip &clip, const sf::Vector2i tileSize, const std::vector<int> &imageData, const std::vector<int> &collisionData, const sf::Vector2i &dataSize);

		/**
		*  \brief Load tileset data from the file
		*
		*  \param [in] filename Path to file with data
		*  \return TRUE on success, FALSE otherwise
		*
		*  \details This will load clip, mesh and size settings as well as the
		*  data values for tile indices and collision data.
		*/
		bool loadFromFile(const std::string &filename);

		/**
		*  \brief Export data to file
		*
		*  \param [in] filename Path to file
		*  \param [in] compressed Whether data will be exported in compressed format
		*  \return TRUE on success, FALSE otherwise
		*
		*  \details This will export all internal data to a file. Data exported
		*  this way can be loaded with loadFromFile
		*/
		bool saveToFile(const std::string &filename, bool compressed = false);

		dgm::TilesetRenderer &getRenderer() { return renderer; }
		const dgm::TilesetRenderer &getRenderer() const { return renderer; }
		dgm::Mesh &getMesh() { return mesh; }
		const dgm::Mesh &getMesh() const { return mesh; }

		Level();
		~Level();
	};
}