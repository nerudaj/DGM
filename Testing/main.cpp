#include <DGM\dgm.hpp>

#ifdef _DEBUG
#pragma comment(lib, "dgm-d.lib")
#else
#pragma comment(lib, "dgm.lib")
#endif

/* This is just an internal testing sandbox */
int main(int argc, char *argv[]) {
	cfg::Ini config;
	if (not config.loadFromFile("../Data/config.ini")) return 1;

	dgm::Window window;
	window.open(config);

	std::vector<int> imageData = {
		0, 1, 2, 1, 0,
		1, 2, 3, 2, 1,
		1, 2, 3, 2, 1,
		1, 2, 3, 2, 1,
		0, 1, 2, 1, 0
	};
	std::vector<int> collisionData = {
		1, 0, 0, 0, 1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		1, 0, 0, 0, 1
	};
	dgm::Clip clip({ 64,64 }, { 0,0,256,64 });
	
	dgm::ResourceManager resmgr;
	std::vector<std::string> names;
	if (not resmgr.loadFromDir("../Graphics", dgm::ResourceManager::Type::Graphic, &names)) {
		return 2;
	}

	dgm::Level level;
	if (not level.loadFromFile("../Data/level.txt")) {
		level.build(clip, { 64,64 }, imageData, collisionData, { 5,5 });
	}
	level.getRenderer().setTexture(resmgr.get<sf::Texture>("tileset"));

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		/* LOGIC */

		/* DRAW */
		window.beginDraw();

		level.draw(window);

		window.endDraw();
	}

	level.saveToFile("../Data/level.txt");
	config.saveToFile("../Data/config.ini");

	return 0;
}