#include <DGM\dgm.hpp>

using std::vector;
using std::string;

void dgm::Conversion::stringToColor(const std::string & str, sf::Color & color) {
	// TODO: this
	std::cerr << "dgm::Conversion::stringToColor(...) - Is not implemented yet.\n";
}

const int * dgm::Conversion::stringToIntArray(const char delimiter, const std::string & str, std::size_t & size) {
	vector<string> split;
	dgm::Strings::split(delimiter, str, split);
	
	size = split.size();
	int *result = new int[size];
	if (result == NULL)
		return NULL;

	for (size_t i = 0; i < size; i++) {
		result[i] = int(strtol(split[i].c_str(), NULL, 10));
	}

	return result;
}

bool dgm::Conversion::stringToVector2i(const char delimiter, const std::string & str, sf::Vector2i & dst) {
	size_t size;
	const int *arr = dgm::Conversion::stringToIntArray(delimiter, str, size);

	if (size == 2) {
		dst.x = arr[0];
		dst.y = arr[1];
	}

	delete arr;
	return (size == 2);
}

bool dgm::Conversion::stringToIntRect(const char delimiter, const std::string & str, sf::IntRect & dst) {
	size_t size;
	const int *arr = dgm::Conversion::stringToIntArray(delimiter, str, size);

	if (size == 4) {
		dst.left = arr[0];
		dst.top = arr[1];
		dst.width = arr[2];
		dst.height = arr[3];
	}

	delete arr;
	return (size == 4);
}

void dgm::Conversion::circleToIntRect(const dgm::Circle & circ, sf::IntRect & dst) {
	dst.left = int(circ.getPosition().x - circ.getRadius());
	dst.top = int(circ.getPosition().y - circ.getRadius());
	dst.width = int(circ.getRadius()) * 2;
	dst.height = int(circ.getRadius()) * 2;
}

