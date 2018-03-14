#pragma once

#include <DGM\dgm.hpp>
#include <vector>

namespace dgm {
	class Clip {
	protected:
		std::vector<sf::IntRect> frames;
		sf::Vector2i size;

	public:
		const sf::IntRect &GetFrame(const std::size_t index) const { return frames[index]; }
		std::size_t GetFrameCount() const { return frames.size(); }
		const sf::Vector2i &GetFrameSize() const { return size; }

		bool Init(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frameCount = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));

		Clip();
		Clip(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frameCount = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));
		~Clip();
	};
}