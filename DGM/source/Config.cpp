#include <DGM\dgm.hpp>
#include <fstream>
#include <regex>

bool dgm::Config::isSectionHeader(const std::string &line) {
	return std::regex_match (line, std::regex("\\[.+\\]"));
}

bool dgm::Config::isKeyValuePair(const std::string &line) {
	return std::regex_match (line, std::regex(".+=.+"));
}

void dgm::Config::getSectionIdentifier(const std::string &line, std::string &dst) {
	dst = line.substr(1, line.length() - 2);
}

void dgm::Config::getKeyValue(const std::string &line, std::string &dstKey, std::string &dstValue) {
	auto delimiterPosition = line.find('=');
	dstKey = line.substr(0, delimiterPosition);
	dstValue = line.substr(delimiterPosition + 1, line.length() - delimiterPosition - 1);
}

bool dgm::Config::loadFromFile(const std::string &filename) {
	config.clear();
	std::ifstream load(filename);
	
	if (!load.good()) {
		std::cerr << "Config::loadFromFile(...) - Could not open file " << filename << ".\n";
		return false;
	}
	
	std::string line;
	std::string section = "root";
	std::string key, value;
	while (std::getline(load, line)) {
		if (isSectionHeader(line)) {
			getSectionIdentifier(line, section);
		}
		else if (isKeyValuePair(line)) {
			getKeyValue(line, key, value);
			config[section][key] = value;
		}
	}
	
	load.close();
	load.clear();
	
	return true;
}

bool dgm::Config::saveToFile(const std::string &filename) {
	std::ofstream save (filename);
	
	if (!save.good()) {
		std::cerr << "Config::saveToFile(...) - Could not open file " << filename << ".\n";
		return false;
	}
	
	for (auto section : config) {
		save << "[" << section.first << "]\n";
		
		for (auto item : section.second) {
			save << item.first << "=" << item.second.asString() << "\n";
		}
		
		save << "\n";
	}
	
	save.close();
	save.clear();
	
	return true;
}