/**
*  \file example05.cpp
*
*  \details This example is implementation of the game 15. It can be launched from
*  cmd with -l parameter to make game easier or harder by changing the grid size.
*  In this game you click the tiles next to empty space to move them to empty space.
*  Your goal is to move all tiles to their proper position to form the final image.
* 
*  Click the tiles with your mouse, reset the game using the R key. Launch the app
*  from folder where it has been build, not from debugger, debugger way would not
*  find the image.
*
*  Example demonstrates usage of dgm::Clip
*/

// Disallow linkage of sfml network module
#define NO_NETWORK

#include <DGM/dgm.hpp>
#include <random>

float power(float val, int exp) {
	float result = 1.f;
	while (exp-- > 0) result *= val;
	return result;
}

/**
 * An easing function for smooth tile transition
 */
float smoothStart(float x) {
	return power(x, 2);
}

class Tile {
private:
	sf::RectangleShape sprite;
	bool moving;
	sf::Vector2f startPos;
	sf::Vector2f destination;
	sf::Vector2f originOffset;
	float moveProgress;

	sf::Vector2f positionFromXY(int x, int y) {
		auto texRect = sprite.getTextureRect();
		sf::Vector2f texSize(float(texRect.width), float(texRect.height));
		sf::Vector2f position(x * texSize.x, y * texSize.y);
		return position + originOffset;
	}

public:
	void draw(dgm::Window &window) {
		window.draw(sprite);
	}

	void update(const dgm::Time &time) {
		// This whole function is about transitioning tile from startPos to destination
		const float TRANSITION_LENGTH = 0.2f;

		if (moving) {
			moveProgress += time.getDeltaTime();

			// Following equatation uses parametrical vector to position sprite to
			// moveProgress has to be normalized to <0.f, 1.f> before passing it to easing function
			sprite.setPosition((destination - startPos) * smoothStart(moveProgress / TRANSITION_LENGTH) + startPos);

			if (moveProgress >= TRANSITION_LENGTH) {
				sprite.setPosition(destination); 
				moving = false;
			}
		}
	}

	void sendTo(int x, int y) {
		destination = positionFromXY(x, y);
		startPos = sprite.getPosition();
		moving = true;
		moveProgress = 0.f;
	}

	void setPosition(int x, int y) {
		sprite.setPosition(positionFromXY(x, y));
	}

	void init(const sf::Texture &texture, const sf::IntRect &textureRect) {
		sf::Vector2f texSize(float(textureRect.width), float(textureRect.height));

		sprite.setSize(texSize);
		sprite.setTexture(&texture);
		sprite.setTextureRect(textureRect);
		sprite.setScale({ 0.95f, 0.95f });

		originOffset = texSize / 2.f;
		sprite.setOrigin(originOffset);
		moving = false;
	}
};

class GameBoard {
private:
	std::vector<Tile> tiles;
	std::vector<int> indices;
	sf::Texture texture;
	sf::Vector2f tileSize;
	int width;

	void initTiles(int level, const dgm::Clip &clip) {
		tiles.resize(level * level - 1);
		for (unsigned i = 0; i < unsigned(level * level) - 1; i++) {
			tiles[i].init(texture, clip.getFrame(i));
			tiles[i].setPosition(i % level, i / level);
		}

		tileSize = sf::Vector2f(clip.getFrameSize());
	}

	void initIndices(int level) {
		width = level;

		indices.resize(level * level);
		for (unsigned i = 0; i < indices.size(); i++) {
			indices[i] = i;
		}
		indices.back() = -1;
	}

	void shuffle() {
		std::random_device seed;
		std::mt19937 generator(seed());
		std::shuffle(indices.begin(), indices.end(), generator);

		int index = 0;
		for (int y = 0; y < width; y++) {
			for (int x = 0; x < width; x++) {
				if (indices[index] != -1) {
					tiles[indices[index]].sendTo(x, y);
				}

				index++;
			}
		}
	}

public:
	void draw(dgm::Window &window) {
		for (auto &tile : tiles) {
			tile.draw(window);
		}
	}

	void update(const dgm::Time &time) {
		for (auto &tile : tiles) {
			tile.update(time);
		}
	}

	void clicked(const sf::Vector2i &mousePos) {
		int x = mousePos.x / int(tileSize.x);
		int y = mousePos.y / int(tileSize.y);
		int index = y * width + x;

		if (x > 0 && indices[index - 1] == -1) {
			tiles[indices[index]].sendTo(x - 1, y);
			std::swap(indices[index], indices[index - 1]);
		}
		else if (x < width - 1 && indices[index + 1] == -1) {
			tiles[indices[index]].sendTo(x + 1, y);
			std::swap(indices[index], indices[index + 1]);
		}
		else if (y > 0 && indices[index - width] == -1) {
			tiles[indices[index]].sendTo(x, y - 1);
			std::swap(indices[index], indices[index - width]);
		}
		else if (y < width - 1 && indices[index + width] == -1) {
			tiles[indices[index]].sendTo(x, y + 1);
			std::swap(indices[index], indices[index + width]);
		}
	}

	void init(unsigned level) {
		sf::Vector2u texSize(texture.getSize());
		dgm::Clip clip(texSize / level, { 0, 0, int(texSize.x), int(texSize.y) });

		initTiles(level, clip);
		initIndices(level);

		shuffle();
	}
	
	GameBoard(const std::string &imagePath) {
		if (not texture.loadFromFile(imagePath)) {
			std::cerr << "Image could not be loaded\n" << std::endl;
			throw std::runtime_error("Image could not be loaded\n");
		}
	}
};

int main(int argc, char *argv[]) {
	// cfg namespace is from libconfig from dsh
	cfg::Args args("l:"); // start binary with -l <number> argument to have grid numberXnumber
	args.parse(argc, argv);

	int level = 3;

	if (args.isSet('l')) level = args.getArgumentValue('l').asInt();

	dgm::Window window({ 768, 768 }, "Sandbox", false);
	dgm::Time time;

	// You can also try a different image, but it has to be 768x768
	GameBoard board("../../examples/image.png");
	board.init(level);

	// Main loop
	sf::Event event;
	while (window.isOpen()) {
		// Poll events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::R) {
					board.init(level);
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				auto mousePos = sf::Mouse::getPosition(window.getWindowContext());
				board.clicked(mousePos);
			}
		}

		// Logic
		time.reset();

		board.update(time);

		// Rendering
		window.beginDraw(sf::Color::Black);

		board.draw(window);

		window.endDraw();
	}
	
	return 0;
}