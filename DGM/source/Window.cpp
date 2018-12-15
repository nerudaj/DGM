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
	isFullscreen_ = fullscreen;

	window.create({ resolution.x, resolution.y, 32 }, title, style);
}

void dgm::Window::close() {
	window.close();
}

void dgm::Window::close(cfg::Ini & config) {
	config["Window"]["width"] = int(size.x);
	config["Window"]["height"] = int(size.y);
	config["Window"]["title"] = title;
	config["Window"]["fullscreen"] = isFullscreen_;
	close();
}

void dgm::Window::toggleFullscreen() {
	close();
	isFullscreen_ = !isFullscreen_;
	open(size, title, isFullscreen_);
}

void dgm::Window::beginDraw(const sf::Color &color) {
	window.clear(color);
}

void dgm::Window::draw(sf::Drawable & drawable) {
	window.draw(drawable);
}

void dgm::Window::endDraw() {
	window.display();
}
