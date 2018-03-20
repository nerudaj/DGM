#pragma once

#include <DGM\dgm.hpp>
#include <cassert>

namespace dgm {
	class Tileset : public sf::Drawable, public sf::Transformable {
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	protected:
		sf::Texture *texturePtr;
		sf::VertexArray vertices;
		std::vector<int> tileIndices;
		dgm::Mesh mesh;
		dgm::Clip clip;

		void changeTile_(float x, float y, uint32_t tileIndex, uint32_t tileValue);

	public:
		/**
		 *  \brief Change appearance of a tile
		 *  
		 *  \param [in] tileX Tile X position
		 *  \param [in] tileY Tile Y position
		 *  \param [in] tileValue New frame index for a tile
		 *  
		 *  \details Will change image displayed on a tile[x, y] to image with index
		 *  value of tileValue. tileX and tileY must lie within the tileset and tileValue
		 *  must index valid clip frame. New value is stored and will be exported by
		 *  SaveToFile() method.
		 *  
		 *  \pre loadFromFile or LoadFromParameters was successfully called.
		 */
		void changeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue) {
			assert(tileX < uint32_t(mesh.getDataSize().x) && tileY < uint32_t(mesh.getDataSize().y));
			changeTile_(float(tileX), float(tileY), tileY * mesh.getDataSize().x + tileX, tileValue);
		}

		/**
		 *  \brief Change appearance of a tile
		 *  
		 *  \param [in] tilePosition Tile XY position
		 *  \param [in] tileValue New frame index for a tile
		 *  
		 *  \details Will change image displayed on a tile[x, y] to image with index
		 *  value of tileValue. tileX and tileY must lie within the tileset and tileValue
		 *  must index valid clip frame. New value is stored and will be exported by
		 *  SaveToFile() method.
		 *  
		 *  \pre loadFromFile or LoadFromParameters was successfully called.
		 */
		void changeTile(const sf::Vector2u &tilePosition, uint32_t tileValue) {
			assert(tilePosition.x < uint32_t(mesh.getDataSize().x) && tilePosition.y < uint32_t(mesh.getDataSize().y));
			changeTile_(float(tilePosition.x), float(tilePosition.y), tilePosition.y * mesh.getDataSize().x + tilePosition.x, tileValue);
		}

		/**
		 *  \brief initialize object from parameters
		 *  
		 *  \param [in] clip Initialized clip object
		 *  \param [in] imageData Array of tile frame indices
		 *  \param [in] collisionData Array of collision (mesh) data
		 *  \param [in] size Size of the map [Width, Height]
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details Every object will be copied, so parameters can seize to exist
		 *  after this call.
		 */
		bool loadFromParameters(const dgm::Clip &clip, const std::vector<int> &imageData, const std::vector<int> &collisionData, const sf::Vector2i &size);

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

		/**
		 *  \brief Set tileset texture
		 *  
		 *  \param [in] texture Pointer to valid texture object
		 *  
		 *  \details You should call this prior to any draw call on this object.
		 *  Pointers retrieved from ResourceManager can be directly passed to
		 *  this function.
		 */
		void setTexture(sf::Texture *texture);

		const dgm::Mesh &getMesh() const { return mesh; }

		dgm::Mesh &getMesh() { return mesh; }

		Tileset();
		~Tileset();
	};
}