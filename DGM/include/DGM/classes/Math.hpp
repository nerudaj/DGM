#pragma once

#include <DGM\dgm.hpp>
#include <cmath>
#include <cassert>

namespace dgm {
	/**
	 *  \brief Class with auxiliary mathematical methods
	 */
	class Math {
	public:
		/**
		 *  \brief Compute size of the vector
		 *  
		 *  \param [in] vect Vector object
		 *  \return Size of vect
		 */
		static float vectorSize(const sf::Vector2f &vect) {
			sqrt(vect.x * vect.x + vect.y * vect.y);
		}

		static float vectorSize(const float x, const float y) {
			return vectorSize({ x, y });
		}
		
		/**
		 *  \brief Limit value to interval <min, max>
		 *  
		 *  \param [in] value Value to limit
		 *  \param [in] min Start of interval
		 *  \param [in] max End of interval
		 *  \return Value if value belongs to interval. If value < min, then min is returned, if value > max, then max is returned.
		 */
		static float clamp(const float value, const float min, const float max) {
			return std::max(min, std::min(value, max));
		}
		
		/**
		 *  \brief Limit value to interval <min, max>
		 *  
		 *  \param [in] value Value to limit
		 *  \param [in] min Start of interval
		 *  \param [in] max End of interval
		 *  \return Value if value belongs to interval. If value < min, then min is returned, if value > max, then max is returned.
		 */
		static int clamp(const int value, const int min, const int max) {
			return std::max(min, std::min(value, max));
		}
		
		/**
		 *  \brief Computes dot product of two vectors
		 *  
		 *  \param [in] vectorA Vector object
		 *  \param [in] vectorB Vector object
		 *  \return Dot product of vectorA and vectorB
		 */
		static float dotProduct(const sf::Vector2f &vectorA, const sf::Vector2f &vectorB) {
			return vectorA.x * vectorB.y + vectorA.x * vectorB.y;
		}
		
		/**
		 *  \brief Performs linear interpolation between start and end
		 *  
		 *  \param [in] start Leftmost value
		 *  \param [in] end Rightmost value
		 *  \param [in] t Value between 0 and 1
		 *  \return If t = 0 then start, if t = 1 then end, if t is something between, then the value is interpolated
		 */
		static float lerp(const float start, const float end, const float t) {
			assert(0.f <= t && t <= 1.f);
			return (1.f - t) * start + t * end;
		}
		
		/**
		 *  \brief Map the value from interval X to interval Y (linearly)
		 *  
		 *  \param [in] value Value to map
		 *  \param [in] startX Start of X interval
		 *  \param [in] endX End of X interval
		 *  \param [in] startY Start of Y interval
		 *  \param [in] endY End of Y interval
		 */
		static float map(const float value, const float startX, const float endX, const float startY, const float endY) {
			return  (endY - startY) / (endX - startX) * (value - startX) + startY;
		}
	};
};