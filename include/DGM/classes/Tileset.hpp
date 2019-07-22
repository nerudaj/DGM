#pragma once

#include <DGM\dgm.hpp>
#include <cassert>

namespace dgm {
	/**
	 *  \brief Class for rendering tileset based maps
	 */
	class Tileset : public sf::Drawable, public sf::Transformable {
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	protected:
		sf::Texture *texturePtr;
		sf::VertexArray vertices;
		sf::Vector2f tileSize;
		sf::Vector2i dataSize;
		dgm::Clip clip;

		void changeTile(float x, float y, uint32_t tileIndex, uint32_t tileValue);

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
			assert(tileX < uint32_t(dataSize.x) && tileY < uint32_t(dataSize.y));
			changeTile(float(tileX), float(tileY), tileY * dataSize.x + tileX, tileValue);
		}

		/**
		 *  \brief Build the internal vertex array
		 *  
		 *  \param [in] clip Initialized clip object
		 *  \param [in] tileSize Dimensions of a tile (can differ from clip frame size)
		 *  \param [in] imageData Array of tile frame indices
		 *  \param [in] collisionData Array of collision (mesh) data
		 *  \param [in] dataSize Size of the map [Width, Height]
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details You can call this function repeatedly without need to deinitialize this
		 *  object. If you only need to change appearance of a couple of tiles, use changeTile()
		 *  instead.
		 *
		 *  \see changeTile
		 */
		void build(const dgm::Clip &clip, const sf::Vector2i tileSize, const std::vector<int> &imageData, const sf::Vector2i &dataSize);

		/**
		 *  \brief Set tileset texture
		 *  
		 *  \param [in] texture Pointer to valid texture object
		 *  
		 *  \details You should call this prior to any draw call on this object.
		 *  Pointers retrieved from ResourceManager can be directly passed to
		 *  this function.
		 */
		void setTexture(sf::Texture &texture);

		const dgm::Clip &getClip() const { return clip; }

		Tileset();
		~Tileset();
	};
}