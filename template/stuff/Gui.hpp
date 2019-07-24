#pragma once

#include <DGM\dgm.hpp>

namespace gui {
	class Button {
	private:
		sf::Text label;
		sf::RectangleShape shape;
		int id;

	public:
		void draw(dgm::Window &window) {
			window.draw(shape);
			window.draw(label);
		}

		void update(const sf::Vector2i &mousePos) {
			if (isHighlighted(mousePos)) {
				shape.setOutlineColor(sf::Color::Yellow);
			}
			else {
				shape.setOutlineColor(sf::Color::Green);
			}
		}

		bool isHighlighted(const sf::Vector2i &mousePos) const {
			return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
		}

		int getId() const {
			return id;
		}

		Button() {}

		Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &text, sf::Font &font, int id) : id(id) {
			shape.setPosition(position);
			shape.setSize(size);
			shape.setFillColor({ 0, 128, 0 });
			shape.setOutlineThickness(3.f);

			label.setFont(font);
			label.setString(text);
			label.setCharacterSize(unsigned(size.y * 0.6f));
			label.setPosition(position + size * 0.1f);
			label.setFillColor(sf::Color::White);
			label.setOutlineColor(sf::Color::Black);
		}
	};
}