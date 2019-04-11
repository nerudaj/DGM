#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Window {
	protected:
		sf::RenderWindow window;
		bool fullscreen;
		sf::Vector2u size;
		std::string title;
		int style;

	public:
		void open(const cfg::Ini &config);
		void open(const sf::Vector2u &resolution, const std::string &title, const bool fullscreen);
		void close();
		void close(cfg::Ini &config);
		bool pollEvent(sf::Event &event) { return window.pollEvent(event); }
		void toggleFullscreen();
		
		bool isOpen() const { return window.isOpen(); }
		bool isFullscreen() const { return fullscreen; }

		const sf::Vector2u &getSize() const { return size; }
		sf::RenderWindow &getWindowContext() { return window; }
		const sf::RenderWindow &getWindowContext() const { return window; }

		void beginDraw(const sf::Color &color = sf::Color::Black);
		void draw(sf::Drawable &drawable);
		void endDraw();

		Window() {}
		~Window() {}
	};
};