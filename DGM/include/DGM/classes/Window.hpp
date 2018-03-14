#pragma once

#include <DGM\dgm.hpp>

namespace dgm {
	class Window {
	protected:
		sf::RenderWindow window;
		bool isFullscreen;
		sf::Vector2u size;
		std::string title;
		int style;

	public:
		void Open(const dgm::Config &config);
		void Open(const sf::Vector2u &resolution, const std::string &title, const bool fullscreen);
		void Close();
		void Close(dgm::Config &config);
		bool PollEvent(sf::Event &event) { return window.pollEvent(event); }
		void ToggleFullscreen();
		
		bool IsOpen() const { return window.isOpen(); }
		bool IsFullscreen() const { return isFullscreen; }
		const sf::Vector2u &GetSize() const { return size; }
		const sf::RenderWindow &GetWindowContext() { return window; }

		void BeginDraw(const sf::Color &color = sf::Color::Black);
		void Draw(sf::Drawable &drawable);
		void EndDraw();

		Window() {}
		~Window() {}
	};
};