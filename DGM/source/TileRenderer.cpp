#include <DGM\dgm.hpp>

void dgm::TileMapRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = texturePtr;

	// draw the vertex array
	target.draw(vertices, states);
}

void dgm::TileMapRenderer::rebuild(const dgm::Clip & clip, const dgm::Mesh &mesh, const int const * dataPtr) {
	// Initialize vertex array
	vertices.clear();
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(mesh.getDataSize().x * mesh.getDataSize().y * 4);

	// Loop over all tiles
	for (int y = 0; y < mesh.getDataSize().y; y++) {
		for (int x = 0; x < mesh.getDataSize().x; x++) {
			// Get tile index
			int ti = y * mesh.getDataSize().x + x;

			// Find textureRect for this tile
			const sf::IntRect &frame = clip.getFrame(dataPtr[ti]);

			// Get a pointer to the current tile's quad
			sf::Vertex *quad = &vertices[ti * 4];

			// Define corners
			quad[0].position = sf::Vector2f(float(x) * mesh.getVoxelSize().x, float(y) * mesh.getVoxelSize().y);
			quad[1].position = sf::Vector2f(float(x + 1) * mesh.getVoxelSize().x, float(y) * mesh.getVoxelSize().y);
			quad[2].position = sf::Vector2f(float(x + 1) * mesh.getVoxelSize().x, float(y + 1) * mesh.getVoxelSize().y);
			quad[3].position = sf::Vector2f(float(x) * mesh.getVoxelSize().x, float(y + 1) * mesh.getVoxelSize().y);

			// Define texture coordinates
			quad[0].texCoords = sf::Vector2f(float(frame.left), float(frame.top));
			quad[1].texCoords = sf::Vector2f(float(frame.left + frame.width), float(frame.top));
			quad[2].texCoords = sf::Vector2f(float(frame.left + frame.width), float(frame.top + frame.height));
			quad[3].texCoords = sf::Vector2f(float(frame.left), float(frame.top + frame.height));
		}
	}
}

void dgm::TileMapRenderer::setTexture(sf::Texture * texture) {
	texturePtr = texture;
}
