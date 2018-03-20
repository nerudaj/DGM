#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Time {
	protected:
		sf::Clock clock;
		sf::Time elapsedp;
		float deltaTimep;

	public:
		float deltaTime() const { return deltaTimep; }

		const sf::Time &elapsed() const { return elapsedp; }

		void reset() {
			elapsedp = clock.restart();
			deltaTimep = elapsedp.asSeconds();
		}
	};
}