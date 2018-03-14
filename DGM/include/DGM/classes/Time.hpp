#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Time {
	protected:
		sf::Clock clock;
		sf::Time elapsed;
		float deltaTime;

	public:
		float DeltaTime() const { return deltaTime; }

		const sf::Time &Elapsed() const { return elapsed; }

		void Reset() {
			elapsed = clock.restart();
			deltaTime = elapsed.asSeconds();
		}
	};
}