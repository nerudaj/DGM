#define NO_NETWORK

#include <DGM/dgm.hpp>
#include "Level.hpp"
#include <cstdio>

class SandParticle : public dgm::ps::Particle {
public:
	dgm::Circle body;

	void move(const sf::Vector2f &vec) override {
		body.move(vec);
		Particle::move(vec);
	}

	void spawn(const sf::Vector2f &position) override {
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
	virtual dgm::ps::Particle * create() override { return new SandParticle; }
};

float relativeVectorSize(const sf::Vector2f& vec) {
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

public:
	sf::Vector2f emitterPosition;
	sf::Vector2f gravity;
	const dgm::Mesh *level;
	bool left = true;

	void update(const dgm::Time &time) override {
		const sf::Time SPAWN_TIMEOUT = sf::seconds(0.2f);

		spawnTimer += time.getElapsed();
		if (spawnTimer >= SPAWN_TIMEOUT) {
			spawnTimer -= SPAWN_TIMEOUT;

			if (particles.expand()) {
				particles.last()->size = sf::Vector2f(1.f, 1.f) * 2.f * PARTICLE_RADIUS;
				particles.last()->spawn(emitterPosition);
				particles.last()->changeFrame(sf::IntRect(0, 0, 10, 10));
			}
		}

		updateNearestNeighboursMatrix(time.getDeltaTime());

		for (unsigned i = 0; i < particles.size(); i++) {
			auto* particle = dynamic_cast<SandParticle*>(particles[i]);
			auto body = particle->body;
			auto actualGravity = gravity * time.getDeltaTime();
			sf::Vector2f forward(0.f, gravity.y);
			body.move(actualGravity);

			auto &nearest = nearestNeighbours[i];

			for (auto other : nearest) {
				if (dgm::Collision::basic(body, other->body)) {
					body.move(-actualGravity);
					forward.y = 0.f;

					float myX = particle->getPosition().x;
					float otherX = other->getPosition().x;
					particle->forward.x = myX - otherX > 0 ? X_VELOCITY : -X_VELOCITY;
				}
			}

			forward.x = particle->forward.x;
			auto actualSideward = particle->forward * time.getDeltaTime();
			body.move(actualSideward);
			for (auto other : nearest) {
				if (dgm::Collision::basic(body, other->body)) {
					body.move(-actualSideward);
					forward.x = 0.f;
				}
			}

			forward *= time.getDeltaTime();
			dgm::Collision::advanced(*level, particle->body, forward);
			particle->move(forward);
			float sign = particle->forward.x < 0.f ? -1.f : 1.f;
			particle->forward.x = sign * dgm::Math::clamp(sign * particle->forward.x - 20.f * time.getDeltaTime(), 0.f, X_VELOCITY);
		}
	}

	virtual bool init(const std::size_t particleCount, const dgm::Clip& clip, dgm::ps::ParticleFactoryInterface* factory) override {
		nearestNeighbours.resize(particleCount);
		return ParticleSystemInterface::init(particleCount, clip, factory);
	}
};

dgm::ResourceManager getInitializedResourceMgr();
void exportLevel();

int main(int argc, char *argv[]) {
	freopen("stdout.txt", "w", stdout);

	exportLevel();

	dgm::Window window;
	window.open({ 1280, 720 }, "Advanced particle systems", false);

	dgm::Time time;
	dgm::ResourceManager resmgr = getInitializedResourceMgr();

	Level level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile("../examples/11_advanced_particle_system.lvd");

	SandSystem sandSystem;
	if (!sandSystem.init(250, dgm::Clip(), new SandParticleFactory())) return 1;
	sandSystem.gravity = { 0.f, 100.f };
	sandSystem.emitterPosition = { 150.f, 40.f };
	sandSystem.level = &(level.getMesh());
	time.reset();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) 
				level.changeTileToVoid(4, 3);
				level.changeTileToVoid(5, 6);
				level.changeTileToVoid(4, 9);
				level.changeTileToVoid(5, 12);
				level.changeTileToVoid(4, 15);
			}
		}

		/* LOGIC */
		sandSystem.update(time);
		time.reset();

		/* RENDER */
		window.beginDraw();

		level.draw(window);
		sandSystem.draw(window);

		window.endDraw();
	}

	return 0;
}

dgm::ResourceManager getInitializedResourceMgr() {
	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false);
	resmgr.loadResourceDir<sf::Texture>("../examples");
	return resmgr;
}

void exportLevel() {
	LevelD lvld;

	// Once a certain attribute of lvld is initialized, it will become the part of export
	lvld.metadata.author = "doomista";
	lvld.metadata.description = "Basic level for example 11";
	lvld.metadata.timestamp = time(NULL);
	lvld.metadata.id = "EXAMPLE_11";
	lvld.metadata.name = "Example Level";

	lvld.mesh.tileWidth = TILE_SIZE;
	lvld.mesh.tileHeight = TILE_SIZE;
	lvld.mesh.width = 30;
	lvld.mesh.height = 19;
	// Indices of tiles in tileset
	lvld.mesh.tiles = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	// Collision information
	lvld.mesh.blocks = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	lvld.saveToFile("../examples/11_advanced_particle_system.lvd");
}