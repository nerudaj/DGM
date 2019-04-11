#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Time {
	private:
		sf::Clock clock;
		sf::Time  elapsed;
		float     deltaTime;

	public:
		float getDeltaTime() const { return deltaTime; }

		const sf::Time &getElapsed() const { return elapsed; }

		void reset() {
			elapsed = clock.restart();
			deltaTime = elapsed.asSeconds();
		}
	};
}