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
			 *  \brief Returns new Particle instance
			 */
			virtual dgm::ps::Particle* create() const = 0;
		};

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
		
		class SimpleParticleSystem : public dgm::ps::ParticleSystem {
		protected:
			void spawnParticle();
			float spawnTimer;
			
		public:
			float emitForce, emitForceDelta;
			sf::Vector2f globalForce, emitPosition;
			float emitAngle, emitRange;
			float averageLifespan, lifespanDelta;
			int particlesPerSec;
		
			virtual void update(const dgm::Time &time) override;

			SimpleParticleSystem();
			~SimpleParticleSystem();
		};
	};
};