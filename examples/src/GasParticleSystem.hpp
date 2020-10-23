#pragma once

#include <DGM\dgm.hpp>

class GasParticle : public dgm::ps::Particle {
public:
	unsigned i;
	GasParticle(unsigned i) : i(i) {}
};

class GasSystem : public dgm::ps::ParticleSystemInterface {
private:
	void initParticle(GasParticle* particle) {
		particle->size = sf::Vector2f(clip.getFrameSize());
		particle->spawn(emitterPosition);
		particle->changeFrame(clip.getFrame(particle->i % clip.getFrameCount()));
		particle->forward = { 0.f, 0.f };
	}

	virtual dgm::ps::Particle* createParticle(unsigned index) const override {
		return new GasParticle(index);
	}

public:
	sf::Vector2f emitterPosition;
	const dgm::Mesh* level;
	float MAX_VELOCITY = 256.f;
	float angle = 0.f;
	dgm::Clip clip;

	virtual void update(const dgm::Time& time) override {
		// Compute new forwards for each particle
		for (unsigned i = 0; i < particles.size(); i++) {
			for (unsigned j = i + 1; j < particles.size(); j++) {
				auto direction = particles[i]->getPosition() - particles[j]->getPosition();
				float distance = relativeVectorSize(direction);

				float velocity = distance == 0.f ? MAX_VELOCITY : MAX_VELOCITY / distance;
				
				if (distance == 0.f) {
					direction = dgm::Conversion::polarToCartesian(angle, 1.f);
					angle++;
				}
				else {
					direction /= sqrt(distance);
				}

				particles[i]->forward += direction * velocity;
				particles[j]->forward -= direction * velocity;
			}
		}

		// Move particles
		for (auto particle : particles) {
			dgm::Circle body(particle->getPosition());
			body.setRadius(particle->size.x / 2.f);

			float size = dgm::Math::vectorSize(particle->forward);
			float sizeNormalized = dgm::Math::clamp(size, 0.f, MAX_VELOCITY * 2);
			auto forward = particle->forward / size * sizeNormalized * time.getDeltaTime();

			dgm::Collision::advanced(*level, body, forward);
			particle->move(forward);
			particle->forward = sf::Vector2f(0.f, 0.f);
		}
	}

	virtual bool init(const std::size_t particleCount) override {
		if (not dgm::ps::ParticleSystemInterface::init(particleCount)) return false;

		while (particles.expand()) {
			initParticle(dynamic_cast<GasParticle*>(particles.last()));
		}

		return true;
	}
};