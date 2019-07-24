#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	namespace ps {
		/**
		 * \brief Interface for creating ParticleFactories
		 */
		class ParticleFactoryInterface {
		public:
			/**
			 * \brief Reset ther inner state of a factory
			 *
			 *  \details This method is the first thing that is
			 *  called in ParticleSystem::init()
			 */
			virtual void reset() = 0;

			/**
			 *  \brief Returns new \ref Particle instance
			 */
			virtual dgm::ps::Particle* create() const = 0;
		};

		/**
		 *  \brief Interface and base implementation of ParticleSystem
		 *
		 *  \details Always inherit from this class when creating new
		 *  particle system.
		 */
		class ParticleSystemInterface {
		protected:
			dgm::ps::ParticleSystemRenderer renderer;
			Buffer<dgm::ps::Particle*> particles;
		
		public:
			/**
			 *  \brief Render particle system to target window
			 */
			void draw(dgm::Window &window) { window.draw(renderer); }
		
			/**
			 *  \brief Get reference to renderer object
			 */
			dgm::ps::ParticleSystemRenderer &getRenderer() { return renderer; }
			
			/**
			*  \brief Get reference to renderer object
			*/
			const dgm::ps::ParticleSystemRenderer &getRenderer() const { return renderer; }
			
			void setTexture(sf::Texture *texture) { renderer.setTexture(texture); }

			bool init(const std::size_t particleCount, const dgm::Clip &clip, dgm::ps::ParticleFactoryInterface *factory);
			
			/**
			 *  \brief Update particle system
			 * 
			 *  \details This is the only method you have to actually
			 *  implement in your inherited particle system.
			 */
			virtual void update(const dgm::Time &time) =0;

			ParticleSystemInterface();
			~ParticleSystemInterface();
		};
	};
};