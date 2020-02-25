// Disallow linkage of sfml network module
#define NO_NETWORK

#include <DGM/dgm.hpp>

/**
*  \brief Example usage of \ref ParticleFactoryInterface
*
*  \details Simply generates instances of \ref Particle
*/
class SimpleParticleFactory : public dgm::ps::ParticleFactoryInterface {
public:
	virtual void reset() override {}
	virtual dgm::ps::Particle * create() override { return new dgm::ps::Particle; }
};

/**
*  \brief Example usage of \ref ParticleSystem
*
*  \details After calling init, set all public attributes to proper
*  values, then you can call \update each frame. Those attributes will
*  be used to spawn particles (exclusively white squares; you can affect that
*  with other than example \ref ParticleFactory.
*/
class SimpleParticleSystem : public dgm::ps::ParticleSystemInterface {
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

	SimpleParticleSystem() : spawnTimer(0.f) {}
};

int main() {
	dgm::Window window({1280,  720}, "Sandbox", false);
	dgm::Time time;

	//  Center of the window
	sf::Vector2f windowCenter(640.f, 360.f);

	dgm::Clip clip;
	SimpleParticleFactory particleFactory;
	SimpleParticleSystem particleSystem;

	if (!particleSystem.init(512, dgm::Clip(), &particleFactory)) return 1;

	// Setup properties of particle system
	particleSystem.averageLifespan = sf::seconds(3.f).asSeconds();
	particleSystem.lifespanDelta = sf::seconds(0.5f).asSeconds();
	// Angle is setup in main loop
	particleSystem.emitRange = 45.f; // Spread of emmiter
	particleSystem.emitForce = 256.f;
	particleSystem.emitForceDelta = 64.f;
	particleSystem.emitPosition = { 640.f, 360.f };
	particleSystem.globalForce = { 0.f, 100.f };
	particleSystem.particlesPerSec = 100;

	// Main loop
	sf::Event event;
	while (window.isOpen()) {
		// Poll events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Logic
		auto mousePos = sf::Mouse::getPosition(window.getWindowContext());

		// Compute polar coordinates to mouse relative to center of window
		auto toMouseAsPolar = dgm::Conversion::cartesianToPolar(sf::Vector2f(mousePos) - windowCenter);

		// Aim particle emmiter at mouse
		particleSystem.emitAngle = toMouseAsPolar.x;

		particleSystem.update(time);
		time.reset();

		// Rendering
		window.beginDraw(sf::Color::Black);

		particleSystem.draw(window);

		window.endDraw();
	}

	return 0;
}

// PARTICLE SYSTEM IMPLEMENTATION
float randomFloat(const float range) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX) / range);
}

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
	spawnTimer += time.getDeltaTime();

	while (spawnTimer > SPAWN_TIMEOUT) {
		spawnTimer -= SPAWN_TIMEOUT;
		spawnParticle();
	}

	// Particles are stored in dgm::Buffer
	for (unsigned i = 0; i < particles.size(); i++) {
		particles[i]->lifespan -= time.getDeltaTime();
		if (not particles[i]->alive()) {
			particles[i]->destroy();
			particles.remove(i);
		}

		particles[i]->forward += globalForce * time.getDeltaTime();
		particles[i]->move(particles[i]->forward * time.getDeltaTime());
	}
}