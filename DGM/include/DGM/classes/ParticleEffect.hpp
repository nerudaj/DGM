#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class ParticleEffect : public sf::Drawable, public sf::Transformable {
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		sf::Vector2f toCartesian(const float angle, const float distance) const;

		sf::Int32 getRandomNumber(const sf::Int32 base, const sf::Int32 delta) const;

		float getRandomNumber(const float base, const float delta) const;

		void setVerticesPosition(const std::size_t index, const sf::Vector2f &position);

		void spawn(const std::size_t index);

	protected:
		// Render values
		std::size_t used;
		std::vector<sf::Vertex> vertices;
		const sf::Texture *texture;

		struct ParticleProperties {
			sf::Vector2f forward;
			sf::Time lifespan;
		};

		// Particle values
		std::vector<ParticleProperties> particles;
		sf::Vector2f particleSize;
		sf::Time particleLifespan, particleLifespanDelta;
		float particleVelocity, particleVelocityDelta;

		// Emmiter values
		float emitRange;
		float emitAngle;

		// Other values
		sf::Vector2f enviroForce;
		int particlesPerSecond;

	public:
		void update(const sf::Time &elapsed);

		/**
		*  \brief Sets the max deviation from 0° at which particle can be propelled
		*
		*  \param [in] range Angle from 0° to 180°
		*
		*  \details This will only take effect when emitter type is set to Circular
		*/
		void setEmitterRange(const float range);

		/**
		*  \brief Sets the average particle lifespan
		*
		*  \param [in] lifespan Base lifespan
		*  \param [in] deviation How much can randomly generated lifespan deviate from base value
		*
		*  \details Will work on both types of emitters
		*/
		void setParticleLifespan(const sf::Time &lifespan, const sf::Time &deviation);

		/**
		*  \brief Sets the average particle forward velocity
		*
		*  \param [in] velocity Base velocity
		*  \param [in] deviation How much can randomly generated velocity deviate from base value
		*
		*  \details Will only work with Circular emitter type
		*/
		void setParticleVelocity(const float velocity, const float deviation);

		void setEnvironmentForce(const sf::Vector2f &force);

		void setEmitterRotation(const float angle);

		void rotateEmitter(const float diff);

		void setTexture(const sf::Texture *texture);

		void setParticlesPerSecond(const int pps);

		bool init(const int size, const dgm::Clip &clip);

		/**
		*  \brief Resets particle system logic back to start
		*/
		void reset();

		ParticleEffect() {
			emitAngle = 0;
		}
		~ParticleEffect() {}
	};
};