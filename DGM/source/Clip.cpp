#pragma once

#include <DGM\dgm.hpp>
#include <cassert>

bool dgm::Clip::init(const sf::Vector2i & frameSize, const sf::IntRect & boundaries, unsigned int frameCount, const sf::Vector2i & frameOffset) {
	assert(frameSize.x > 0 and frameSize.y > 0);
	assert(boundaries.width > 0 and boundaries.height > 0);


	if (frameCount == 0) {
		sf::Vector2i blockSize = frameSize + frameOffset;
		sf::Vector2i chunkSize(boundaries.width, boundaries.height);
		frameCount = (chunkSize.x / blockSize.x) * (chunkSize.y / blockSize.y);
	}

	try {
		frames.resize(frameCount);
	}
	catch (std::bad_alloc &e) {
		std::cerr << "ERROR:Clip: " << e.what() << ".\n";
		return false;
	}

	Clip::size = frameSize;

	unsigned int i = 0;
	for (unsigned long y = boundaries.top; y < unsigned long(boundaries.top + boundaries.height); y += (frameSize.y + frameOffset.y)) {
		for (unsigned long x = boundaries.left; x < unsigned long(boundaries.left + boundaries.width); x += (frameSize.x + frameOffset.x)) {
			frames[i] = sf::IntRect(x, y, frameSize.x, frameSize.y);
			i++;

			if (i == frameCount) {
				x = (boundaries.left + boundaries.width);
				y = (boundaries.top + boundaries.height);
				break;
			}
		}
	}

	return true;
}

dgm::Clip::Clip() {
}

dgm::Clip::Clip(const sf::Vector2i & frameSize, const sf::IntRect & boundaries, unsigned int frameCount, const sf::Vector2i & frameOffset) {
	init(frameSize, boundaries, frameCount, frameOffset);
}

dgm::Clip::~Clip() {
}
