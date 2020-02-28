#pragma once

#include <DGM\dgm.hpp>

class SandParticle : public dgm::ps::Particle {
public:
	dgm::Circle body;

	void move(const sf::Vector2f& vec) override {
		body.move(vec);
		Particle::move(vec);
	}

	void spawn(const sf::Vector2f& position) override {
		// NOTE: All of initialization must happen here
		body.setRadius(size.x / 2.f);
		body.setPosition(position + size / 2.f);
		Particle::spawn(position);

		for (unsigned i = 0; i < 4; i++) {
			quad[i].color = sf::Color::Yellow;
		}
	}
};

class SandParticleFactory : public dgm::ps::ParticleFactoryInterface {
public:
	virtual void reset() override {}
	virtual dgm::ps::Particle* create() override { return new SandParticle; }
};

static float relativeVectorSize(const sf::Vector2f& vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

class SandSystem : public dgm::ps::ParticleSystemInterface {
private:
	const float X_VELOCITY = 10.f;
	const float PARTICLE_RADIUS = 1.f;

	std::vector<std::vector<SandParticle*>> nearestNeighbours;
	sf::Time spawnTimer = sf::seconds(0.f);

	void updateNearestNeighboursMatrix(float deltaTime) {
		for (auto& vec : nearestNeighbours) {
			vec.clear();
			vec.reserve(30);
		}

		float maxDistance = getMaxDistance(deltaTime);

		for (unsigned i = 0; i < particles.size(); i++) {
			auto position = particles[i]->getPosition();

			for (unsigned j = i + 1; j < particles.size(); j++) {
				auto otherPos = particles[j]->getPosition();

				if (relativeVectorSize(position - otherPos) <= maxDistance) {
					nearestNeighbours[i].push_back(dynamic_cast<SandParticle*>(particles[j]));
					nearestNeighbours[j].push_back(dynamic_cast<SandParticle*>(particles[i]));
				}
			}
		}
	}

	/**
	 *  Get maximum distance between two particles that
	 *  both can traverse in a single frame in order to collide
	 */
	float getMaxDistance(float deltaTime) const {
		auto maxMovement = sf::Vector2f(X_VELOCITY, gravity.y) * deltaTime;
		sf::Vector2f particleSize(PARTICLE_RADIUS * 2.f, PARTICLE_RADIUS * 2.f);
		return relativeVectorSize((maxMovement + particleSize) * 2.f);
	}

	void spawnParticle() {
		if (not particles.expand()) return;

		auto* particle = particles.last();
		particle->size = sf::Vector2f(1.f, 1.f) * 2.f * PARTICLE_RADIUS;
		particle->spawn(emitterPosition);
		particle->changeFrame(sf::IntRect(0, 0, 10, 10));
	}

	void updateParticle(SandParticle *particle, const std::vector<SandParticle*> &nearest, const float deltaTime) {
		auto body = particle->body;
		auto actualGravity = gravity * deltaTime;
		sf::Vector2f forward(0.f, gravity.y);
		body.move(actualGravity);

		// Can I move vertically without colliding with another particle?
		for (auto other : nearest) {
			if (dgm::Collision::basic(body, other->body)) {
				body.move(-actualGravity);
				forward.y = 0.f;

				float myX = particle->getPosition().x;
				float otherX = other->getPosition().x;
				particle->forward.x = myX - otherX > 0 ? X_VELOCITY : -X_VELOCITY;
			}
		}

		// Can I move horizontally without colliding with another particle?
		forward.x = particle->forward.x;
		auto actualSideward = particle->forward * deltaTime;
		body.move(actualSideward);
		for (auto other : nearest) {
			if (dgm::Collision::basic(body, other->body)) {
				body.move(-actualSideward);
				forward.x = 0.f;
			}
		}

		// Try to move in allowed directions within the level
		forward *= deltaTime;
		dgm::Collision::advanced(*level, particle->body, forward);
		particle->move(forward);

		// gradually reduce X velocity to 0 in 0.5 second
		float sign = particle->forward.x < 0.f ? -1.f : 1.f;
		particle->forward.x = sign * dgm::Math::clamp(sign * particle->forward.x - 20.f * deltaTime, 0.f, X_VELOCITY);
	}

public:
	sf::Vector2f emitterPosition;
	sf::Vector2f gravity;
	const dgm::Mesh* level;
	bool left = true;

	void update(const dgm::Time& time) override {
		const sf::Time SPAWN_TIMEOUT = sf::seconds(0.2f);

		spawnTimer += time.getElapsed();
		if (spawnTimer >= SPAWN_TIMEOUT) {
			spawnTimer -= SPAWN_TIMEOUT;

			spawnParticle();
		}

		updateNearestNeighboursMatrix(time.getDeltaTime());

		for (unsigned i = 0; i < particles.size(); i++) {
			auto* particle = dynamic_cast<SandParticle*>(particles[i]);
			auto& nearest = nearestNeighbours[i];
			updateParticle(particle, nearest, time.getDeltaTime());

			
		}
	}

	virtual bool init(const std::size_t particleCount, const dgm::Clip& clip, dgm::ps::ParticleFactoryInterface* factory) override {
		nearestNeighbours.resize(particleCount);
		return ParticleSystemInterface::init(particleCount, clip, factory);
	}
};