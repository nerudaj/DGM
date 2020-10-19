#pragma once

#include "GuiState.hpp"
#include "../Globals.hpp"

class AppStateMainMenu : public dgm::AppState, public GuiState {
private:
	Settings settings = { 50.f, 50.f };
	bool viewShouldBeUpdated = true;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMainMenu(const dgm::ResourceManager &resmgr) : GuiState(resmgr) {}
};