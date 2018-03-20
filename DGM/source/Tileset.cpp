#include <DGM\dgm.hpp>

void dgm::Tileset::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = texturePtr;

	// draw the vertex array
	target.draw(vertices, states);
}

void dgm::Tileset::ChangeTile_(float x, float y, uint32_t tileIndex, uint32_t tileValue) {
	assert(tileValue < clip.GetFrameCount());
	assert(tileIndex < tileIndices.size());

	tileIndices[tileIndex] = tileValue;

	// Find textureRect for this tile
	const sf::IntRect &frame = clip.GetFrame(tileValue);
	sf::Vector2i voxSize = mesh.GetVoxelSize();

	// Get a pointer to the current tile's quad
	sf::Vertex *quad = &vertices[tileIndex * 4];

	// Define corners
	quad[0].position = sf::Vector2f(x * voxSize.x, y * voxSize.y);
	quad[1].position = sf::Vector2f((x + 1.f) * voxSize.x, y * voxSize.y);
	quad[2].position = sf::Vector2f((x + 1.f) * voxSize.x, (y + 1.f) * voxSize.y);
	quad[3].position = sf::Vector2f(x * voxSize.x, (y + 1.f) * voxSize.y);

	// Define texture coordinates
	quad[0].texCoords = sf::Vector2f(float(frame.left), float(frame.top));
	quad[1].texCoords = sf::Vector2f(float(frame.left + frame.width), float(frame.top));
	quad[2].texCoords = sf::Vector2f(float(frame.left + frame.width), float(frame.top + frame.height));
	quad[3].texCoords = sf::Vector2f(float(frame.left), float(frame.top + frame.height));
}

bool dgm::Tileset::LoadFromParameters(const dgm::Clip & clip, const std::vector<int>& imageData, const std::vector<int>& collisionData, const sf::Vector2i &size) {
	assert(collisionData.size() == imageData.size());
	assert(collisionData.size() == size.x * size.y);

	Tileset::clip = clip;
	mesh.SetVoxelSize(clip.GetFrameSize());
	mesh.SetDataSize(size);

	for (int i = 0; i < size.x * size.y; i++) {
		mesh[i] = collisionData[i];
	}
	tileIndices.resize(imageData.size());

	// Initialize vertex array
	vertices.clear();
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(size.x * size.y * 4);

	// Loop over all tiles
	for (int y = 0; y < mesh.GetDataSize().y; y++) {
		for (int x = 0; x < mesh.GetDataSize().x; x++) {
			ChangeTile(x, y, imageData[y * size.x + x]);
		}
	}

	return true;
}

bool dgm::Tileset::LoadFromFile(const std::string & filename) {
	std::cerr << "Tileset::LoadFromFile(...) - TODO Implement\n";
	return false;
}

bool dgm::Tileset::SaveToFile(const std::string & filename, bool compressed = false) {
	std::cerr << "Tileset::SaveToFile(...) - TODO Implement\n";
	return false;
}

void dgm::Tileset::SetTexture(sf::Texture * texture) {
	assert(texture != NULL);
	texturePtr = texture;
}

dgm::Tileset::Tileset() {
}

dgm::Tileset::~Tileset() {
}
