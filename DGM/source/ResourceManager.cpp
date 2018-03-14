#include <DGM/dgm.hpp>
#include <regex>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

void dgm::ResourceManager::ResourceName(const std::string &filename, std::string &name) {
	std::regex exprPrefix(".*\\/.*\\/");
	name = std::regex_replace(filename, exprPrefix, "");
	std::regex exprPrefixAlt(".*\\\\.*\\\\");
	name = std::regex_replace(name, exprPrefixAlt, "");
	std::regex exprSuffix("\\....$");
	name = std::regex_replace(name, exprSuffix, "");
}

bool dgm::ResourceManager::LoadFromDir(const std::string &foldername, dgm::ResourceManager::Type type) {
	fs::path path(foldername);
	if (not fs::is_directory(path)) {
		std::cerr << "ResourceManager::LoadFromDir - " << foldername << " is not a directory!\n";
		return false;
	}

	fs::recursive_directory_iterator dirIter(path);
	for (auto item : dirIter) {
		fs::path itemPath(item);

		switch (type) {
		case Type::Font:
			if (not LoadResource<sf::Font>(itemPath.string())) return false;
			break;
		case Type::Graphic:
			if (not LoadResource<sf::Texture>(itemPath.string())) return false;
			break;
		case Type::Sound:
			if (not LoadResource<sf::SoundBuffer>(itemPath.string())) return false;
			break;
		}
	}

	return true;
}

bool dgm::ResourceManager::Initialize(const dgm::Config &config) {
	if (config.HasSection("ResourceManager")) {
		auto itr = config["ResourceManager"];
		std::string dirs = itr.find("autoloadDirs") == itr.end() ? "" : itr.at("autoloadDirs").AsString();
		std::vector<std::string> dirNames;
		dgm::Strings::Split(';', dirs, dirNames);

		for (auto dirName : dirNames) {
			if (not LoadFromDir(dirName, Type::Graphic)) return false;
		}
	}

	return true;
}

dgm::ResourceManager::ResourceManager() {
	
}

dgm::ResourceManager::~ResourceManager() {
	
}