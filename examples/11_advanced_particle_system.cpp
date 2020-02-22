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

		size = { 2.f, 2.f }; // Size must be set prior to Partile::spawn
		body.setRadius(1.f);

		body.setPosition(position);
		Particle::spawn(position);

		for (unsigned i = 0; i < 4; i++) {
			quad[i].color = sf::Color::Yellow;
		}
	}
};

class SandParticleFactory : public dgm::ps::ParticleFactoryInterface {
public:
	virtual void reset() override {}
	virtual dgm::ps::Particle * create() const override { return new SandParticle; }
};

class SandSystem : public dgm::ps::ParticleSystemInterface {
private:
	sf::Time spawnTimer = sf::seconds(0.f);

	unsigned particleCollidesWithOther(unsigned i) {
		SandParticle *first = dynamic_cast<SandParticle*>(particles[i]);

		for (unsigned j = 0; j < particles.size(); j++) {
			if (i == j) continue;
			SandParticle *second = dynamic_cast<SandParticle*>(particles[j]);

			if (dgm::Collision::basic(first->body, second->body)) {
				return j;
			}
		}

		return i;
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
				particles.last()->spawn(emitterPosition);
				particles.last()->changeFrame(sf::IntRect(0, 0, 10, 10));
/*				particles.last()->forward.x = left ? -10.f : 10.f;
				left = !left;*/
			}
		}		

		for (unsigned i = 0; i < particles.size(); i++) {
			SandParticle *particle = dynamic_cast<SandParticle*>(particles[i]);

			auto currentForward = (particle->forward + gravity) * time.getDeltaTime();

			particle->move(currentForward);

			if (dgm::Collision::basic(*level, particle->body)) {
				particle->move(-currentForward);
				continue;
			}

			unsigned ptr;
			if ((ptr = particleCollidesWithOther(i)) != i) {
				particle->move(-currentForward);

				particle->forward.x = (particles[i]->getPosition() - particles[ptr]->getPosition()).x >= 0.f ? 10.f : -10.f;

				particle->move(particle->forward * time.getDeltaTime());

				if ((ptr = particleCollidesWithOther(i)) != i) {
					particle->move(-particle->forward * time.getDeltaTime());
				}
			}
			else {
				particle->forward *= 0.5f;
			}
		}
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
	sandSystem.gravity = { 0.f, 50.f };
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