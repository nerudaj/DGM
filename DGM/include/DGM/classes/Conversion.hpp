#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Conversion {
	public:
		/**
		 * \brief Takes a string with hex representation of a color (#XXXXXX)
		 * and converts it to output sf::Color
		 */
		static void stringToColor(const std::string &str, sf::Color &color);

		/**
		 * \brief Takes a string of integer numbers separated with delimiter
		 * splits it to integer array and returns pointer to newly allocated
		 * array. Size of array is stored in size.
		 *
		 * \note Call delete on the array when you are done with it
		 *
		 * \returns NULL on failure, valid pointer to int array otherwise
		 */
		static const int *stringToIntArray(const char delimiter, const std::string &str, std::size_t &size);

		/**
		 * \brief Takes a string of integer numbers separated with delimiter
		 * and splits it to sf::Vector2i
		 *
		 * \details On failure, dst is not modified
		 *
		 * \returns TRUE if str contains only precisely int number. FALSE otherwise
		 */
		static bool stringToVector2i(const char delimiter, const std::string &str, sf::Vector2i &dst);

		/**
		* \brief Takes a string of integer numbers separated with delimiter
		* and splits it to sf::IntRect
		*
		* \details On failure, dst is not modified
		*
		* \returns TRUE if str contains precisely four int number. FALSE otherwise
		*/
		static bool stringToIntRect(const char delimiter, const std::string &str, sf::IntRect &dst);

		/**
		 * \brief Creates a bounding box of a circle, storing result to dst
		 */
		static void circleToIntRect(const dgm::Circle &circ, sf::IntRect &dst);
	};
}