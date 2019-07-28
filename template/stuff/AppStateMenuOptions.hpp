#pragma once

#include <DGM/dgm.hpp>
#include "Gui.hpp"

class AppStateMenuOptions : public dgm::AppState {
protected:
	sf::Text title;
	sf::Text labelFullscreen;
	gui::Button buttonBack;
	gui::Checkbox chkboxFullscreen;

	sf::Vector2i mousePos;
	const dgm::ResourceManager &resmgr;

	void processButtonClick(int id);

public:
	enum {
		Back
	};

	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMenuOptions(const dgm::ResourceManager &resmgr) : resmgr(resmgr) {}
};