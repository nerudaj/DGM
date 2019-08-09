#pragma once

#include <DGM/dgm.hpp>
#include <TGUI\TGUI.hpp>

class AppStateMenuOptions : public dgm::AppState {
protected:
	tgui::Gui gui;
	const dgm::ResourceManager &resmgr;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMenuOptions(const dgm::ResourceManager &resmgr) : resmgr(resmgr) {}
};