#pragma once

#include <DGM\dgm.hpp>

namespace gui {
	class Element {
	protected:
		int id;

	public:
		int getId() const {
			return id;
		}
	};

	class Highlightable : public Element {
	protected:
		sf::RectangleShape shape;

	public:
		bool isHighlighted(const sf::Vector2i &mousePos) const {
			return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
		}

		void update(const sf::Vector2i &mousePos) {
			if (isHighlighted(mousePos)) {
				shape.setOutlineColor(sf::Color::Yellow);
			}
			else {
				shape.setOutlineColor(sf::Color::Green);
			}
		}

		Highlightable() {
			shape.setFillColor({ 0, 128, 0 });
			shape.setOutlineThickness(3.f);
		}
	};

	class Button : public Highlightable {
	protected:
		sf::Text label;

	public:
		void draw(dgm::Window &window) {
			window.draw(shape);
			window.draw(label);
		}

		Button() {}

		Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &text, sf::Font &font, int id) {
			Element::id = id;

			shape.setPosition(position);
			shape.setSize(size);
			
			label.setFont(font);
			label.setString(text);
			label.setCharacterSize(unsigned(size.y * 0.6f));
			label.setPosition(position + size * 0.1f);
			label.setFillColor(sf::Color::White);
			label.setOutlineColor(sf::Color::Black);
		}
	};

	class Checkbox : public Highlightable {
	protected:
		bool value;

		void setFillColorBasedOnValue() {
			if (value) {
				shape.setFillColor({ 192, 192, 0 });
			}
			else {
				shape.setFillColor({ 0, 128, 0 });
			}
		}

	public:
		void draw(dgm::Window &window) {
			window.draw(shape);
		}

		bool getValue() const {
			return value;
		}

		void toggle() {
			value = !value;
			setFillColorBasedOnValue();
		}

		Checkbox() {}

		Checkbox(const sf::Vector2f&position, const sf::Vector2f &size, bool enabled, int id) {
			Element::id = id;

			shape.setPosition(position);
			shape.setSize(size);

			value = enabled;
			setFillColorBasedOnValue();
		}
	};
}