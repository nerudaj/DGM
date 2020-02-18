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
		body.setPosition(position);
		Particle::spawn(position);
	}

	SandParticle(const sf::Vector2f &forward) {
		Particle::forward = forward;
		size = { 2.f, 2.f };
		body.setRadius(1.f);
	}
};

class SandParticleFactory : public dgm::ps::ParticleFactoryInterface {
public:
	virtual void reset() override {}

	virtual dgm::ps::Particle * create() const override {
		//return new dgm::ps::Particle;
		return new SandParticle({ 0.f, 0.f });
	}
};

class SandSystem : public dgm::ps::ParticleSystemInterface {
private:
	sf::Time spawnTimer = sf::seconds(0.f);

	bool particleCollidesWithOther(unsigned i) {
		SandParticle *first = dynamic_cast<SandParticle*>(particles[i]);

		for (unsigned j = 0; j < particles.size(); j++) {
			if (i == j) continue;
			SandParticle *second = dynamic_cast<SandParticle*>(particles[j]);

			if (dgm::Collision::basic(first->body, second->body)) {
				return true;
			}
		}

		return false;
	}

public:
	sf::Vector2f emitterPosition;
	sf::Vector2f gravity;
	const dgm::Mesh *level;
	bool left = true;

	void update(const dgm::Time &time) override {
		const sf::Time SPAWN_TIMEOUT = sf::seconds(0.5f);

		spawnTimer += time.getElapsed();
		if (spawnTimer >= SPAWN_TIMEOUT) {
			spawnTimer -= SPAWN_TIMEOUT;

			if (particles.expand()) {
				//particles.last()->size = { 2.f, 2.f };
				particles.last()->spawn(emitterPosition);
				particles.last()->changeFrame(sf::IntRect(0, 0, 10, 10));
				particles.last()->forward.x = left ? -10.f : 10.f;
				left = !left;
			}
		}		

		for (unsigned i = 0; i < particles.size(); i++) {
			auto currentForward = gravity * time.getDeltaTime();
			SandParticle *particle = dynamic_cast<SandParticle*>(particles[i]);

			particle->move(currentForward);

			if (dgm::Collision::basic(*level, particle->body)) {
				particle->move(-currentForward);
				continue;
			}

			if (particleCollidesWithOther(i)) {
				particle->move(-currentForward);
				particle->move(particle->forward * time.getDeltaTime());

				if (particleCollidesWithOther(i)) {
					particle->move(-particle->forward * time.getDeltaTime());
				}
			}

/*			for (unsigned j = 0; j < particles.size(); j++) {
				if (j == i) continue;

				SandParticle *particle2 = dynamic_cast<SandParticle*>(particles[j]);

				if (dgm::Collision::basic(particle->body, particle2->body)) {
					particle->move(-currentForward);
					break;
				}
			}*/
		}
	}
};

dgm::ResourceManager getInitializedResourceMgr();
void exportLevel();

int main(int argc, char *argv[]) {
	freopen("stdout.txt", "w", stdout);

	exportLevel();

	dgm::Window window;
	window.open({ 1280, 720 }, "Sand simulation", false);

	dgm::Time time;
	dgm::ResourceManager resmgr = getInitializedResourceMgr();

	Level level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile("../examples/11_sand_simulation.lvd");

	SandSystem sandSystem;
	if (!sandSystem.init(250, dgm::Clip(), new SandParticleFactory())) return 1;
	sandSystem.gravity = { 0.f, 20.f };
	sandSystem.emitterPosition = { 150.f, 40.f };
	sandSystem.level = &(level.getMesh());

/*	sandSystem.emitPosition = { 160.f, 40.f };
	sandSystem.gravity = { 0.f, 20.f };
	sandSystem.particlesPerSec = 100;
	*/
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
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
	lvld.mesh.width = 10;
	lvld.mesh.height = 19;
	// Indices of tiles in tileset
	lvld.mesh.tiles = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	// Collision information
	lvld.mesh.blocks = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	lvld.saveToFile("../examples/11_sand_simulation.lvd");
}