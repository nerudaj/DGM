#include <DGM\dgm.hpp>

void dgm::Window::Open(const dgm::Config & config) {
	if (config.HasSection("Window")) {
		auto section = config["Window"];
		Open(sf::Vector2u(section["width"].AsInt(), section["height"].AsInt()), section["title"].AsString(), section["fullscreen"].AsBool());
	}
}

void dgm::Window::Open(const sf::Vector2u & resolution, const std::string & title, const bool fullscreen) {
	size = resolution;
	Window::title = title;
	Window::style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	isFullscreen = fullscreen;

	window.create({ resolution.x, resolution.y, 32 }, title, style);
}

void dgm::Window::Close() {
	window.close();
}

void dgm::Window::Close(dgm::Config & config) {
	config["Window"]["width"] = int(size.x);
	config["Window"]["height"] = int(size.y);
	config["Window"]["title"] = title;
	config["Window"]["fullscreen"] = isFullscreen;
	Close();
}

void dgm::Window::ToggleFullscreen() {
	Close();
	isFullscreen = !isFullscreen;
	Open(size, title, isFullscreen);
}

void dgm::Window::BeginDraw(const sf::Color &color) {
	window.clear(color);
}

void dgm::Window::Draw(sf::Drawable & drawable) {
	window.draw(drawable);
}

void dgm::Window::EndDraw() {
	window.display();
}
