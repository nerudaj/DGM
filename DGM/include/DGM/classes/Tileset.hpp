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
		void ChangeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue) {
			assert(tileX < mesh.GetDataSize().x && tileY < mesh.GetDataSize().y);
			ChangeTile_(float(tileX), float(tileY), tileY * mesh.GetDataSize().x + tileX, tileValue);
		}

		void ChangeTile(const sf::Vector2u &tilePosition, uint32_t tileValue) {
			assert(tilePosition.x < mesh.GetDataSize().x && tilePosition.y < mesh.GetDataSize().y);
			ChangeTile_(float(tilePosition.x), float(tilePosition.y), tilePosition.y * mesh.GetDataSize().x + tilePosition.x, tileValue);
		}
		bool LoadFromParameters(const dgm::Clip &clip, const std::vector<int> &imageData, const std::vector<int> &collisionData, const sf::Vector2i &size);

		bool LoadFromFile(const std::string &filename);

		bool SaveToFile(const std::string &filename);

		void SetTexture(sf::Texture *texture);

		const dgm::Mesh &GetMesh() const { return mesh; }

		dgm::Mesh &GetMesh() { return mesh; }

		Tileset();
		~Tileset();
	};
}