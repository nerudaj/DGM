#include <DGM\dgm.hpp>

void dgm::Window::open(const cfg::Ini & config) {
	if (config.hasSection("Window")) {
		auto section = config["Window"];
		open(sf::Vector2u(section["width"].asInt(), section["height"].asInt()), section["title"].asString(), section["fullscreen"].asBool());
	}
}

void dgm::Window::open(const sf::Vector2u & resolution, const std::string & title, const bool fullscreen) {
	size = resolution;
	Window::title = title;
	Window::style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	Window::fullscreen = fullscreen;

	window.create({ resolution.x, resolution.y, 32 }, title, style);
}

void dgm::Window::close(cfg::Ini & config) {
	config["Window"]["width"] = int(size.x);
	config["Window"]["height"] = int(size.y);
	config["Window"]["title"] = title;
	config["Window"]["fullscreen"] = fullscreen;
	close();
}

void dgm::Window::toggleFullscreen() {
	close();
	fullscreen = !fullscreen;
	open(size, title, fullscreen);
}
