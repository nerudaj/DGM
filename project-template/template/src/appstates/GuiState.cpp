#include "GuiState.hpp"

tgui::Label::Ptr GuiState::createOptionLabel(tgui::Layout2d position, tgui::Layout2d size, const std::string& text) {
	tgui::Label::Ptr label = tgui::Label::create(text);
	label->setAutoSize(true);
	label->setSize(size);
	label->setPosition(position);
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	return label;
}

void GuiState::createCheckbox(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, bool checked, std::function<void(bool)> onChange, const std::string& name) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	tgui::CheckBox::Ptr checkbox = tgui::CheckBox::create();
	checkbox->setSize("10%", "100%");
	checkbox->setPosition("50%", "0%");
	checkbox->setChecked(checked);
	checkbox->connect("Changed", onChange);

	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(checkbox, name);
	panel->add(labelPtr);
	gui.add(panel);
}

void GuiState::createSlider(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, float value, std::function<void(void)> onChange, const std::string& name, float lo, float hi, float step) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	tgui::Slider::Ptr slider = tgui::Slider::create(lo, hi);
	slider->setSize("50%", "60%");
	slider->setPosition("50%", "20%");
	slider->setStep(step);
	slider->setValue(value);
	slider->connect("ValueChanged", onChange);

	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(slider, name);
	panel->add(labelPtr);
	gui.add(panel);
}

void GuiState::createDropdown(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, const std::vector<std::string>& items, const std::string& selected, std::function<void(void)> onSelect, const std::string& name) {
	auto labelPtr = createOptionLabel({ "0%", "0%" }, { "50%", "100%" }, label);

	// Dropdown for selecting resolution
	tgui::ComboBox::Ptr dropdown = tgui::ComboBox::create();
	dropdown->setSize("50%", "100%");
	dropdown->setPosition("50%", "0%");
	for (auto& item : items) {
		dropdown->addItem(item, item);
	}
	dropdown->setSelectedItem(selected);
	dropdown->connect("ItemSelected", onSelect);

	tgui::Panel::Ptr panel = tgui::Panel::create(size);
	panel->setPosition(position);
	panel->add(dropdown, name);
	panel->add(labelPtr);
	gui.add(panel);
}

void GuiState::createButton(const std::string& label, tgui::Layout2d position, tgui::Layout2d size, std::function<void(void)> onClick) {
	tgui::Button::Ptr button = tgui::Button::create(label);
	button->setSize(size);
	button->setPosition(position);
	button->connect("pressed", [this, onClick]() {
		soundbox.playSound("click.wav", 0);
		onClick();
	});
	gui.add(button);
}

GuiState::GuiState(const dgm::ResourceManager& resmgr) : resmgr(resmgr) {
	gui.setFont(resmgr.get<sf::Font>("cruft.ttf"));
}
