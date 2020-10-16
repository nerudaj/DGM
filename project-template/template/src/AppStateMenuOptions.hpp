#pragma once

#include <DGM/dgm.hpp>
#include <TGUI\TGUI.hpp>
#include "Globals.hpp"

class AppStateMenuOptions : public dgm::AppState {
protected:
	tgui::Gui gui;
	const dgm::ResourceManager &resmgr;
	Settings &settings;

	tgui::Label::Ptr createOptionLabel(tgui::Layout2d position, tgui::Layout2d size, const std::string& text);
	void createCheckbox(const std::string &label, tgui::Layout2d position, tgui::Layout2d size, bool checked, std::function<void(bool)> onChecked, const std::string &name);
	void createSlider(const std::string &label, tgui::Layout2d position, tgui::Layout2d size, float value, std::function<void(void)> onChange, const std::string& name, float lo = 0.f, float hi = 100.f, float step = 1.f);
	void createDropdown(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, const std::vector<std::string>& items, const std::string& selected, std::function<void(void)> onSelect, const std::string& name);
	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMenuOptions(const dgm::ResourceManager &resmgr, Settings &settings) : resmgr(resmgr), settings(settings) {}
};