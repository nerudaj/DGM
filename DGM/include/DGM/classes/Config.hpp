#pragma once

#include "ConfigItem.hpp"
#include <map>

namespace dgm {
	class ConfigSection : public std::map<std::string, ConfigItem> {
	public:
		bool HasKey(const std::string &key) { return find(key) != end(); }
	};

	class Config {
	protected:
		std::map<std::string, ConfigSection> config;

		static bool IsSectionHeader(const std::string &line);
		static bool IsKeyValuePair(const std::string &line);
		static void GetSectionIdentifier(const std::string &line, std::string &dst);
		static void GetKeyValue(const std::string &line, std::string &dstKey, std::string &dstValue);

	public:
		const ConfigSection &operator[](const std::string &section) const {
			return config.at(section);
		}

		ConfigSection &operator[](const std::string &section) {
			auto &tmp = config[section];
			return tmp;
		}

		bool HasSection(const std::string &section) const { return (config.find(section) != config.end()); }

		bool Load(const std::string &filename);
		bool Save(const std::string &filename);
	};

}
