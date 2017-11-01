#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	/**
	 *  \brief Simple class for rendering tile based maps. It uses vertex array
	 *  so even really huge maps will be drawn very quickly.
	 *
	 *  \details Use rebuild() to recompute internal vertex array and use setTexture
	 *  to skin the map. Then use sf::RenderWindow::draw() on the object. Since this
	 *  inherits from Drawable and Transformable, this object moreless act like
	 *  sprite does.
	 */
	class TileMapRenderer : public sf::Drawable, public sf::Transformable {
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	protected:
		sf::Texture *texturePtr;
		sf::VertexArray vertices;

	public:
		/**
		 *  \brief Rebuilds internal vertexarray representation of the map
		 *
		 *  \param [in] clip Clip data for computing texture frames
		 *  \param [in] mesh Mesh object determining map resolution and tile resolution
		 *  \param [in] dataPtr Pointer to data array containing indices to clip. Each index is a single tile
		 *
		 *  \details VertexArray will be cleared and will be completely recomputed
		 */
		void rebuild(const dgm::Clip &clip, const dgm::Mesh &mesh, const int * const dataPtr);

		/**
		 *  \brief Sets the pointer to texture that should be used for rendering
		 */
		void setTexture(sf::Texture *texture);

		TileMapRenderer() {}
		~TileMapRenderer() {}
	};
};