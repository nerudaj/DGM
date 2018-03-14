#include <DGM\dgm.hpp>

using std::vector;
using std::string;

void dgm::Conversion::StringToColor(const std::string & str, sf::Color & color) {
	// TODO: this
	std::cerr << "dgm::Conversion::StringToColor(...) - Is not implemented yet.\n";
}

const int * dgm::Conversion::StringToIntArray(const char delimiter, const std::string & str, std::size_t & size) {
	vector<string> split;
	dgm::Strings::Split(delimiter, str, split);
	
	size = split.size();
	int *result = new int[size];
	if (result == NULL)
		return NULL;

	for (size_t i = 0; i < size; i++) {
		result[i] = int(strtol(split[i].c_str(), NULL, 10));
	}

	return result;
}

bool dgm::Conversion::StringToVector2i(const char delimiter, const std::string & str, sf::Vector2i & dst) {
	size_t size;
	const int *arr = dgm::Conversion::StringToIntArray(delimiter, str, size);

	if (size == 2) {
		dst.x = arr[0];
		dst.y = arr[1];
	}

	delete arr;
	return (size == 2);
}

bool dgm::Conversion::StringToIntRect(const char delimiter, const std::string & str, sf::IntRect & dst) {
	size_t size;
	const int *arr = dgm::Conversion::StringToIntArray(delimiter, str, size);

	if (size == 4) {
		dst.left = arr[0];
		dst.top = arr[1];
		dst.width = arr[2];
		dst.height = arr[3];
	}

	delete arr;
	return (size == 4);
}

void dgm::Conversion::CircleToIntRect(const dgm::Circle & circ, sf::IntRect & dst) {
	dst.left = int(circ.GetPosition().x - circ.GetRadius());
	dst.top = int(circ.GetPosition().y - circ.GetRadius());
	dst.width = int(circ.GetRadius()) * 2;
	dst.height = int(circ.GetRadius()) * 2;
}

