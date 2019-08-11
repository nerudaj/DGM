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
		std::string title;
		int style;

	public:
		/**
		 *  \brief Open Window using a Ini config object
		 *
		 *  The \p config object should contain a [Window] section
		 *  and defined keys 'fullscreen', 'width', 'height' and 'title'.
		 *  If [Window] section is not defined, defaults will be used.
		 *  Defaults are: 1280x720, no title, no fullscreen.
		 */
		void open(const cfg::Ini &config);

		/**
		 *  \brief Open a window
		 *
		 *  \param[in]  resolution  Target window resolution
		 *  \param[in]  title       Caption in systray
		 *  \param[in]  fullscreen  Whether to start in fullscreen
		 */
		void open(const sf::Vector2u &resolution, const std::string &title, bool fullscreen);

		/**
		 *  \brief Close the window
		 */
		void close() { window.close(); }

		/**
		 *  \brief Close the Window and write configuration to Ini config object
		 */
		void close(cfg::Ini &config);
		bool pollEvent(sf::Event &event) { return window.pollEvent(event); }
		
		void toggleFullscreen();

		/**
		 *  \brief Change resolution of a Window
		 *
		 *  \warn  This will close and re-open the Window!
		 */
		void changeResolution(const sf::Vector2u& newResolution);
		
		bool isOpen() const { return window.isOpen(); }
		bool isFullscreen() const { return fullscreen; }

		sf::Vector2u getSize() const { return window.getSize(); }
		sf::RenderWindow &getWindowContext() { return window; }
		const sf::RenderWindow &getWindowContext() const { return window; }
		const std::string& getTitle() const { return title; }

		void beginDraw(const sf::Color &color = sf::Color::Black) { window.clear(color); }
		void draw(sf::Drawable &drawable) { window.draw(drawable); }
		void endDraw() { window.display(); }

		Window() : fullscreen(false) {}
		Window(const cfg::Ini &config) { open(config); }
		Window(const sf::Vector2u &resolution, const std::string &title, bool fullscreen) { open(resolution, title, fullscreen); }
		~Window() { if (isOpen()) close(); }
	};
};