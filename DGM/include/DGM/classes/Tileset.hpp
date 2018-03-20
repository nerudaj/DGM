#pragma once

#include <DGM\dgm.hpp>

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

	public:
		void ChangeTile(uint32_t x, uint32_t y, uint32_t tile);

		bool LoadFromParameters(const dgm::Clip &clip, const std::vector<int> &imageData, const std::vector<int> &collisionData, const sf::Vector2i &size);

		bool LoadFromFile(const std::string &filename);

		void SetTexture(sf::Texture *texture);

		const dgm::Mesh &GetMesh() const { return mesh; }

		dgm::Mesh &GetMesh() { return mesh; }

		Tileset();
		~Tileset();
	};
}