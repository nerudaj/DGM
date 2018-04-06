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

			bool alive() const { return (lifespan > 0.f); }

			void changeFrame(const sf::IntRect &frame);

			void setForward(const sf::Vector2f &forward) { Particle::forward = forward; }

			const sf::Vector2f &getForward() const { return forward; }

			void setSize(const sf::Vector2f &size) { Particle::size = size; }

			/**
			 *  \brief Spawns the particle
			 *  
			 *  \pre setSize()
			 */
			void spawn(const sf::Vector2f &position);

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

			void init(sf::Vertex *vertices);

			Particle() {}
			~Particle() {}
		};
	};
}