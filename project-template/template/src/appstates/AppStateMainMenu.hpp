#pragma once

#include "GuiState.hpp"
#include "../Globals.hpp"

class AppStateMainMenu : public dgm::AppState, public GuiState {
private:
	Settings &settings;
	bool viewShouldBeUpdated = true;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMainMenu(const dgm::ResourceManager &resmgr, Settings &settings) : GuiState(resmgr), settings(settings) {}
};