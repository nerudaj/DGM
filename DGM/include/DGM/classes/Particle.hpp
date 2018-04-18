#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	namespace ps {
		class Particle {
		protected:
			sf::Vertex *quad;

		public:
			float lifespan; ///< How long till dead
			sf::Vector2f forward; ///< Direction of particle movement
			sf::Vector2f size; ///< Particle render size

			sf::Vector2f getPosition() const { return quad[0].position + size / 2.f; }

			/**
			 *  \brief Test whether lifespan is greater than zero
			 */
			bool alive() const { return (lifespan > 0.f); }

			/**
			 *  \brief Change textCoords of particle
			 */
			void changeFrame(const sf::IntRect &frame);

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