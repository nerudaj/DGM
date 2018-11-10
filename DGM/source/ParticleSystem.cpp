#include <DGM\dgm.hpp>

using dgm::ps::ParticleSystem;
using dgm::ps::SimpleParticleSystem;

// *** PARTICLE SYSTEM ***
bool ParticleSystem::init(const std::size_t particleCount, const dgm::Clip &clip, dgm::ps::ParticleFactoryInterface *factory) {
	if (not renderer.init(particleCount, clip)) return false;
	factory->reset();
	
	try {
		particles.resize(particleCount);

		unsigned cnt = 0;
		for (auto &particle : particles) {
			cnt++;
			particle = factory->create();
			particle->init(renderer.getParticleVertices(cnt));
		}
	}
	catch(...) {
		return false;
	}
	
	return true;
}

dgm::ps::ParticleSystem::ParticleSystem() {
}

dgm::ps::ParticleSystem::~ParticleSystem() {
	for (auto &particle : particles) {
		delete particle;
	}
}

float randomFloat(const float range) {
	uint32_t r = uint32_t(range * 1000.f);
	return (rand() % r) / 1000.f;
}

// *** SIMPLE PARTICLE SYSTEM ***
void SimpleParticleSystem::spawnParticle() {
	if (not particles.expand()) return;

	dgm::ps::Particle *particle = particles.last();
	particle->size = { 5.f, 5.f };
	float force = emitForce + randomFloat(emitForceDelta) - emitForceDelta / 2.f;
	float angle = emitAngle + randomFloat(emitRange) - emitRange / 2.f;
	particle->forward = dgm::Conversion::polarToCartesian(angle, force);
	float lifespan = averageLifespan + randomFloat(lifespanDelta) - lifespanDelta / 2.f;
	particle->spawn(emitPosition);
	particle->lifespan = lifespan;
	particle->changeFrame(sf::IntRect(0, 0, 10, 10));
}

void SimpleParticleSystem::update(const dgm::Time &time) {
	const float SPAWN_TIMEOUT = sf::seconds(1).asSeconds() / particlesPerSec;
	spawnTimer += time.deltaTime();
	
	while (spawnTimer > SPAWN_TIMEOUT) {
		spawnTimer -= SPAWN_TIMEOUT;
		spawnParticle();
	}
	
	unsigned cnt = 0;
	for (auto &particle : particles) {
		cnt++;
		particle->lifespan -= time.deltaTime();
		if (not particle->alive()) {
			particle->destroy();
			particles.remove(cnt);
		}

		particle->forward = particle->forward + (globalForce * time.deltaTime());
		particle->move(particle->forward * time.deltaTime());
	}
}

SimpleParticleSystem::SimpleParticleSystem() {
	spawnTimer = 0.f;
}

SimpleParticleSystem::~SimpleParticleSystem() {
}
