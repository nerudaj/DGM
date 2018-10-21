#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Conversion {
	public:
		/**
		 * \brief Takes a string with hex representation of a color (#XXXXXX)
		 * and converts it to output sf::Color
		 */
		static sf::Color stringToColor(const std::string &str);

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

		/**
		 *  \brief Convert polar coordinates to cartesian
		 *
		 *  \param [in] angle 0° is at [0, 1], counting clockwise
		 *  \param [in] size Length of the vector
		 */
		static sf::Vector2f polarToCartesian(const float angle, const float size) {
			const float PIOVER180 = 0.01745329252f;
			return sf::Vector2f(sin(angle * PIOVER180) * size, -cos(angle * PIOVER180) * size);
		}

		/**
		 *  \brief Convert polar coordinates to cartesian
		 *
		 *  \param [in] angleSize X coord is angle, Y coord is size
		 */
		static sf::Vector2f polarToCartesian(const sf::Vector2f &angleSize) {
			polarToCartesian(angleSize.x, angleSize.y);
		}

		static sf::Vector2f cartesianToPolar(const float x, const float y);

		static sf::Vector2f cartesianToPolar(const sf::Vector2f &coordinates) {
			return cartesianToPolar(coordinates.x, coordinates.y);
		}
	};
}