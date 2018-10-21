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
		 *  \brief Example usage of \ref ParticleFactoryInterface
		 *  
		 *  \details Simply generates instances of \ref Particle
		 */
		class ParticleFactory : public ParticleFactoryInterface {
		public:
			// Dìdí se pøes ParticleFactoryInterface.
			virtual void reset() override {}
			virtual dgm::ps::Particle * create() const override { return new dgm::ps::Particle; }
		};

		/**
		 *  \brief Interface and base implementation of ParticleSystem
		 *
		 *  \details Always inherit from this class when creating new
		 *  particle system.
		 */
		class ParticleSystem {
		protected:
			dgm::ps::ParticleSystemRenderer renderer;
			dgm::Buffer<dgm::ps::Particle*> particles;
		
		public:
			void draw(dgm::Window &window) { window.draw(renderer); }
		
			dgm::ps::ParticleSystemRenderer &getRenderer() { return renderer; }
			
			const dgm::ps::ParticleSystemRenderer &getRenderer() const { return renderer; }
			
			void setTexture(sf::Texture *texture) { renderer.setTexture(texture); }
			
			virtual void update(const dgm::Time &time) =0;
			
			bool init(const std::size_t particleCount, const dgm::Clip &clip, dgm::ps::ParticleFactoryInterface *factory);

			ParticleSystem();
			~ParticleSystem();
		};
		
		/**
		 *  \brief Example usage of \ref ParticleSystem
		 *  
		 *  \details After calling init, set all public attributes to proper
		 *  values, then you can call \update each frame. Those attributes will
		 *  be used to spawn particles (exclusively white squares; you can affect that
		 *  with other than example \ref ParticleFactory.
		 */
		class SimpleParticleSystem : public dgm::ps::ParticleSystem {
		protected:
			void spawnParticle();
			float spawnTimer; ///< Internal clock for controlling particle spawning
			
		public:
			float emitForce; ///< Average initial forward momentum of particle
			float emitForceDelta; ///< How much can forward momentum differ from average value
			sf::Vector2f globalForce; ///< Force vector applied to existing particles each second
			sf::Vector2f emitPosition; ///< Where will be particles spawned from
			float emitAngle; ///< Angle (clockwise, 0° = 12o'clock) under which will be particles fired from emitter
			float emitRange; ///< Difference from base emit angle
			float averageLifespan; ///< How long will particle live on average
			float lifespanDelta; ///< How much can lifespan duration differ from average value
			int particlesPerSec; ///< How many particles per second will be produced
		
			virtual void update(const dgm::Time &time) override;

			SimpleParticleSystem();
			~SimpleParticleSystem();
		};
	};
};