#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "../Soundbox.hpp"

class GuiState {
protected:
	tgui::Gui gui;
	const dgm::ResourceManager& resmgr;
	Soundbox soundbox = Soundbox(resmgr);

	tgui::Label::Ptr createOptionLabel(tgui::Layout2d position, tgui::Layout2d size, const std::string& text);
	void createCheckbox(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, bool checked, std::function<void(bool)> onChecked, const std::string& name);
	void createSlider(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, float value, std::function<void(void)> onChange, const std::string& name, float lo = 0.f, float hi = 100.f, float step = 1.f);
	void createDropdown(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, const std::vector<std::string>& items, const std::string& selected, std::function<void(void)> onSelect, const std::string& name);
	void createButton(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, std::function<void(void)> onClick);

	GuiState(const dgm::ResourceManager& resmgr);
};