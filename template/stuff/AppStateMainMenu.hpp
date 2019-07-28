#pragma once

#include <DGM\dgm.hpp>
#include "Gui.hpp"

class AppStateMainMenu : public dgm::AppState {
private:
	sf::Text text;
	dgm::ResourceManager resmgr;
	std::string rootDir;
	std::vector<gui::Button> buttons;
	sf::Vector2i mousePos;

	void processButtonClick(int id);

public:
	enum {
		Play,
		Options,
		Quit
	};

	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMainMenu(const std::string &resourcesRoot) : rootDir(resourcesRoot) {};
};