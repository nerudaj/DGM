/**
 *  \file Clip.hpp
 *  \author doomista
 */

#pragma once

namespace dgm {
	/**
	 *  \brief Structure for holding texture clipping data
	 */
	class Clip {
	protected:
		unsigned int size;
		sf::IntRect *clips;

	public:
		/* METHODS */
		unsigned int		getSize() const;
		sf::Vector2i		getTileSize() const;
		const sf::IntRect	&getFrame(unsigned int index) const;
		
		bool loadFromFile(std::string filename, std::string identifier);
		bool loadFromMemory(sf::IntRect *data, unsigned int frames);
		bool loadFromParameters(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frames = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));
		
		void clear();
		
		/* CONSTRUCTORS */
		Clip();
		Clip(std::string filename, std::string identifier);
		Clip(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frames = 0, const sf::Vector2i &frameOffset = sf::Vector2i(0, 0));
		~Clip();
	};
};