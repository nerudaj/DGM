#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	namespace ps {
		class Particle {
		protected:
			sf::Vertex *quad;
			sf::Vector2f forward; ///< Direction of particle movement
			sf::Vector2f size; ///< Particle render size

		public:
			float lifespan; ///< How long till dead

			/**
			 *  \brief Test whether lifespan is greater than zero
			 */
			bool alive() const { return (lifespan > 0.f); }

			/**
			 *  \brief Change textCoords of particle
			 */
			void changeFrame(const sf::IntRect &frame);

			/**
			 *  \brief Set forward vector for particle
			 */
			void setForward(const sf::Vector2f &forward) { Particle::forward = forward; }

			/**
			 *  \brief Get forward vector of particle
			 */
			const sf::Vector2f &getForward() const { return forward; }

			/**
			 *  \brief Sets the render size of particle
			 */
			void setSize(const sf::Vector2f &size) { Particle::size = size; }

			/**
			 *  \brief Spawns the particle
			 *  
			 *  \pre setSize()
			 */
			void spawn(const sf::Vector2f &position);

			/**
			 *  \brief Move the particle in direction of forward
			 */
			void move(const sf::Vector2f &forward);

			/**
			 *  \brief Set rotation of the particle
			 *
			 *  \param [in] angle Angle of rotation
			 */
			void setRotation(const float angle);

			/**
			 *  \brief Destroys the particle
			 *
			 *  \details Until next call to spawn(), particle will not be visible
			 *  and will not be updated in any way. Frame still can be changed.
			 */
			void destroy();

			/**
			 *  \brief Initialize the object with pointer to its vertices
			 *
			 *  \param [in] vertices Pointer to topleft vertex of particle
			 *
			 *  \details This method must be called prior to any other. If
			 *  you use the ParticleSystem template, particles are properly
			 *  initialized already.
			 */
			void init(sf::Vertex *vertices);

			Particle() {}
			~Particle() {}
		};
	};
}