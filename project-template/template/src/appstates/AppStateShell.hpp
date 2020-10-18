#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "../Globals.hpp"

class ShellModuleTutorial : public dgm::ShellModule {
public:
	virtual std::string getDescription() const override;

	ShellModuleTutorial();
};

class AppStateShell : public dgm::AppState {
private:
	const dgm::ResourceManager& resmgr;
	Settings& settings;

	dgm::Shell shell;
	dgm::ShellModuleVars shellVars;
	ShellModuleTutorial shellTutorial;
	int historyPtr = -1;

	tgui::Gui gui;

	void buildLayout();

public:
	// Inherited via AppState
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateShell(const dgm::ResourceManager& resmgr, Settings &settings) : resmgr(resmgr), settings(settings) {}
};