#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	namespace ps {
		class ParticleSystem {
		protected:
			dgm::ps::ParticleSystemRenderer renderer;
			dgm::Buffer<dgm::ps::Particle> particles;
		
		public:
			void draw(dgm::Window &window) { window.draw(renderer); }
		
			dgm::ps::ParticleSystemRenderer &getRenderer() { return renderer; }
			
			const dgm::ps::ParticleSystemRenderer &getRenderer() const { return renderer; }
			
			void setTexture(sf::Texture *texture) { renderer.setTexture(texture); }
			
			virtual void update(const dgm::Time &time) =0;
			
			bool init(const std::size_t particleCount, const dgm::Clip &clip);
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