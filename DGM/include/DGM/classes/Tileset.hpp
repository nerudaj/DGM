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

		void ChangeTile_(float x, float y, uint32_t tileIndex, uint32_t tileValue);

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
		 *  \pre LoadFromFile or LoadFromParameters was successfully called.
		 */
		void ChangeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue) {
			assert(tileX < mesh.GetDataSize().x && tileY < mesh.GetDataSize().y);
			ChangeTile_(float(tileX), float(tileY), tileY * mesh.GetDataSize().x + tileX, tileValue);
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
		 *  \pre LoadFromFile or LoadFromParameters was successfully called.
		 */
		void ChangeTile(const sf::Vector2u &tilePosition, uint32_t tileValue) {
			assert(tilePosition.x < mesh.GetDataSize().x && tilePosition.y < mesh.GetDataSize().y);
			ChangeTile_(float(tilePosition.x), float(tilePosition.y), tilePosition.y * mesh.GetDataSize().x + tilePosition.x, tileValue);
		}

		/**
		 *  \brief Initialize object from parameters
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
		bool LoadFromParameters(const dgm::Clip &clip, const std::vector<int> &imageData, const std::vector<int> &collisionData, const sf::Vector2i &size);

		/**
		 *  \brief Load tileset data from the file 
		 *  
		 *  \param [in] filename Path to file with data
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details This will load clip, mesh and size settings as well as the
		 *  data values for tile indices and collision data.
		 */
		bool LoadFromFile(const std::string &filename);

		/**
		 *  \brief Export data to file
		 *  
		 *  \param [in] filename Path to file
		 *  \param [in] compressed Whether data will be exported in compressed format
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details This will export all internal data to a file. Data exported
		 *  this way can be loaded with LoadFromFile
		 */
		bool SaveToFile(const std::string &filename, bool compressed = false);

		/**
		 *  \brief Set tileset texture
		 *  
		 *  \param [in] texture Pointer to valid texture object
		 *  
		 *  \details You should call this prior to any draw call on this object.
		 *  Pointers retrieved from ResourceManager can be directly passed to
		 *  this function.
		 */
		void SetTexture(sf::Texture *texture);

		const dgm::Mesh &GetMesh() const { return mesh; }

		dgm::Mesh &GetMesh() { return mesh; }

		Tileset();
		~Tileset();
	};
}