#include <DGM\dgm.hpp>
#include <fstream>
#include <regex>

bool dgm::Config::IsSectionHeader(const std::string &line) {
	return std::regex_match (line, std::regex("\\[.+\\]"));
}

bool dgm::Config::IsKeyValuePair(const std::string &line) {
	return std::regex_match (line, std::regex(".+=.+"));
}

void dgm::Config::GetSectionIdentifier(const std::string &line, std::string &dst) {
	dst = line.substr(1, line.length() - 2);
}

void dgm::Config::GetKeyValue(const std::string &line, std::string &dstKey, std::string &dstValue) {
	auto delimiterPosition = line.find('=');
	dstKey = line.substr(0, delimiterPosition);
	dstValue = line.substr(delimiterPosition + 1, line.length() - delimiterPosition - 1);
}

bool dgm::Config::Load(const std::string &filename) {
	config.clear();
	std::ifstream load(filename);
	
	if (!load.good()) return false;
	
	std::string line;
	std::string section = "root";
	std::string key, value;
	while (std::getline(load, line)) {
		if (IsSectionHeader(line)) {
			GetSectionIdentifier(line, section);
		}
		else if (IsKeyValuePair(line)) {
			GetKeyValue(line, key, value);
			config[section][key] = value;
		}
	}
	
	load.close();
	load.clear();
	
	return true;
}

bool dgm::Config::Save(const std::string &filename) {
	std::ofstream save (filename);
	
	if (!save.good()) return false;
	
	for (auto section : config) {
		save << "[" << section.first << "]\n";
		
		for (auto item : section.second) {
			save << item.first << "=" << item.second.AsString() << "\n";
		}
		
		save << "\n";
	}
	
	save.close();
	save.clear();
	
	return true;
}