#include <DGM\dgm.hpp>

void dgm::Level::changeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue) {
	assert(tileX < uint32_t(mesh.getDataSize().x) && tileY < uint32_t(mesh.getDataSize().y));
	renderer.changeTile(tileX, tileY, tileValue);
	imageData[tileY * mesh.getDataSize().x + tileX] = tileValue;
}

void dgm::Level::build(const dgm::Clip & clip, const sf::Vector2i tileSize, const std::vector<int>& imageData, const std::vector<int> &collisionData, const sf::Vector2i & dataSize) {
	assert(imageData.size() == collisionData.size());

	mesh.setVoxelSize(tileSize);
	mesh.setDataSize(dataSize);
	for (std::size_t i = 0; i < collisionData.size(); i++) {
		mesh[i] = collisionData[i];
	}

	Level::imageData = imageData;

	renderer.build(clip, tileSize, imageData, dataSize);
}

bool dgm::Level::loadFromFile(const std::string & filename) {
	std::cerr << "TilesetRenderer::loadFromFile(...) - TODO Implement\n";
	return false;
}

bool dgm::Level::saveToFile(const std::string & filename, bool compressed) {
	std::cerr << "TilesetRenderer::saveToFile(...) - TODO Implement\n";
	return false;
}

dgm::Level::Level()
{
}

dgm::Level::~Level()
{
}
