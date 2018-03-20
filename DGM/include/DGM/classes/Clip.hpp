#pragma once

#include <DGM\dgm.hpp>
#include <vector>

namespace dgm {
	class Clip {
	protected:
		std::vector<sf::IntRect> frames;
		sf::Vector2i size;

	public:
		const sf::IntRect &getFrame(const std::size_t index) const { return frames[index]; }
		std::size_t getFrameCount() const { return frames.size(); }
		const sf::Vector2i &getFrameSize() const { return size; }

		bool init(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frameCount = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));

		Clip();
		Clip(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frameCount = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));
		~Clip();
	};
}