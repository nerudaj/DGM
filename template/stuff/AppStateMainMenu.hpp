#pragma once

#include <DGM\dgm.hpp>
#include <TGUI\TGUI.hpp>

class AppStateMainMenu : public dgm::AppState {
private:
	sf::Text text;
	dgm::ResourceManager resmgr;
	std::string rootDir;
	tgui::Gui gui;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMainMenu(const std::string &resourcesRoot) : rootDir(resourcesRoot) {};
};