#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	/**
	 *  \brief Wrapper around sf::RenderWindow
	 *
	 *  Class provides method for easier window management.
	 */
	class Window {
	protected:
		sf::RenderWindow window;
		bool fullscreen;
		sf::Vector2u size;
		std::string title;
		int style;

	public:
		void open(const cfg::Ini &config);
		void open(const sf::Vector2u &resolution, const std::string &title, bool fullscreen);
		void close() { window.close(); }
		void close(cfg::Ini &config);
		bool pollEvent(sf::Event &event) { return window.pollEvent(event); }
		void toggleFullscreen();
		
		bool isOpen() const { return window.isOpen(); }
		bool isFullscreen() const { return fullscreen; }

		const sf::Vector2u &getSize() const { return size; }
		sf::RenderWindow &getWindowContext() { return window; }
		const sf::RenderWindow &getWindowContext() const { return window; }

		void beginDraw(const sf::Color &color = sf::Color::Black) { window.clear(color); }
		void draw(sf::Drawable &drawable) { window.draw(drawable); }
		void endDraw() { window.display(); }

		Window() {}
		Window(const cfg::Ini &config) { open(config); }
		Window(const sf::Vector2u &resolution, const std::string &title, bool fullscreen) { open(resolution, title, fullscreen); }
		~Window() {}
	};
};