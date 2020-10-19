#pragma once

#include "GuiState.hpp"
#include "../Globals.hpp"

class AppStateMenuOptions : public dgm::AppState, public GuiState {
protected:
	Settings &settings;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMenuOptions(const dgm::ResourceManager &resmgr, Settings &settings) : GuiState(resmgr), settings(settings) {}
};