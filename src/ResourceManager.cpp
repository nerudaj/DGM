#include <DGM/dgm.hpp>
#include <stdexcept>

using dgm::ResourceManager;

void ResourceManager::loadResourceFromFile(const std::string &filename, sf::Texture &texture) {
	if (not texture.loadFromFile(filename)) {
		throw std::runtime_error("Cannot load texture");
	}
}

void ResourceManager::loadResourceFromFile(const std::string &filename, sf::Font &font) {
	if (not font.loadFromFile(filename)) {
		throw std::runtime_error("Cannot load font");
	}
}

void ResourceManager::loadResourceFromFile(const std::string &filename, sf::SoundBuffer &sound) {
	if (not sound.loadFromFile(filename)) {
		throw std::runtime_error("Cannot load sound buffer");
	}
}

void ResourceManager::loadResourceFromFile(const std::string &filename, std::shared_ptr<dgm::AnimationStates> &states) {
	states = dgm::Animation::loadStatesFromFile(filename);
}

std::string dgm::ResourceManager::getResourceName(const std::string & filename) {
	auto itr = filename.find_last_of('/');
	return filename.substr(++itr);
}

ResourceManager::ResourceManager() {
}

ResourceManager::ResourceManager(ResourceManager &&other) {
	database = other.database;
	other.database.clear();
}

ResourceManager::~ResourceManager() {
	for (auto keyval : database) {
		free(keyval.second);
	}
}