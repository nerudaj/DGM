#include <DGM\dgm.hpp>
#include <ciso646>
#include <cstdlib>

long dgm::ConfigItem::AsInt() const {
	return strtol(value.c_str(), NULL, 10);
}

float dgm::ConfigItem::AsFloat() const {
	return strtof(value.c_str(), NULL);
}

const std::string &dgm::ConfigItem::AsString() const {
	return value;
}

bool dgm::ConfigItem::AsBool() const {
	if (value == "false" or value == "0") return false;
	
	return true;
}

dgm::ConfigItem &dgm::ConfigItem::operator=(const char *value) {
	ConfigItem::value = value;
	return (*this);
}
	
dgm::ConfigItem &dgm::ConfigItem::operator=(const std::string &value) {
	ConfigItem::value = value;
	return (*this);
}

dgm::ConfigItem &dgm::ConfigItem::operator=(const bool value) {
	if (value) {
		ConfigItem::value = "1";
	}
	else {
		ConfigItem::value = "0";
	}
	return (*this);
}

dgm::ConfigItem &dgm::ConfigItem::operator=(const int value) {
	ConfigItem::value = std::to_string(value);
	return *this;
}

dgm::ConfigItem &dgm::ConfigItem::operator=(const long value) {
	ConfigItem::value = std::to_string(value);
	return *this;
}

dgm::ConfigItem &dgm::ConfigItem::operator=(const float value) {
	ConfigItem::value = std::to_string(value);
	return *this;
}

dgm::ConfigItem::ConfigItem() {
	
}

dgm::ConfigItem::~ConfigItem() {
	
}
