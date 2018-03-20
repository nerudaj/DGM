#pragma once

#include <string>
#include <iostream>

namespace dgm {
	class ConfigItem {
	protected:
		std::string value;

	public:
		long asInt() const;
		float asFloat() const;
		const std::string &asString() const;
		bool asBool() const;

		ConfigItem &operator=(const char *value);
		ConfigItem &operator=(const std::string &value);
		ConfigItem &operator=(const bool value);
		ConfigItem &operator=(const int value);
		ConfigItem &operator=(const long value);
		ConfigItem &operator=(const float value);

		ConfigItem();
		~ConfigItem();
	};
}