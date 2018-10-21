/**
 * \file Collision.hpp
 * \author doomista
 */
 
#pragma once

namespace dgm {
	class Collision {
	public:
		/**
		 * 	\brief Tests collision between a rectangle and a point
		 * 
		 * 	\param Existing rectangle object
		 *  \param Existing sf::Vector2i serving as point (format returned by mouse position function)
		 * 
		 * 	\return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Rect &rect, const sf::Vector2i &point);
		
		/**
		 * 	\brief Tests collision between a circle and a point
		 * 
		 * 	\param Existing circle object
		 *  \param Existing sf::Vector2i serving as point (format returned by mouse position function)
		 * 
		 * 	\return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Circle &A, const dgm::Circle &B);
		
		/**
		 * 	\brief Tests collision between a circle and a circle
		 * 
		 * 	\param Existing circle object
		 *  \param Existing circle object
		 * 
		 * 	\return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Circle &circle, const sf::Vector2i &point);
		
		/**
		 * 	\brief Tests collision between a circle and a rectange
		 * 
		 * 	\param Existing rectangle object
		 *  \param Existing circle object
		 * 
		 * 	\return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Rect &A, const dgm::Circle &B);
		
		/**
		 * 	\brief Tests collision between two rectangles
		 * 
		 * 	\param Existing rectangle object
		 *  \param Existing rectangle object
		 * 
		 * 	\return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Rect &A, const dgm::Rect &B);
		
		/**
		 *  \brief Tests collision between mesh and a circle
		 * 
		 *  \param Existing mesh object
		 *  \param Existing circle object
		 * 
		 *  \return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Mesh &A, const dgm::Circle &B, int *meshHitPosition = nullptr);
		
		/**
		 *  \brief Tests collision between mesh and a rectangle
		 * 
		 *  \param Existing mesh object
		 *  \param Existing rectangle object
		 * 
		 *  \return TRUE if collision was detected. FALSE otherwise
		 */
		static bool basic (const dgm::Mesh &A, const dgm::Rect &B, int *meshHitPosition = nullptr);
		
		/**
		 *  \brief Elaborates movement of a circle within a mesh
		 * 
		 *  \param Existing mesh object
		 *  \param Existing circle object
		 *  \param Movement defining forward vector
		 * 
		 *  \details This function tries to apply the forward vector to the body object
		 *  and then tests collision with the mesh. If the collision is found, function
		 *  tries to modify the vector so that at movement can occur in at least one
		 *  direction. Neccessary for 8+ direction movement and platformers.
		 * 
		 *  \return TRUE if collision was detected. FALSE otherwise
		 */
		static bool advanced(const dgm::Mesh &mesh, const dgm::Circle &body, sf::Vector2f &forward, int *meshHitPosition = nullptr);
		
		/**
		 *  \brief Elaborates movement of a rect within a mesh
		 * 
		 *  \param Existing mesh object
		 *  \param Existing rect object
		 *  \param Movement defining forward vector
		 * 
		 *  \details This function tries to apply the forward vector to the body object
		 *  and then tests collision with the mesh. If the collision is found, function
		 *  tries to modify the vector so that at movement can occur in at least one
		 *  direction. Neccessary for 8+ direction movement and platformers.
		 * 
		 *  \return TRUE if collision was detected. FALSE otherwise
		 */
		static bool advanced(const dgm::Mesh &mesh, const dgm::Rect &body, sf::Vector2f &forward, int *meshHitPosition = nullptr);
	};
}