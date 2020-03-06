#include <DGM/dgm.hpp>
#include <queue>
#include "RootDir.hpp"

dgm::ResourceManager getInitializedResourceMgr() {
	dgm::ResourceManager resmgr;
	resmgr.setPedantic(false);
	resmgr.loadResourceDir<sf::Texture>(rootDir);
	resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(rootDir);
	return resmgr;
}

struct Light {
	sf::Vector2f position;
	float range;
	uint8_t intensity;
};

class IlluminatedTileMap : public dgm::TileMap {
public:
	void setTileLight(unsigned x, unsigned y, uint8_t intensity) {
		sf::Vertex* quad = &vertices[(y * dataSize.x + x) * size_t(4)];

		for (unsigned i = 0; i < 4; i++) {
			quad[i].color = sf::Color(255, 255, 255, intensity);
		}
	}

	void clearLights() {
		for (unsigned y = 0; y < dataSize.y; y++) {
			for (unsigned x = 0; x < dataSize.x; x++) {
				setTileLight(x, y, 0);
			}
		}
	}
};

class LightedLevel {
private:
	IlluminatedTileMap tilemap;
	dgm::Mesh mesh;
	Light* light;

	sf::Vector2u worldToTileCoord(const sf::Vector2f &position) const {
		return sf::Vector2u(
			unsigned(position.x) / mesh.getVoxelSize().x,
			unsigned(position.y) / mesh.getVoxelSize().y
		);
	}

	unsigned getMeshIndex(const sf::Vector2u &position) {
		return unsigned(position.y * mesh.getDataSize().x + position.x);
	}

public:
	void draw(dgm::Window &window) {
		window.draw(tilemap);
	}

	void update() {
		tilemap.clearLights();

		struct TileLight {
			sf::Vector2u position;
			uint8_t intensity;

			TileLight(const sf::Vector2u& position, uint8_t intensity) : position(position), intensity(intensity) {}
		};

		std::queue<TileLight> queue;
		queue.push({ worldToTileCoord(light->position), light->intensity });

		std::vector<bool> visited(mesh.getDataSize().x* mesh.getDataSize().y, false);

		while (! queue.empty()) {
			auto item = queue.front();
			queue.pop();

			unsigned i = getMeshIndex(item.position);

			if (visited[i]) continue;
			else visited[i] = true;

			tilemap.setTileLight(item.position.x, item.position.y, item.intensity);

			if (mesh[i] || item.intensity < 50) continue;

			queue.push(TileLight({ item.position.x - 1, item.position.y }, item.intensity - 30));
			queue.push(TileLight({ item.position.x + 1, item.position.y }, item.intensity - 30));
			queue.push(TileLight({ item.position.x, item.position.y - 1 }, item.intensity - 30));
			queue.push(TileLight({ item.position.x, item.position.y + 1 }, item.intensity - 30));
			queue.push(TileLight({ item.position.x - 1, item.position.y - 1 }, item.intensity - 42));
			queue.push(TileLight({ item.position.x + 1, item.position.y + 1 }, item.intensity - 42));
			queue.push(TileLight({ item.position.x - 1, item.position.y + 1 }, item.intensity - 42));
			queue.push(TileLight({ item.position.x + 1, item.position.y - 1 }, item.intensity - 42));
		}
	}

	void loadFromFile(const std::string &filename) {
		LevelD lvd;
		lvd.loadFromFile(filename);

		tilemap.build(lvd);
		mesh = dgm::Mesh(lvd);
	}

	void setLightSource(Light* light) {
		LightedLevel::light = light;
	}

	const dgm::Mesh &getMesh() const {
		return mesh;
	}

	LightedLevel(sf::Texture &texture) {
		dgm::Clip clip({ 32, 32 }, { 0, 0, int(texture.getSize().x), int(texture.getSize().y) });

		tilemap.init(texture, clip);
	}
};

class Player {
private:
	sf::CircleShape sprite;
	dgm::Circle body;
	dgm::Controller input;

	const float SPEED = 128.f;

	enum Actions {
		Up = 0, Left, Down, Right
	};

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time& time, const dgm::Mesh &level) {
		sf::Vector2f forward(0.f, 0.f);
		if (input.keyPressed(Actions::Up)) {
			forward.y = -SPEED;
		} else if (input.keyPressed(Actions::Down)) {
			forward.y = SPEED;
		}

		if (input.keyPressed(Actions::Left)) {
			forward.x = -SPEED;
		} else if (input.keyPressed(Actions::Right)) {
			forward.x = SPEED;
		}

		forward *= time.getDeltaTime();

		dgm::Collision::advanced(level, body, forward);
		body.move(forward);
		sprite.move(forward);
	}

	void spawn(const sf::Vector2f& position) {
		sprite.setPosition(position);
		body.setPosition(position);
	}

	sf::Vector2f getPosition() const {
		return body.getPosition();
	}

	Player() {
		float radius = 16.f;

		sprite.setRadius(radius);
		sprite.setFillColor(sf::Color::Green);
		sprite.setOrigin(radius, radius);
		body.setRadius(radius);

		input.setBinding(Actions::Up, sf::Keyboard::W);
		input.setBinding(Actions::Left, sf::Keyboard::A);
		input.setBinding(Actions::Down, sf::Keyboard::S);
		input.setBinding(Actions::Right, sf::Keyboard::D);
	}
};

int main() {
	dgm::Window window({1280, 720}, "Simple lights", false);
	dgm::Time time;
	dgm::ResourceManager resmgr = getInitializedResourceMgr();

	LightedLevel level(resmgr.get<sf::Texture>("tileset.png"));
	level.loadFromFile(rootDir + "/12_simple_lights.lvd");

	Player player;
	player.spawn({ 50.f, 50.f });

	auto view = window.getWindowContext().getView();
	view.zoom(0.5f);
	window.getWindowContext().setView(view);

	Light light = { player.getPosition(), 128.f, 255 };
	level.setLightSource(&light);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// LOGIC
		player.update(time, level.getMesh());
		light.position = player.getPosition();
		level.update();

		auto view = window.getWindowContext().getView();
		view.setCenter(player.getPosition());
		window.getWindowContext().setView(view);
		time.reset();

		// RENDER
		window.beginDraw();

		level.draw(window);
		player.draw(window);

		window.endDraw();
	}

	return 0;
}